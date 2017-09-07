#ifndef CHATMSGMODEL_H
#define CHATMSGMODEL_H
#include "chatmsgitem.h"


class ChatMsgModel : public QObject
{
    Q_OBJECT
public:
    ChatMsgModel(QObject *parent);
    Q_INVOKABLE void addDataToModel(QString nickName,QString msg,int direction,QDateTime time);
//    void add();
    Q_INVOKABLE ChatMsgItemModel *getChatMsgItemModel();

private:
    ChatMsgItemModel *m_chatMsgItem = new ChatMsgItemModel(this);
};

#endif // CHATMSGMODEL_H
