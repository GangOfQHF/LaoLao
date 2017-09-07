#include "singlechatserver.h"
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/bind.hpp>
#include <iostream>
#include <exception>
#include "account.h"
#include "database.h"
#include <memory>
#include <boost/chrono.hpp>
#include "protocol.h"
#include <time.h>

using std::cout;
using std::endl;
using std::string;
using std::vector;
using boost::mutex;
using boost::lock_guard;
using std::map;
using std::time;
using std::time_t;
using std::strftime;


//新添加
using namespace boost::asio;




SingleChatServer::SingleChatServer():m_buff(512,0),m_acceptor(m_io,endpoint_type(ip::tcp::v4(),6688))
{
    accept();
}

void SingleChatServer::accept()
{
    sock_ptr sock(new socket_type(m_io));
    m_acceptor.async_accept(*sock,boost::bind(&this_type::acceptHandler,this,
                                              boost::asio::placeholders::error,sock));
}

void SingleChatServer::acceptHandler(const SingleChatServer::error_code &ec, SingleChatServer::sock_ptr sock)
{
    if(ec)
        return;
    cout << "Client:" << sock->remote_endpoint().address() << endl;
    sock->async_receive(buffer(m_buff),boost::bind(&this_type::readHandler,this,
                                                   boost::asio::placeholders::error,sock));
    accept();
}

void SingleChatServer::readHandler(const SingleChatServer::error_code &ec, SingleChatServer::sock_ptr sock)
{
    string msg = &m_buff[0]; //获取收到的数据
    //    cout << "收到的消息为：" << msg << endl;
    if(0 == msg.find(VALIDATION_INFO))
        validationHandler(ec,sock,msg);
    else if(0 == msg.find(ACCOUNT_INFO))
        getAccountHandler(ec,sock,msg);
    else if(0 == msg.find(FRIEND_INFO))
        getFriendHandler(ec,sock,msg);
    else if(0 == msg.find(OUTLINE_INFO))
        getOutlineMsgHandler(ec,sock,msg);
    else if(0 == msg.find(FRIEND_REQUEST))//获取好友请求
        getFriendRequest(ec,sock,msg);
    else if(0 == msg.find(REGISTER))
        registerNewUser(ec,msg);
    else if(0 == msg.find(REMARK))  // 修改备注
        changeRemarkHandler(ec,msg);
    else if(0 == msg.find(CREATE_GROUP)) //创建组
        addFriendGroupHandler(ec,msg);
    else if(0 == msg.find(CHANGE_GROUP))
        changeGroupHandler(ec,msg);
    else if(0 == msg.find(DELETE_FRIEND)) //删除好友
        deleteFriendHandler(ec,msg);
    else if(0 == msg.find(FIND_FRIEND))//查找好友
        findFriendHAndler(ec,sock,msg);
    else if(0 == msg.find(ADD_FRIEND))//添加好友
        addFriendHandler(ec,msg);
    else if(0 == msg.find(FRIEND_FEEDBACK))//添加好友反馈
        addFriendFeedback(ec,msg);
    else if(0 ==  msg.find(SEND_MESSAGE_INFO))
        sendMsg(ec,msg);
    else if(0 == msg.find(CHARTRECORD))
        getChatRecords(ec,sock,msg);
    else if(0 == msg.find(EXIT_INFO))
        exitHandler(ec,msg);
    for(auto &e : m_buff) //清空缓冲区
        e = '\0';
    //再次异步接收消息
    sock->async_receive(buffer(m_buff),boost::bind(&this_type::readHandler,this,
                                                   boost::asio::placeholders::error,sock));
}



void SingleChatServer::validationHandler(const SingleChatServer::error_code &ec,
                                         SingleChatServer::sock_ptr sock, std::string msg)
{
    if(ec)
        return;
    cout << "The request is " <<  msg << endl;
    Database *database = new Database();
    Account account;
    string accountN;//自己帐号
    string accountIP;//获取登录时的ip地址
    accountIP = sock->remote_endpoint().address().to_string();
    vector<string> vec;
    boost::split(vec,msg,boost::is_any_of("^"),boost::token_compress_on); // vec装的是收到的所有消息拆分后的

    auto it = vec.begin();
    string password;
    accountN = *(++it);
    password = *(++it);

    //判断该用户是否已经登录
    map<string,Account>::iterator iter = m_account.find(accountN);
//    if(iter != m_account.end())
//    {
//        sock->send(boost::asio::buffer("false"));
//    }
//    else{
        account = requestVd(accountN,password,database);
        if(account.getNickName()!=""){
            string info = VALIDATION_INFO;
            info += "true";
            sock->send(boost::asio::buffer(info));
            m_account.insert(std::pair<string,Account>(accountN,account));
            m_actIp.insert(std::pair<string,string>(accountN,accountIP) );
        }
        else
        {
            sock->send(boost::asio::buffer("false"));
        }
//    }

}

