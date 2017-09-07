#ifndef ACCOUNT_H
#define ACCOUNT_H
#include <string>
#include "chatrecord.h"
#include <list>
#include <vector>
#include <QDate>


class Database;
class Group;
class Account
{
public:
    Account();
    Account(std::string nickName,std::string accountNum,std::string explain);
    Account requestVd(std::string accountNum,std::string password);

    //set函数
    void setOnlineStatus(){m_status = 1;}
    void setStatus(int status){m_status = status;}
    void setDatabase(Database *database){p_database = database;}
    void setpGroup(Group* group){p_group.push_back(group);}
    void setpChatRecord(std::vector<ChatRecord *> chatRecord){p_chatRecord=chatRecord;}

    //设置离线状态
    bool setStatusOut(std::string accountNum);

    //检查对方是否在线
    bool friendStatus(std::string accountNum);

    //添加离线消息
    void addOutLine(std::string self_accountNum,std::string frd_accountNum,std::string self_nickname,std::string text);

    //添加聊天记录
    void addChatRecord(std::string self_accountNum,std::string frd_accountNum,std::string self_nickname,std::string sendTime,std::string text);
    //注册新帐户
    void registerNewUser(std::vector<std::string> vec);

    //修改好友备注
    void changeRemark(std::vector<std::string> vec);
    //创建好友分组
    void addFriendGroup(std::string accountNum,std::string groupName);

    //移动好友分组
    void changeFriendGroup(std::string accountNum, std::string friendNum,std::string  oldGroupName,std::string newGroupName);

    //删除好友
    void deleteFriend(std::string accountNum,std::string friendNum);
    //查找用户搜索的好友
    std::vector<std::string> findFriend(std::string friendNum);
    //添加好友
    void addFriendRequest(std::string frd_accountNum,std::string self_accountNum,std::string nickName,std::string instruction,std::string time);
    //添加好友反馈
    void addFriendFeedback(std::string self_accountNum,std::string frd_accountNum,std::string self_nickname,std::string frd_nickname);

    //get函数
//    std::vector<std::string> getAccount();
    std::vector<Group *> getGroupList(){return p_group;}
    ChatRecord getChatRecords(std::string pNum,std::string fNum);
    std::string getAccountNum(){return m_accountNum;}
    std::string getNickName(){return m_nickName;}
    std::string getExplain(){return  m_explain;}
    int getStatus(){return m_status;}
    std::vector<std::string> getOutlineMsg(std::string accountNum);
    std::vector<std::string> getFriendRequest(std::string accountNum);//获取好友请求


private:
    std::string m_nickName;
    std::string m_accountNum;
    std::string m_password;
    std::string m_explain;
    QDate m_birthday;
    std::string m_job;
    std::string m_address;
    std::vector<Group *> p_group;
    std::vector<ChatRecord *> p_chatRecord;
    Database *p_database;
    int m_status;

};

#endif // ACCOUNT_H
