#ifndef MESSAGEITEM_H
#define MESSAGEITEM_H

#include "QQmlObjectListModel.h"
#include "QQmlAutoPropertyHelpers.h"
#include "QQmlPtrPropertyHelpers.h"

class MessageItem : public QObject
{
    Q_OBJECT
    QML_WRITABLE_AUTO_PROPERTY(QString,         nackName)
    QML_WRITABLE_AUTO_PROPERTY(QString,         friNum)
    QML_WRITABLE_AUTO_PROPERTY(QString,         message)

    QML_OBJMODEL_PROPERTY(MessageItem,               chs)
public:
    MessageItem(QObject* parent) : QObject(parent),m_chs(new QQmlObjectListModel<MessageItem>(this))
    {

    }
};
typedef QQmlObjectListModel<MessageItem> MessageItemModel;

#endif // MESSAGEITEM_H
