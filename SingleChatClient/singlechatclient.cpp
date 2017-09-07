#include "singlechatclient.h"
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <iostream>
//#include <QMetaType>

using std::vector;
using std::list;
using std::cout;
using std::endl;
using std::string;
using namespace boost;
using system::error_code;

SingleChatClient::SingleChatClient():m_sock(m_io),
    m_ep(address_type::from_string("127.0.0.1"),6688)
{
    m_friendList.reset(new FriendList(this));
    m_messageModel.reset(new MessageModel(this));
    m_chatMsgModel.reset(new ChatMsgModel(this));
    m_addNoticeModel.reset(new AddFriendNoticeModel(this));
    myConnect();
    connect(this,&SingleChatClient::sig_loadDataToFriendList,
            m_friendList.data(),&FriendList::slot_loadDataToFriendList);
    //    qRegisterMetaType<std::string>("string");
    connect(this,&SingleChatClient::sig_recvMsgsignal,this,&SingleChatClient::slot_handerMsg);
    connect(this,&SingleChatClient::sig_recvNotice,this,&SingleChatClient::slot_handerNoticeMsg);
}

void SingleChatClient::myConnect()
{
    m_sock.connect(m_ep);
    cout << "Client Start..." << endl;
}

bool SingleChatClient::requestVd(QString userName, QString password)
{
    m_selfAccountN = userName;
    string user = userName.toStdString();
    string pass = password.toStdString();
    string s = VALIDATION_INFO;
    write(s + "^" + user + "^" + pass);
    vector<char> buf(100,0);
    m_sock.receive(asio::buffer(buf));
    //    cout << "validation:" << &buf[0] <<endl; //
    buf.erase(buf.begin()); //去掉消息的头部
    string msg = &buf[0];
    if(msg.find("true") == 0)
    {
        return true;
    }
    else
        return false;
}

void SingleChatClient::getAccount(QString userName)
{
    string user = userName.toStdString();
    string s = ACCOUNT_INFO;
    write( s + "^" + user);
    vector<char> buf(100,0);
    m_sock.receive(asio::buffer(buf));
    cout << "account: "  << &buf[0] << endl;
    buf.erase(buf.begin());
    vector<string> result;
    split(result,buf,is_any_of("^"),token_compress_on);
    m_self_AccountInfo = convert_vector(result);
}

void SingleChatClient::getFriends(QString userName)
{
    string user = userName.toStdString();
    string s = FRIEND_INFO;
    write(s + "^" + user);
    vector<char> buf(200,0);
    m_sock.receive(asio::buffer(buf));
    cout << "Friends: " << &buf[0] << endl;
    buf.erase(buf.begin());
    vector<string> allFri; //allFri里面的每一项代表着每个好友的信息，allFri装的是所有好友的信息
    split(allFri,buf,is_any_of("#"),token_compress_on);//拆分各个好友
    for(auto &e : allFri)
    {
        //friInfo里面的每一项代表着各个信息，friInfo装的是好友的分组/帐号/昵称/备注/个性签名
        vector<string> friInfo;
        split(friInfo,e,is_any_of("^")/*,token_compress_on*/);
        QStringList info = convert_vector(friInfo);
        emit sig_loadDataToFriendList(info);
    }
    //    return allFri;
}

//vector<string> SingleChatClient::getFriendFromQml(vector<string> frdVec)
//{
//    allFreinds =
//    return frdVec;
//}

//int SingleChatClient::findFrd(QString accountNum)
//{
//    vector<string> vec = getFriendFromQml();
//}

void SingleChatClient::getOfflineMessage(QString userName)
{
    string user = userName.toStdString();
    string s = OUTLINE_INFO;
    write(s + "^" + user);
    vector<char> buf(300,0);
    m_sock.receive(asio::buffer(buf));
    cout << "OfflineMessage:" << &buf[0] << endl;
    buf.erase(buf.begin());
    if(!buf[1])
        return;
    //    buf.erase(buf.end()--); //去掉最后的#号
    vector<string> allmsg;
    split(allmsg,buf,is_any_of("#"),token_compress_on);
    auto iter = allmsg.end()--;
    allmsg.erase(iter);
    cout << "Offline length:" << allmsg.size() << endl;
    allmsg = my_remove(allmsg);
    for(auto &e : allmsg)
    {
        vector<string> frimsg;
        split(frimsg,e,is_any_of("^"),token_compress_on);
        QString msg1 = QString::fromStdString(frimsg[0]);
        QString msg2 = QString::fromStdString(frimsg[1]);
        QString msg3 = QString::fromStdString(frimsg[2]);
        m_messageModel->addDataToModel(msg1,msg2,msg3);
    }
    m_unReadMessageCount = m_messageModel->getMessageModel()->count();
}

