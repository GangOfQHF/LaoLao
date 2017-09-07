#ifndef SINGLECHATCLIENT_H
#define SINGLECHATCLIENT_H

#include "friendlist.h"
#include "messagemodel.h"
#include "chatmsgmodel.h"
#include "addfriendnoticemodel.h"
#include "protocol.h"
#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/system/error_code.hpp>
#include <vector>
#include <QObject>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/system/error_code.hpp>
#include <QSharedPointer>

class SingleChatClient : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString selfAccountNumber READ getSelfAccountN NOTIFY selfAccountNumberChanged)
    Q_PROPERTY(QStringList selfAccountInfo READ selfAccountInfo NOTIFY selfAccountInfoChanged)
    Q_PROPERTY(FriendList* friendList READ getFriendList)
    Q_PROPERTY(QString selectedFriendAccount READ getSelectedFriendAccount)
    //    Q_PROPERTY(int  unReadMsgCount READ getUnReadMsgCount /*NOTIFY unReadMsgCountChanged*/)
    //简化类型定义
    typedef SingleChatClient this_type;
    typedef boost::asio::ip::tcp::endpoint endpoint_type;
    typedef boost::asio::ip::address address_type;
    typedef boost::asio::ip::tcp::socket socket_type;
    typedef boost::system::error_code error_code;
    //    typedef std::vector<char> buffer_type;
public:
    //构造函数
    SingleChatClient();

    //添加的用于和服务端连接的函数
    Q_INVOKABLE void myConnect();
    //图表里面的函数
    Q_INVOKABLE bool requestVd(QString userName,QString password);
    Q_INVOKABLE void getAccount(QString userName);
    Q_INVOKABLE void getFriends(QString userName);
    Q_INVOKABLE void getOfflineMessage(QString userName);
    Q_INVOKABLE void getOffNotice(QString accountNum);//获取离线通知
    Q_INVOKABLE void getChatRecords(QString userName);
    Q_INVOKABLE void registerToDatabase(QString accountInfo);
    Q_INVOKABLE void sendMessage(QString msg);
    Q_INVOKABLE void clientExit(QString accountNum);
    Q_INVOKABLE void removeChatMsgModel();
    Q_INVOKABLE void subOfflineMsgCount();
    Q_INVOKABLE void createGroup(QString groupInfo);
    Q_INVOKABLE void moveFriendGroup(QString  groupInfo);
    Q_INVOKABLE void modifyRemarks(QString  modifyInfo);

    Q_INVOKABLE void clearSelectedFriend();
    Q_INVOKABLE void receiveMessage();
    Q_INVOKABLE void setSelectedFriend(QString friendAccount);


    //查找用户是否在好友列表中，1代表在，0代表不再
    Q_INVOKABLE int findFriend(QString accountNum);
    //获取查找的用户信息
    Q_INVOKABLE QString getUserInfo(QString accountNum);
    //获取查找到的用户的昵称
    Q_INVOKABLE QString getUserNickName(QString s);
    //获取查找到的用户的个人说明
    Q_INVOKABLE QString getUserSelfInfo(QString s);
    //发送添加请求
    Q_INVOKABLE void requestAdd(QString selfAccountNum,QString frdAccountNum,QString selfNickName,QString addExplain);
    //发送加好友反馈通知
    Q_INVOKABLE void sendAnswerNotice(QString msg,QString selfAccountN,QString frdAccount,QString selfNickName,QString frdNickName,QDateTime time);

    Q_INVOKABLE void callThread(){
        boost::thread recv([&]{receiveMessage();});
        recv.detach();
    }

    //get函数,返回的数据供qml使用
    FriendList *getFriendList() const;
    Q_INVOKABLE QString getSelfAccountN() const;
    Q_INVOKABLE QStringList selfAccountInfo() const;
    QString  getSelectedFriendAccount() const;
    MessageModel *getMsgModel() const;
    ChatMsgModel *getChatMsgModel() const;
    AddFriendNoticeModel *getAddNoticeModel() const;



    //处理重复的消息
    std::vector<std::string> my_remove(std::vector<std::string> msg);
signals:
    void sig_loadDataToFriendList(QStringList stringList);
    void selfAccountInfoChanged();
    void selfAccountNumberChanged();
    void unReadMsgCountChanged(int count);
    void sig_recvMsgsignal(QString mybuf);
    void sig_recvNotice(QString mybuf);
private slots:
    void slot_handerMsg(QString str);
    void slot_handerNoticeMsg(QString str);

private:
    boost::asio::io_service m_io;
    socket_type m_sock;
    endpoint_type m_ep;
    boost::mutex mun;

    QSharedPointer<FriendList>          m_friendList;
    QSharedPointer<MessageModel>        m_messageModel;
    QSharedPointer<ChatMsgModel>        m_chatMsgModel;
    QSharedPointer<AddFriendNoticeModel>    m_addNoticeModel;
    QString                             m_selfAccountN;
    QStringList m_self_AccountInfo; // 包括自己的昵称、帐号和个性签名
    QString                             m_selectedFriendAccount{""};
    int                                 m_unReadMessageCount = 0;
private:
    void write(const std::string &msg);    //自定义的一个向服务器发消息的函数
    QStringList convert_vector(const std::vector<std::string> &vec); //将vector<string>转化为QStringList
    void addOfflineMsgCount();
    //    vector<string> allFreinds; //存放所有好友
};


#endif // SINGLECHATCLIENT_H

