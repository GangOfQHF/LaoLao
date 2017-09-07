#include "messagemodel.h"

MessageModel::MessageModel(QObject *parent) : QObject(parent)
{
}


void MessageModel::setfrimsgNum(QString selectFriNum)
{
    m_frimsgNum = selectFriNum;
}

MessageItemModel *MessageModel::getMessageModel()
{
    return m_msgItemModel;
}

void MessageModel::removeMsg(int index)
{
    m_msgItemModel->remove(index);
}

void MessageModel::addDataToModel(QString nackName,QString friNum,QString message)
{    
    MessageItem *msgItem = new MessageItem(this);
    msgItem->set_nackName(nackName);
    msgItem->set_friNum(friNum);
    msgItem->set_message(message);
    m_msgItemModel->append(msgItem);
}

QString MessageModel::getfrimsgNum()
{
    return m_frimsgNum;
}

//void MessageModel::recordNum(QString firNum)
//{

//}