void SingleChatServer::changeGroupHandler(const SingleChatServer::error_code &ec, std::string msg)
{
    cout << "moving group" << endl;
    if(ec)
        return;
    vector<string> vec;
    boost::split(vec,msg,boost::is_any_of("^"));
    vec.erase(vec.begin());
    Database *database = new Database();
    Account account;
    account.setDatabase(database);
    //vec[0]是自己的帐号，vec[1]是好友的帐号，vec[2]是原来的分组的名字,vec[3]是要移动到的分组名
//    cout << vec[0] << " " << vec[1] << " " << vec[2] << " " << vec[3] << endl;
    account.changeFriendGroup(vec[0],vec[1],vec[2],vec[3]);
}

void SingleChatServer::getAccountHandler(const SingleChatServer::error_code &ec,
                                         SingleChatServer::sock_ptr sock,std::string msg)
{
    if(ec)
        return;
    vector<string> vec;
    boost::split(vec,msg,boost::is_any_of("^"));
    string accountNum = *(++vec.begin());
    map<string,Account>::iterator iter = m_account.find(accountNum);
    string info = ACCOUNT_INFO;
    Account account = iter->second;
    info += getAccount(account);
    sock->send(boost::asio::buffer(info));
}

void SingleChatServer::getFriendHandler(const SingleChatServer::error_code &ec, SingleChatServer::sock_ptr sock,std::string msg)
{
    if(ec)
        return;
    cout << msg << endl;
    //获取好友信息(各个好友之间用#分隔)
    vector<string> vec;
    boost::split(vec,msg,boost::is_any_of("^"));
    string accountNum = *(++vec.begin());
    map<string,Account>::iterator iter = m_account.find(accountNum);
    Account account = iter->second;
    cout << account.getExplain() << endl;

    vector<string> friends;
    friends = getFriends(getGroups(account));
    cout << account.getNickName();
    string info = FRIEND_INFO;
    int i = 0;
    for(auto f : friends)
        cout << f << endl;
    for(auto &f:friends)
    {
        if(i != 0)
            info += "#";
        info += f;
        ++i;
    }
    sock->send(boost::asio::buffer(info));
}

void SingleChatServer::getOutlineMsgHandler(const SingleChatServer::error_code &ec, SingleChatServer::sock_ptr sock,std::string msg)
{
    if(ec)
        return;
    vector<string> vec;
    cout << "收到的消息为：" << msg << endl;
    boost::split(vec,msg,boost::is_any_of("^"));
    string accountNum = *(++vec.begin());
    vector<string> outlineMsg = getOutlineMsg(accountNum);
    string msg2 = OUTLINE_INFO;
    for(auto &e:outlineMsg)
    {
        msg2 += e;
        msg2 += "#";//将多条离线记录用#连接
    }

    cout <<"获取的离线消息：" << msg2 << endl;
    sock->send(boost::asio::buffer(msg2));
}

void SingleChatServer::getFriendRequest(const SingleChatServer::error_code &ec, SingleChatServer::sock_ptr sock, std::string msg)
{
    if(ec)
        return;
    vector<string> vec;
    cout << "收到的消息： " << msg << endl;
    boost::split(vec,msg,boost::is_any_of("^"));
    string accountNum = *(++vec.begin());
    Account act;
    Database *database = new Database();
    act.setDatabase(database);
    vector<string> frd_rqst;
    frd_rqst = act.getFriendRequest(accountNum);
    string newMsg = FRIEND_REQUEST;
    for(auto &e:frd_rqst){
        newMsg += e;
        newMsg += "#";
    }
    cout << "获取的添加好友消息：" << newMsg << endl;
    sock->send(boost::asio::buffer(newMsg));
}