void SingleChatClient::getOffNotice(QString accountNum)
{

    string actN = accountNum.toStdString();
    string s = GET_NOTICE;
    write(s + "^" + actN);
    vector<char> buf(300,0);
    m_sock.receive(asio::buffer(buf));
    cout << "离线通知2：" << &buf[0] << endl;
    buf.erase(buf.begin());
    vector<string> allNotice;
    string str = &buf[0];
    split(allNotice,str,is_any_of("#"),token_compress_on);
    allNotice.erase(allNotice.end()--);
    if(!buf[1])
        return;
    for(auto &e:allNotice)
    {
        cout << "离线通知3：" << &buf[0] << endl;
        vector<string> oneNotice;
        split(oneNotice,e,is_any_of("^"),token_compress_on);
        QString accountN = QString::fromStdString(oneNotice[0]);
        QString nickName = QString::fromStdString(oneNotice[1]);
        QString addExplain = QString::fromStdString(oneNotice[2]);
        QString time = QString::fromStdString(oneNotice[3]);
        QDateTime sendTime = QDateTime::fromString(time,"yyyy-MM-dd hh:mm:ss");
        m_addNoticeModel->addDataToModel(accountN,nickName,addExplain,sendTime);
        cout << "离线通知4：" << &buf[0] << endl;
    }
    cout << "离线通知5：" << m_addNoticeModel->getAddFriendNoticeItemModel()->count() << endl;

}

vector<string> SingleChatClient::my_remove(vector<string> msg)
{
    int i=0;
    vector<string> my_msg;
    for(vector<string>::const_iterator its = --msg.end();its != msg.begin();--its)
    {
        my_msg.push_back(*its);
    }
    my_msg.push_back(*msg.begin());
    //    cout << &my_msg[0] << endl;
    for(vector<string>::const_iterator it = my_msg.begin();it != my_msg.end();){
        string str= *it;
        for(int i=0;i< (int)str.size();i++){
            size_t pos;
            pos = str.find("^",i);
            str = str.substr(0,pos);
        }
        i++;
        for(vector<string>::const_iterator its = my_msg.begin()+i;its != my_msg.end();){
            string str1= *its;
            for(int i=0;i<(int)str1.size();i++){
                size_t pos;
                pos = str1.find("^",i);
                str1 = str1.substr(0,pos);
            }
            if(str == str1){
                its = my_msg.erase(its);
            }else{
                ++its;
            }
        }
        ++it;
    }
    return my_msg;
}

void SingleChatClient::clearSelectedFriend()
{
    m_selectedFriendAccount = "";
}

void SingleChatClient::receiveMessage()
{
    asio::io_service io;
    asio::ip::udp::endpoint recv_ep(asio::ip::udp::v4(),6699);
    asio::ip::udp::socket sock(io,recv_ep);

    for(;;){
        vector<char> recvBuf(100,0);
        sock.receive_from(asio::buffer(recvBuf),recv_ep);
        cout << &recvBuf[0] << endl;
        string str = &recvBuf[0];
        QString qstr = QString::fromStdString(str);
        if(recvBuf[0] == MESSAGE)
        {
            recvBuf.erase(recvBuf.begin());
            emit sig_recvMsgsignal(qstr);
        }
        else if(recvBuf[0] == ADD_FRIEND_NOTICE)
        {
            recvBuf.erase(recvBuf.begin());
            emit sig_recvNotice(qstr);
        }
        recvBuf.clear();
    }
}

void SingleChatClient::slot_handerNoticeMsg(QString str)
{
    vector<string> allNotice;
    string mystr = str.toStdString();
    split(allNotice,mystr,is_any_of("#"),token_compress_on);
    for(auto &e:allNotice)
    {
        vector<string> oneNotice;
        split(oneNotice,e,is_any_of("^"),token_compress_on);
        QString accountN = QString::fromStdString(oneNotice[0]);
        QString nickName = QString::fromStdString(oneNotice[1]);
        QString addExplain = QString::fromStdString(oneNotice[2]);
        QString time = QString::fromStdString(oneNotice[3]);
        QDateTime sendTime = QDateTime::fromString(time,"yyyy-MM-dd hh:mm:ss");
        m_addNoticeModel->addDataToModel(accountN,nickName,addExplain,sendTime);
    }
}

