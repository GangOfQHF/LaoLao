#include "chatmsgmodel.h"

ChatMsgModel::ChatMsgModel(QObject *parent) : QObject(parent)
{
}

void ChatMsgModel::addDataToModel(QString nickName, QString msg, int direction, QDateTime time)
{
    ChatMsgItem *cmi = new ChatMsgItem(this);
    cmi->set_nickname(nickName);
    cmi->set_message(msg);
    cmi->set_direction(direction);
    cmi->set_time(time);
    m_chatMsgItem->append(cmi);
}



ChatMsgItemModel *ChatMsgModel::getChatMsgItemModel()
{
    return m_chatMsgItem;
}
