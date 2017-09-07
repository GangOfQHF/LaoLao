#ifndef ADDFRIENDNOTICEMODEL_H
#define ADDFRIENDNOTICEMODEL_H
#include "addfriendnoticeItem.h"

class AddFriendNoticeModel:public QObject
{
    Q_OBJECT
public:
    AddFriendNoticeModel(QObject *parent);
    Q_INVOKABLE void addDataToModel(QString nickName,QString accountN,QString addExplain,QDateTime time);

    Q_INVOKABLE AddFriendNoticeItemModel *getAddFriendNoticeItemModel();

private:
    AddFriendNoticeItemModel *m_addFriendNoticeItem = new AddFriendNoticeItemModel(this);
};

#endif // ADDFRIENDNOTICEMODEL_H