void SingleChatServer::getChatRecords(const SingleChatServer::error_code &ec, SingleChatServer::sock_ptr sock, std::string msg)
{
    if(ec)
        return;
    cout << "收到的请求聊天记录信息：" << msg << endl;
    vector<string> vec;
    boost::split(vec,msg,boost::is_any_of("^"));
    string self_accountNum = vec[1];
    string frd_accountNum = vec[2];
    Database *database = new Database();
    Account account;
    account.setDatabase(database);
    ChatRecord chatRecord = account.getChatRecords(self_accountNum,frd_accountNum);
    vector<Message *> p1_msg = chatRecord.getMsgList();
    vector<Message *> p2_msg;
    //如果聊天记录为空就只发送头部
    if(0 == p1_msg.size())
    {
        string s = CHARTRECORD;
        sock->send(boost::asio::buffer(s));
        return;
    }
    else if(3 == p1_msg.size())
    {
        //将三条聊天记录转换位置
        for(int n = 2; n != -1;--n)
            p2_msg.push_back(p1_msg[n]);
    }
    else if(2 == p1_msg.size())
    {
        //将两条聊天记录转换位置
        for(int n = 1;n != -1;--n)
            p2_msg.push_back(p1_msg[n]);
    }
    else if(1 == p1_msg.size())
    {
        p2_msg.push_back(p1_msg[0]);
    }

    //聊天记录连接起来发送
    string info = CHARTRECORD;
    int i = 0;//标识每条消息的第几个元素
    for(auto &m:p2_msg)
    {
        vector<string> msgDetails = m->getMsgDetails();
        for(auto &d:msgDetails)
        {
            if(i < 2)
            {
                info += d + "^";
                ++i;
            }
            else
            {
                info += d;
            }//每一条消息的内容依次为：内容、时间、昵称
        }

        info += "#";
        i = 0;
    }
    sock->send(boost::asio::buffer(info));
}


void SingleChatServer::registerNewUser(const SingleChatServer::error_code &ec, std::string msg)
{
    cout << "Register" << endl;
    if(ec)
        return;
    vector<string> vec;
    boost::split(vec,msg,boost::is_any_of("^"));
    vec.erase(vec.begin());
    Database *database = new Database();
    Account account;
    account.setDatabase(database);
    account.registerNewUser(vec);
}
//修改好友备注处理函数
void SingleChatServer::changeRemarkHandler(const error_code &ec, std::string msg)
{
    cout << "Change friend's remark" << endl;
    if(ec)
        return;
    vector<string> vec;
    boost::split(vec,msg,boost::is_any_of("^"));
    vec.erase(vec.begin());
    Database *database = new Database();
    Account account;
    account.setDatabase(database);
    account.changeRemark(vec);
}

//创建好友分组
void SingleChatServer::addFriendGroupHandler(const error_code &ec, std::string msg)
{
    cout << "Add a new group" << endl;
    if(ec)
        return;
    vector<string> vec;
    boost::split(vec,msg,boost::is_any_of("^"));
    vec.erase(vec.begin());
    Database *database = new Database();
    Account account;
    account.setDatabase(database);
    account.addFriendGroup(vec[0],vec[1]);
}

//删除好友 用户账号、好友账号
void SingleChatServer::deleteFriendHandler(const SingleChatServer::error_code &ec, std::string msg)
{
    cout<< "Delete a friend" << endl;
    if(ec)
        return;
    vector<string> vec;
    boost::split(vec,msg,boost::is_any_of("^"));
    vec.erase(vec.begin());
    Database *database = new Database();
    Account account;
    account.setDatabase(database);
    account.deleteFriend(vec[0],vec[1]);
}

//查找用户搜索的好友
void SingleChatServer::findFriendHAndler(const SingleChatServer::error_code &ec,SingleChatServer::sock_ptr sock,std::string msg)
{
    cout << "Find a friend" << endl;
    if(ec)
        return;
    vector<string> vec;
    boost::split(vec,msg,boost::is_any_of("^"));
    vec.erase(vec.begin());
    Database *database = new Database();
    Account account;
    account.setDatabase(database);
    vector<std::string> frd_info;
    frd_info = account.findFriend(vec[0]);
    string frd_msg = FIND_FRIEND;
    for(auto &e:frd_info)
    {
        frd_msg += e;
        frd_msg += "#";
    }
    cout << "获取的好友信息" << frd_msg << endl;
    sock->send(boost::asio::buffer(frd_msg));
}

