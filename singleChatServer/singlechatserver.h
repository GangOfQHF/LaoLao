#ifndef SINGLECHATSERVER_H
#define SINGLECHATSERVER_H
#include <string>
#include <vector>
#include "group.h"
#include <QDate>
#include "message.h"
#include <boost/system/error_code.hpp>
#include <boost/thread.hpp>
#include <map>

#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>

class SingleChatServer
{
    //新的添加
    typedef SingleChatServer               this_type;
    typedef boost::asio::ip::tcp::acceptor acceptor_type;
    typedef boost::asio::ip::tcp::endpoint endpoint_type;
    typedef boost::asio::ip::tcp::socket   socket_type;
    typedef std::shared_ptr<socket_type>   sock_ptr;
    typedef boost::system::error_code      error_code;
    typedef std::vector<char>               buffer_type;
public:
    SingleChatServer();
    //新的添加
    void accept();
    void acceptHandler(const error_code &ec, sock_ptr sock);
    void readHandler(const error_code &ec, sock_ptr sock);
    void validationHandler(const error_code &ec,sock_ptr sock,std::string msg);
    void getAccountHandler(const error_code &ec,sock_ptr sock,std::string msg);
    void getFriendHandler(const error_code &ec,sock_ptr sock,std::string msg);
    void getOutlineMsgHandler(const error_code &ec,sock_ptr sock,std::string msg);
    void getFriendRequest(const error_code &ec,sock_ptr sock,std::string msg);//获取好友请求处理函数
    void getChatRecords(const error_code &ec,sock_ptr sock,std::string msg);
    void registerNewUser(const error_code &ec,std::string msg);
    void sendMsg(const error_code &ec,std::string msg);
    void exitHandler(const error_code &ec,std::string msg);
    void run();
    void changeRemarkHandler(const error_code &ec,std::string msg);//修改好友备注处理函数
    void addFriendGroupHandler(const error_code &ec,std::string msg);//创建分组处理函数
    void deleteFriendHandler(const error_code &ec,std::string msg);//删除好友处理函数
    void findFriendHAndler(const error_code &ec,sock_ptr sock,std::string msg);//查找用户搜索的好友并返回好友昵称、个性说明信息

    void addFriendHandler(const error_code &ec,std::string msg);//添加好友处理函数
    void changeGroupHandler(const error_code &ec,std::string msg);//移动好友分组处理函数

    void addFriendFeedback(const error_code &ec,std::string msg);//添加好友反馈信息

    //    void analysis(int endpoint_num1);
    Account requestVd(std::string accountNum,std::string password,Database *dt);

    //set函数
    void setStatus();
    void setSendStatus(Message message);

    //发送消息


    //检查对方是否在线
    bool friendStatus(std::string accountNum);

    //get函数
    std::string getAccount(Account act);
    std::vector<Group*> getGroups(Account act);
    std::vector<std::string> getFriends(std::vector<Group*> pGroup);
    std::vector<std::string> getOutlineMsg(std::string accountNum);

    void addNewMsg(std::string pNUm,std::string fNum);

private:
    //    boost::condition_variable_any cond_rece;
    //    boost::condition_variable_any cond_send;
    std::map<std::string,std::string> m_actIp;//在线帐户的表（帐号和ip地址的对应）

    std::map<std::string,Account> m_account;//所有登录的帐户的集合

    boost::asio::io_service       m_io;
    buffer_type                   m_buff;
    acceptor_type                 m_acceptor;
};

#endif // SINGLECHATSERVER_H
