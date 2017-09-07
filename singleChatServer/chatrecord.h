#ifndef CHATRECORD_H
#define CHATRECORD_H
#include <vector>
#include "message.h"
#include <string>


class ChatRecord
{
public:
    ChatRecord();
    void setMessage(Message *msg){p_message.push_back(msg);}
    std::vector<Message*> getMsgList();
    void addNewMsg(std::string pNum,Message message);
private:
    std::vector<Message*> p_message;
};

#endif // CHATRECORD_H