//添加好友处理函数
void SingleChatServer::addFriendHandler(const SingleChatServer::error_code &ec,std::string msg)
{
    if(ec)
        return;
    cout << "收到的消息： " << msg << endl;
    vector<string> vec;
    //自己的账号 好友的账号 自己的昵称 个人说明
    boost::split(vec,msg,boost::is_any_of("^"));
    string self_account = vec[1];
    string frd_account = vec[2];
    string self_nickName = vec[3];
    string instruction = vec[4];
    Database *database = new Database();
    Account account;
    account.setDatabase(database);
    //获取当前时间
    time_t timep;
    time(&timep);
    char tmp[64];
    strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S",localtime(&timep));
    string localtime = tmp;
    //好友不在线
    if(!friendStatus(frd_account)){
        cout << "好友不在线" << endl;
        account.addFriendRequest(self_account,frd_account,self_nickName,instruction,localtime);
    }
    //好友在线
    else {
        map<string,string>::iterator iter = m_actIp.find(frd_account);
        string atN = iter->second;
        if(iter == m_actIp.end())
        {
            cout<<"Don't found,error" << endl;

        }else {
            boost::asio::ip::udp::endpoint send_ep(boost::asio::ip::address::from_string(atN),6699);//连接端点
            boost::asio::io_service io;
            boost::asio::ip::udp::socket  u_sock(io);//创建socket对象
            u_sock.open(boost::asio::ip::udp::v4());//使用ipv4打开socket
            string s = ADD_FRIEND;
            s += self_account;
            s += "^" + self_nickName;
            s += "^" + instruction;
            s += "^" + localtime;

            u_sock.send_to(boost::asio::buffer(s),send_ep);
        }
    }
}

void SingleChatServer::addFriendFeedback(const SingleChatServer::error_code &ec, std::string msg)
{
    if(ec)
        return;
    cout << "收到的消息： " << msg << endl;
    vector<string> vec;
    boost::split(vec,msg,boost::is_any_of("^"));
    int result = atoi(vec[1].c_str());
    Database *database = new Database();
    Account account;
    account.setDatabase(database);
    //为0时表示好友同意添加，为1时表示拒绝添加
    if(0 == result){
        string self_account = vec[2];
        string frd_account = vec[3];
        string self_nickName = vec[4];
        string frd_nickName = vec[5];
        string time = vec[5];
        string content = "你们已是好友，开始聊天吧";
        account.addFriendFeedback(self_account,frd_account,self_nickName,frd_nickName);
        if(!friendStatus(frd_account)){
            cout << "好友不在线" << endl;
            account.addOutLine(self_account,frd_account,self_nickName,content);
            account.addChatRecord(self_account,frd_account,self_nickName,time,content);
        }else {
            map<string,string>::iterator iter = m_actIp.find(frd_account);
            string atN = iter->second;
            if(iter == m_actIp.end())
            {
                cout<<"Don't found,error" << endl;

            }
            else{
                boost::asio::ip::udp::endpoint send_ep(boost::asio::ip::address::from_string(atN),6699);//连接端点

                boost::asio::io_service io;
                boost::asio::ip::udp::socket  u_sock(io);//创建socket对象

                u_sock.open(boost::asio::ip::udp::v4());//使用ipv4打开socket

                //发送的数据
                string s = SEND_MESSAGE_INFO;
                s += self_nickName;
                s += "^" + self_account;
                s += "^" + content;
                s += "^" + time;

                u_sock.send_to(boost::asio::buffer(s),send_ep);//向连接端点发送数据

            }

            account.addChatRecord(self_account,frd_account,self_nickName,time,content);
        }
    }
}

