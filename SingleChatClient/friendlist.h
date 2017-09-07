#ifndef FRIENDLIST_H
#define FRIENDLIST_H

#include "frienditem.h"
#include <QObject>
#include <QSharedPointer>

class FriendList : public QObject //用于管理好友模型的一个自定义类
{
    Q_OBJECT
public:
    FriendList(QObject *parent);
    FriendItemModel *getFriendItemModel() const;
    QString getRemarkName(QString friendAccount);
    Q_INVOKABLE QString getGroupName(QString friendAccount);
    Q_INVOKABLE bool findGroup(QString groupName);
    Q_INVOKABLE void loadDataToModel(QString groupName, QString account,QString nickName,QString remarkName,QString signup);
    Q_INVOKABLE void changeDataToModel(QString friendAccount/*,QString oldGroupName*/,QString newGroupName);

public slots:
    void slot_loadDataToFriendList(QStringList stringList);
private:
    //    shared_ptr<FriendItemModel> m_friendItemModel;
    QSharedPointer<FriendItemModel>     m_friendItemModel;
//    bool                                m_isFirstSelected{true};
};

#endif // FRIENDLIST_H


