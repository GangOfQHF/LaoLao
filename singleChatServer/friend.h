#ifndef FRIEND_H
#define FRIEND_H
#include <vector>
#include <string>
#include "account.h"

class Friend
{
public:
    Friend();
    Friend(std::string remarks,std::string accountNum,std::string groupName);

    //get函数
    std::string getFriendRemarks(){return m_remarks;}
    Account *getAccount(){return p_account;}
    std::string getFriendAccountNum(){return m_accountNum;}
    std::string getGroupName(){return m_groupName;}

    //set函数
    void setAccount(Account *account){p_account = account;}

private:
    std::string m_remarks;
    Account *p_account;
    std::string m_accountNum;
    std::string m_groupName;
};

#endif // FRIEND_H
