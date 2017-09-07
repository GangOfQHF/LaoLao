#ifndef MESSAGEMODEL_H
#define MESSAGEMODEL_H

#include "messageitem.h"
#include <QObject>

class MessageModel : public QObject
{
    Q_OBJECT
public:
    MessageModel(QObject *parent);
    Q_INVOKABLE void setfrimsgNum(QString selectFriNum);
    QString getfrimsgNum();
    MessageItemModel *getMessageModel();
    Q_INVOKABLE void removeMsg(int index);
    void addDataToModel(QString nackName,QString friNum,QString message);
private:
    MessageItemModel *m_msgItemModel = new MessageItemModel(this);
    QString m_frimsgNum;
};


#endif // MESSAGEMODEL_H
