#include "chatrecord.h"

ChatRecord::ChatRecord()
{

}

std::vector<Message *> ChatRecord::getMsgList()
{
    return p_message;
}

