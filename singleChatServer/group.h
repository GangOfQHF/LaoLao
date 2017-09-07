#ifndef GROUP_H
#define GROUP_H
#include <string>
//#include "friend.h"
#include<vector>
#include "friend.h"


class Group
{
public:
    //构造函数
    Group();
    Group(std::string name);

    //get函数
    std::string getGroupName(){return m_name;}
    std::vector<Friend *> getFriendList(){return p_friend;}

    //set函数
    void setFriend(Friend *friends){p_friend.push_back(friends);}

private:
    int m_totalNum;
    int m_onlineNum;
    std::string m_name;
    std::vector<Friend *> p_friend;
};

#endif // GROUP_H