void SingleChatServer::sendMsg(const SingleChatServer::error_code &ec,std::string msg)
{
    if(ec)
        return;
    cout << "进入发送" << endl;
    cout << "收到的消息为：" << msg << endl;
    vector<string> vec;
    boost::split(vec,msg,boost::is_any_of("^"));
    string self_accountNum,frd_accountNum,time,self_nickname,content;
    self_accountNum = vec[1];//自己的帐号
    frd_accountNum = vec[2];//好友的帐号
    self_nickname = vec[3];//自己的昵称
    time = vec[4];//发送时间
    content = vec[5];//发送内容

    Database *database = new Database();
    Account account;
    account.setDatabase(database);
    //好友不在线，添加离线消息以及聊天记录
    if(!friendStatus(frd_accountNum))
    {
        cout <<"好友不在线" << endl;
        account.addOutLine(self_accountNum,frd_accountNum,self_nickname,content);
        account.addChatRecord(self_accountNum,frd_accountNum,self_nickname,time,content);
    }

    //好友在线，直接发送
    else
    {

        map<string,string>::iterator iter = m_actIp.find(frd_accountNum);
        string atN = iter->second;
        if(iter == m_actIp.end())
        {
            cout<<"Don't found,error" << endl;

        }
        else{
            boost::asio::ip::udp::endpoint send_ep(boost::asio::ip::address::from_string(atN),6699);//连接端点

            boost::asio::io_service io;
            boost::asio::ip::udp::socket  u_sock(io);//创建socket对象

            u_sock.open(boost::asio::ip::udp::v4());//使用ipv4打开socket

            //发送的数据
            string s = SEND_MESSAGE_INFO;
            s += self_nickname;
            s += "^" + self_accountNum;
            s += "^" + content;
            s += "^" + time;

            u_sock.send_to(boost::asio::buffer(s),send_ep);//向连接端点发送数据

        }

        account.addChatRecord(self_accountNum,frd_accountNum,self_nickname,time,content);

    }
}


void SingleChatServer::exitHandler(const SingleChatServer::error_code &ec,std::string msg)
{
    if(ec)
        return;
    vector<string> vec;
    boost::split(vec,msg,boost::is_any_of("^"));
    string accountNum = *(++vec.begin());
    cout << "退出的帐户的帐号：" << accountNum << endl;
    map<string,Account>::iterator iter = m_account.find(accountNum);
    Account account = iter->second;
    account.setStatusOut(accountNum);
    m_actIp.erase(accountNum);
    m_account.erase(accountNum);
}

void SingleChatServer::run()
{
    m_io.run();
}

//void SingleChatServer::analysis(int endpoint_num1)
//{

//    try{
//        typedef boost::asio::ip::tcp::acceptor acceptor_type;
//        typedef boost::asio::ip::tcp::endpoint endpoint_type;
//        typedef boost::asio::ip::tcp::socket   socket_type;

//        boost::asio::io_service io;
//        acceptor_type acceptor(io,endpoint_type(boost::asio::ip::tcp::v4(),endpoint_num1));
//        socket_type sock(io);
//        string m_ip;//保存该用户的ip地址
//        acceptor.accept(sock);//建立连接
//        m_ip = sock.remote_endpoint().address().to_string();
//        Account account;//自己帐户
//        string accountN;//自己帐号

//        //处理客户端请求
//        for(;;){
//            vector<char> str(200,0);
//            sock.receive(boost::asio::buffer(str));

//            //解析字符串请求
//            string s;
//            vector<string> vec;
//            for(vector<char>::iterator it = str.begin();it != str.end();++it)
//            {
//                s += *it;
//            }
//            cout <<"the request is " << s << endl;
//            boost::split(vec,s,boost::is_any_of("^"),boost::token_compress_on);

//            vector<string>::iterator it = vec.begin();
//            string parm1 = *it;

//            //验证帐户
//            if(parm1.find(VALIDATION_INFO) == 0)
//            {
//                string password;
//                accountN = *(++it);
//                password = *(++it);
//                account = requestVd(accountN,password);
//                if(account.getNickName()!=""){
//                    string info = VALIDATION_INFO;
//                    info += "true";
//                    sock.send(boost::asio::buffer(info));
//                    mutex mu1;
//                    lock_guard<mutex> g(mu1);
//                    actIp.insert(std::pair<string,string>(accountN,m_ip));
//                }
//                else
//                {
//                    sock.send(boost::asio::buffer("false"));
//                }

