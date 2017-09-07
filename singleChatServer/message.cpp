#include "message.h"

Message::Message()
{

}

Message::Message(std::string message, std::string sendTime, std::string nickName):m_message(message),m_sendTime(sendTime),m_nickName(nickName                                                                                                                                 )
{

}

std::vector<std::string> Message::getMsgDetails()
{
    std::vector<std::string> msg;
    msg.push_back(m_message);
    msg.push_back(m_sendTime);
    msg.push_back(m_nickName);
    return msg;
}
