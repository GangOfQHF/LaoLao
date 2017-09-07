#ifndef MYITEM_H
#define MYITEM_H

#include "QQmlObjectListModel.h"
#include "QQmlAutoPropertyHelpers.h"
#include "QQmlPtrPropertyHelpers.h"

class FriendItem : public QObject
{
    Q_OBJECT
    QML_WRITABLE_AUTO_PROPERTY(QString,         groupName)
    QML_WRITABLE_AUTO_PROPERTY(QString,         account)
    QML_WRITABLE_AUTO_PROPERTY(QString,         nickName)
    QML_WRITABLE_AUTO_PROPERTY(QString,         remarksName)
    QML_WRITABLE_AUTO_PROPERTY(QString,         signup)
//    QML_WRITABLE_AUTO_PROPERTY(bool,            bSelected)

    QML_OBJMODEL_PROPERTY(FriendItem,               chs)
public:
    FriendItem(QObject* parent) : QObject(parent),m_chs(new QQmlObjectListModel<FriendItem>(this))
    {

    }
};

typedef QQmlObjectListModel<FriendItem> FriendItemModel;

#endif // MYITEM_H