//            }
//            //获取自己的帐户信息(^分隔)
//            else if(parm1.find(ACCOUNT_INFO) == 0)
//            {
//                string info = ACCOUNT_INFO;
//                info += getAccount(account);
//                sock.send(boost::asio::buffer(info));
//            }
//            //获取组信息（）
//            //            else if(parm1.find(GROUP_INFO) == 0)
//            //            {
//            //                string info;
//            //                vector<Group *> groupList;
//            //                groupList = getGroups(account);
//            //                for(auto &g:groupList)
//            //                {
//            //                    info += g->getGroupName();
//            //                }
//            //                sock.send(boost::asio::buffer(info));
//            //            }


//            else if(parm1.find(FRIEND_INFO) == 0 )
//            {
//                //获取好友信息(各个好友之间用#分隔)
//                vector<string> friends;
//                friends = getFriends(getGroups(account));
//                string info = FRIEND_INFO;
//                int i = 0;
//                for(auto &f:friends)
//                {
//                    if(i != 0)
//                        info += "#";
//                    info += f;
//                    ++i;
//                }

//                sock.send(boost::asio::buffer(info));


//            }
//            //获取离线消息
//            else if(parm1.find(OUTLINE_INFO) == 0)
//            {
//                vector<string> outlineMsg = getOutlineMsg(accountN);
//                string msg = OUTLINE_INFO;
//                for(auto &e:outlineMsg)
//                {
//                    msg += e;
//                    msg += "#";//将多条离线记录用$连接
//                }
//                cout << msg << endl;

//                sock.send(boost::asio::buffer(msg));
//            }

//            //发送消息
//            else if(parm1.find(SEND_MESSAGE_INFO) == 0)
//            {
//                string parm2,parm3,parm4;
//                parm2 = *(++it);//对方帐号
//                parm3 = *(++it);//发送时间
//                parm4 = *(++it);//发送内容

//                //好友离线，将消息加入离线表
//                if(!friendStatus(parm2))
//                {
//                    account.addOutLine(parm2,parm3,parm4);
//                    //                    sock.send(boost::asio::buffer("ok"));
//                }
//                //好友在线，将消息通过UDP发送给好友
//                else
//                {
//                    sendMsg(parm1,s);
//                }

//            }
//            //退出
//            else if(parm1.find(EXIT_INFO) == 0)
//            {
//                account.setStatusOut(accountN);//设置状态为离线

//                mutex mu3;//锁定互斥量，访问共享资源actIp
//                lock_guard<mutex> f(mu3);
//                actIp.erase(accountN);//将该帐户从在线表中删除
//                return;
//            }

//        }


//    }
//    catch(std::exception &e)
//    {
//        std::cout << e.what() << std::endl;

//    }
//}



Account SingleChatServer::requestVd(std::string accountNum, std::string password,Database *dt)
{
    Account account1;
    account1.setDatabase(dt);
    return account1.requestVd(accountNum,password);

}



bool SingleChatServer::friendStatus(std::string accountNum)
{
    Account act;
    return act.friendStatus(accountNum);
}

string SingleChatServer::getAccount(Account act)
{
    std::string info;
    info = act.getNickName() + "^" + act.getExplain();
    return info;
}

vector<Group *> SingleChatServer::getGroups(Account act)
{
    vector<Group *> g = act.getGroupList();
    return g;
}

vector<string> SingleChatServer::getFriends(vector<Group *> pGroup)
{
    vector<string> allFriends;
    for(auto &g:pGroup)
    {
        string f_info,fd_info;
        vector<Friend *> friends = g->getFriendList();
        f_info += g->getGroupName();
        if(0 != friends.size())
            f_info+= "^";
        fd_info = f_info;
        if(0 == friends.size()) {
            allFriends.push_back(fd_info);
        } else {
            Account *act;
            for(auto &f:friends)
            {
                act = f->getAccount();
                f_info += act->getAccountNum() + "^";
                f_info += act->getNickName() + "^";
                f_info += (f->getFriendRemarks() + "^");
                f_info += act->getExplain();
                allFriends.push_back(f_info);
                f_info = fd_info;
            }
        }
    }
    return allFriends;
}


std::vector<std::string> SingleChatServer::getOutlineMsg(std::string accountNum)
{
    Account act;
    Database *database = new Database();
    act.setDatabase(database);
    return act.getOutlineMsg(accountNum);
}

