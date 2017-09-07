#ifndef MESSAGE_H
#define MESSAGE_H
#include <string>
#include <QDate>
#include <list>

class Message
{
public:
    Message();
    Message(std::string message,std::string sendTime, std::string nickName);

    //get函数
    std::vector<std::string> getMsgDetails();

    //set函数
    bool setSendStatus();

private:
    std::string m_message;
    std::string m_sendTime;
    std::string m_nickName;
};

#endif // MESSAGE_H
