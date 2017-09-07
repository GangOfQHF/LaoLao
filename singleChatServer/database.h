#ifndef DATABASE_H
#define DATABASE_H
#include <string>
#include "chatrecord.h"
#include "account.h"


class Group;
class Friend;

class Database
{
public:
    Database();

    //功能函数
    Account validation(std::string accountNum,std::string password);
    void addNewMsg(std::string pNum,std::string fNum,Message message);
    ChatRecord getChatRecords(std::string pNum,std::string fNum);

    //帮助Account、Group、Friend关联函数
    Account relate();

    //移动好友分组
    void moveFriendGroup(std::string accountNum, std::string friendNum,std::string  oldGroupName,std::string newGroupName);


    //帮助ChatRecord、Message关联函数
    ChatRecord cm_relate();

    //检查好友是否在线
    bool friendStatus(std::string accountNum);

    //添加消息进入离线表
   void addOutLine(std::string self_accountNum,std::string frd_accountNum,std::string self_nickname,std::string text);


    //添加消息进聊天记录
   void addChatRecord(std::string self_accountNum,std::string frd_accountNum,std::string self_nickname,std::string sendTime,std::string text);

    //设置离线状态
    bool setStatusOut(std::string accountNum);

    //获取离线消息
    std::vector<std::string> getOutlineMsg(std::string accountNum);

    //注册用户到数据库
    void addUserData(std::vector<std::string> user_info);

    //修改好友备注
    void changeRemark(std::vector<std::string> vec);

    //创建好友分组
    void addFriendGroup(std::string accontNum,std::string groupName);

    //删除好友
    void deleteFriend(std::string accountNum,std::string friendNum);

    //查找用户搜索的好友
    std::vector<std::string> findFriend(std::string friendNum);

    //添加好友
    void addFriendRequest(std::string frd_accountNum,std::string self_accountNum,std::string nickName,std::string instruction,std::string time);


    //从好友请求表中获取添加好友的请求
    std::vector<std::string> getFriendRequest(std::string accountNum);

    //添加好友反馈
    void addFriendFeedback(std::string self_accountNum, std::string frd_accountNum,std::string self_nickname,std::string frd_nickname);

private:
    Account account;
    std::vector<Group > vgroup;
    std::vector<Friend> vfriend;
    std::vector<Account> vaccount;
    std::vector<Message> vmessage;
};

#endif // DATABASE_H