void SingleChatClient::slot_handerMsg(QString str){
    vector<string> allmsg;
    string mystr = str.toStdString();
    split(allmsg,mystr,is_any_of("#"),token_compress_on);
    for(auto &e : allmsg)
    {
        vector<string> frimsg;
        split(frimsg,e,is_any_of("^"),token_compress_on);
        QString msg1 = QString::fromStdString(frimsg[0]);
        QString msg2 = QString::fromStdString(frimsg[1]);
        QString msg3 = QString::fromStdString(frimsg[2]);
        QString time = QString::fromStdString(frimsg[3]);
        QDateTime sendTime = QDateTime::fromString(time,"yyyy-MM-dd hh:mm:ss");
        if(msg2 == m_selectedFriendAccount /*|| msg2 == m_messageModel->getfrimsgNum()*/){
            //将收到的消息添加到聊天Model里
            m_chatMsgModel->addDataToModel(msg1,msg3,1,sendTime);
        }else {
            //将收到的消息添加到消息Model中
            if(m_messageModel->getMessageModel()->count() > 0)
            {
                QList<MessageItem *> list = m_messageModel->getMessageModel()->toList();
                int j = 0;
                for(auto a : list)
                {
                    if(a->get_nackName() == msg1){
                        //消息来自同一个人
                        a->set_message(msg3);
                        j = 1;
                        addOfflineMsgCount();
                    }
                }

                if(0 == j) {
                    //消息不是来自同一个人，将消息加进聊天模型
                    m_messageModel->addDataToModel(msg1,msg2,msg3);
                    addOfflineMsgCount();
                }
            } else {
                m_messageModel->addDataToModel(msg1,msg2,msg3);
                addOfflineMsgCount();
            }
        }
    }
}



void SingleChatClient::getChatRecords(QString userName)
{
    string s = CHATRECORD;
    string friendN = userName.toStdString();
    write(s + "^" + m_selfAccountN.toStdString() + "^" + friendN);
    vector<char> buf(512,0);
    m_sock.receive(asio::buffer(buf));
    buf.erase(buf.begin());//去掉头部

    cout << &buf[0] << endl;
    vector<string> allmsg;
    split(allmsg,buf,is_any_of("#"),token_compress_on); //拆分收到的好友的聊天记录
    auto iter = allmsg.end()--;
    allmsg.erase(iter);
    cout << "ChatRecord length:" << allmsg.size() << endl;
    for(auto e : allmsg) {
        vector<string> eMsg;
        split(eMsg,e,is_any_of("^"),token_compress_on); //拆分每一条聊天记录，以便加载模型
        QString content = QString::fromStdString(eMsg[0]);
        QString time = QString::fromStdString(eMsg[1]);
        QString sender = QString::fromStdString(eMsg[2]);
        QDateTime sendTime = QDateTime::fromString(time,"yyyy-MM-dd hh:mm:ss"); // 将时间转换成QDateTime
        //        cout << sendTime.toString().toStdString() << endl;
        int status = 1;
        if(sender == m_self_AccountInfo[0]) { // 如果发送者是当前用户，就设置消息的显示状态为0
            status = 0;
            m_chatMsgModel->addDataToModel("me",content,status,sendTime);
        } else {
            m_chatMsgModel->addDataToModel(sender,content,status,sendTime);
        }
    }
}


FriendList *SingleChatClient::getFriendList() const
{
    return m_friendList.data();
}

QString SingleChatClient::getSelfAccountN() const
{
    return m_selfAccountN;
}

MessageModel *SingleChatClient::getMsgModel() const
{
    return m_messageModel.data();
}

ChatMsgModel *SingleChatClient::getChatMsgModel() const
{
    return m_chatMsgModel.data();
}

AddFriendNoticeModel *SingleChatClient::getAddNoticeModel() const
{
    return m_addNoticeModel.data();
}

QStringList SingleChatClient::selfAccountInfo() const
{
    return m_self_AccountInfo;
}

QString SingleChatClient::getSelectedFriendAccount() const
{
    return m_selectedFriendAccount;
}

void SingleChatClient::sendMessage(QString msg)
{
    string temp = msg.toStdString();
    cout << temp << endl;
    string s = SEND_MESSAGE_INFO;
    write(s + "^" + temp);
}

void SingleChatClient::clientExit(QString accountNum)
{
    string s = EXIT_INFO;
    s += "^" + accountNum.toStdString();
    write(s);
    cout << "Client exit" << endl;
}

