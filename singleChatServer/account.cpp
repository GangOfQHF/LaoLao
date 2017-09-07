#include "account.h"
#include "group.h"
#include "database.h"
Account::Account()
{

}

Account::Account(std::string nickName,std::string accountNum, std::string explain):m_nickName(nickName),m_accountNum(accountNum),m_explain(explain)
{

}

Account Account::requestVd(std::string accountNum, std::string password)
{
    return p_database->validation(accountNum,password);
}

bool Account::setStatusOut(std::string accountNum)
{
    return  p_database->setStatusOut(accountNum);
}

bool Account::friendStatus(std::string accountNum)
{
   return p_database->friendStatus(accountNum);
}

void Account::addOutLine(std::string self_accountNum,std::string frd_accountNum,std::string self_nickname, std::string text)
{
    p_database->addOutLine( self_accountNum,frd_accountNum,self_nickname,text);
}


void Account::changeFriendGroup(std::string accountNum, std::string friendNum,std::string  oldGroupName,std::string newGroupName)
{
    p_database->moveFriendGroup(accountNum,friendNum,oldGroupName,newGroupName);
}

void Account::addChatRecord(std::string self_accountNum, std::string frd_accountNum, std::string self_nickname, std::string sendTime, std::string text)
{
    p_database->addChatRecord(self_accountNum,frd_accountNum,self_nickname,sendTime,text);
}

void Account::registerNewUser(std::vector<std::string> vec)
{
    p_database->addUserData(vec);
}

//修改好友备注
void Account::changeRemark(std::vector<std::string> vec)
{
    p_database->changeRemark(vec);
}

//创建好友分组
void Account::addFriendGroup(std::string accountNum, std::string groupName)
{
    p_database->addFriendGroup(accountNum,groupName);
}

//删除好友
void Account::deleteFriend(std::string accountNum, std::string friendNum)
{
    p_database->deleteFriend(accountNum,friendNum);
}

//查找用户搜索的好友
std::vector<std::string> Account::findFriend(std::string friendNum)
{
    return p_database->findFriend(friendNum);;
}

//添加好友
void Account::addFriendRequest(std::string frd_accountNum,std::string self_accountNum,std::string nickName, std::string instruction,std::string time)
{
    p_database->addFriendRequest(frd_accountNum,self_accountNum,nickName,instruction,time);
}

void Account::addFriendFeedback(std::string self_accountNum, std::string frd_accountNum,std::string self_nickname,std::string frd_nickname)
{
    p_database->addFriendFeedback(self_accountNum,frd_accountNum,self_nickname,frd_nickname);
}

ChatRecord Account::getChatRecords(std::string pNum, std::string fNum)
{
    return p_database->getChatRecords(pNum,fNum);
}

std::vector<std::string> Account::getOutlineMsg(std::string accountNum)
{
    return p_database->getOutlineMsg(accountNum);
}

//获取好友请求
std::vector<std::string> Account::getFriendRequest(std::string accountNum)
{
    return  p_database->getFriendRequest(accountNum);
}

