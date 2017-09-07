#include "addfriendnoticemodel.h"

AddFriendNoticeModel::AddFriendNoticeModel(QObject *parent): QObject(parent)
{

}

void AddFriendNoticeModel::addDataToModel(QString accountN, QString nickName, QString addExplain, QDateTime time)
{
    AddFriendNoticeItem *addNotice = new AddFriendNoticeItem(this);
    addNotice->set_accountNum(accountN);
    addNotice->set_nickname(nickName);
    addNotice->set_addExplain(accountN);
    addNotice->set_addExplain(addExplain);
    addNotice->set_time(time);
    m_addFriendNoticeItem->append(addNotice);
}

AddFriendNoticeItemModel *AddFriendNoticeModel::getAddFriendNoticeItemModel()
{
    return m_addFriendNoticeItem;
}