void SingleChatClient::removeChatMsgModel()
{
    m_chatMsgModel->getChatMsgItemModel()->clear();
}

void SingleChatClient::subOfflineMsgCount()
{
    if(m_unReadMessageCount != 0){
        m_unReadMessageCount -= 1;
        emit unReadMsgCountChanged(m_unReadMessageCount);
    }
}

void SingleChatClient::createGroup(QString groupInfo)
{
    string info = groupInfo.toStdString();
    string s = CREATE_GROUP;
    s += "^";
    s += getSelfAccountN().toStdString();
    write(s + "^" + info);
}

void SingleChatClient::moveFriendGroup(QString groupInfo)
{
    //groupInfo里面包括好友的帐号以及分组的名字并且用^分隔
    string info = groupInfo.toStdString();
    string s = CHANGE_GROUP;
    s += "^";
    s += getSelfAccountN().toStdString();
    write(s + "^" + info);
}

void SingleChatClient::modifyRemarks(QString modifyInfo)
{
    string info = modifyInfo.toStdString();
    string s = MODIFY_REMARKS;
    s += "^";
    s += getSelfAccountN().toStdString();
    write(s + "^" + info);
}

void SingleChatClient::registerToDatabase(QString accountInfo)
{
    string info = accountInfo.toStdString();
    string s = REGISTER_INFO;
    write(s + "^" + info);
}

//当选择好友之后，记录所选好友的帐号和组名
void SingleChatClient::setSelectedFriend(QString friendAccount)
{
    m_selectedFriendAccount = friendAccount;
}

int SingleChatClient::findFriend(QString accountNum)
{
    FriendItemModel * fim = getFriendList()->getFriendItemModel();
    if(fim->count() > 0)
    {
        QList<FriendItem *> list = fim->toList();
        foreach(auto e, list)
        {
            QList<FriendItem *> lst =  e->get_chs()->toList();
            foreach(auto g,lst)
            {
                cout <<g->get_account().toStdString() << endl;
                if(g->get_account() == accountNum)
                {
                    return 1;
                }
            }
        }
    }
    return 0;

}

QString SingleChatClient::getUserInfo(QString accountNum)
{
    string info = accountNum.toStdString();
    string s = GET_USER_INFO;
    write(s + "^" + info);
    vector<char> buf(512,0);
    m_sock.receive(asio::buffer(buf));
    buf.erase(buf.begin());//去掉头部
    QString str = &buf[0];
    return str;
}

void SingleChatClient::requestAdd(QString selfAccountNum, QString frdAccountNum, QString selfNickName, QString addExplain)
{
    string info = selfAccountNum.toStdString() + "^" + frdAccountNum.toStdString() + "^" + selfNickName.toStdString() + "^" + addExplain.toStdString();
    string s = ADD_FRIEND;
    write(s + "^" + info);
}

void SingleChatClient::sendAnswerNotice(QString msg,QString selfAccountN, QString frdAccount, QString selfNickName, QString frdNickName,QDateTime time)
{

    QString t = time.toString("yyyy-MM-dd hh:mm:ss");
    string info = msg.toStdString() + "^" + selfAccountN.toStdString() + "^" + frdAccount.toStdString() + "^" + selfNickName.toStdString()  + "^" + frdNickName.toStdString() + "^" + t.toStdString();
    string s = FRIEND_FEEDBACK;
    write(s + "^" + info);
}

QString SingleChatClient::getUserNickName(QString s)
{
    vector<string> info1,info2;
    string str = s.toStdString();
    split(info1,str,is_any_of("#"),token_compress_on);
    split(info2,info1[0],is_any_of("^"),token_compress_on);
    return QString::fromStdString(info2[0]);
}

QString SingleChatClient::getUserSelfInfo(QString s)
{
    vector<string> info1,info2;
    string str = s.toStdString();
    split(info1,str,is_any_of("#"),token_compress_on);
    split(info2,info1[0],is_any_of("^"),token_compress_on);
    return QString::fromStdString(info2[1]);
}





void SingleChatClient::write(const string &msg)
{
    m_sock.send(asio::buffer(msg));
}

QStringList SingleChatClient::convert_vector(const vector<string> &vec)
{
    QStringList account;
    for(vector<string>::const_iterator iter = vec.begin();iter != vec.end();++iter){
        const char *s = (*iter).c_str();
        account.append(s);
    }
    return account;
}

void SingleChatClient::addOfflineMsgCount()
{
    m_unReadMessageCount += 1;
    emit unReadMsgCountChanged(m_unReadMessageCount);
}


