#ifndef ADDFRIENDNOTICE_H
#define ADDFRIENDNOTICE_H

#include "QQmlObjectListModel.h"
#include "QQmlAutoPropertyHelpers.h"
#include "QQmlPtrPropertyHelpers.h"

class AddFriendNoticeItem : public QObject
{
    Q_OBJECT
    QML_WRITABLE_AUTO_PROPERTY(QString, accountNum)
    QML_WRITABLE_AUTO_PROPERTY(QString,      nickname)
    QML_WRITABLE_AUTO_PROPERTY(QString,         addExplain)
    QML_WRITABLE_AUTO_PROPERTY(QDateTime,   time)


    //QML_WRITABLE_PTR_PROPERTY(QQmlObjectListModel<AddFriendNotice>,   chs)

    QML_OBJMODEL_PROPERTY(AddFriendNoticeItem,               chs)
public:
    AddFriendNoticeItem(QObject* parent) : QObject(parent),m_chs(new QQmlObjectListModel<AddFriendNoticeItem>(this))
    {

    }


};

typedef QQmlObjectListModel<AddFriendNoticeItem> AddFriendNoticeItemModel;
#endif // ADDFRIENDNOTICE_H
