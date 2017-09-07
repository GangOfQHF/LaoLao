#ifndef CHATMSGITEM_H
#define CHATMSGITEM_H

#include "QQmlObjectListModel.h"
#include "QQmlAutoPropertyHelpers.h"
#include "QQmlPtrPropertyHelpers.h"

class ChatMsgItem : public QObject
{
    Q_OBJECT
    QML_WRITABLE_AUTO_PROPERTY(QString,             nickname)
    QML_WRITABLE_AUTO_PROPERTY(QString,         message)
    QML_WRITABLE_AUTO_PROPERTY(int,         direction)
    QML_WRITABLE_AUTO_PROPERTY(QDateTime,   time)


    //QML_WRITABLE_PTR_PROPERTY(QQmlObjectListModel<ChatMsgItem>,   chs)

    QML_OBJMODEL_PROPERTY(ChatMsgItem,               chs)
public:
    ChatMsgItem(QObject* parent) : QObject(parent),m_chs(new QQmlObjectListModel<ChatMsgItem>(this))
    {

    }


};

typedef QQmlObjectListModel<ChatMsgItem> ChatMsgItemModel;
#endif // CHATMSGITEM_H
