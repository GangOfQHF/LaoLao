#include "friendlist.h"
#include <iostream>

FriendList::FriendList(QObject *parent):QObject(parent)
{
    m_friendItemModel.reset(new FriendItemModel(this));
    //    m_friendItemModel = make_shared<FriendItemModel>(this);
}

//获取好友的备注名字
QString FriendList::getRemarkName(QString friendAccount)
{
    QList<FriendItem *> list = m_friendItemModel->toList();
    foreach(auto e, list)
    {
        QList<FriendItem *> groupMemberList = e->get_chs()->toList();
        foreach(auto me, groupMemberList)
        {
            if(me->get_account() == friendAccount){
                if(me->get_remarksName().isEmpty())
                    return me->get_nickName();
                else
                    return me->get_remarksName();
            }


        }
    }
    return "";
}

QString FriendList::getGroupName(QString friendAccount)
{
    QList<FriendItem *> list = m_friendItemModel->toList();
    foreach(auto e, list)
    {
        QList<FriendItem *> groupMemberList = e->get_chs()->toList();
        foreach(auto me, groupMemberList)
        {
            if(me->get_account() == friendAccount){
                return e->get_groupName();
            }

        }
    }
    return "";

}

bool FriendList::findGroup(QString groupName)
{
    QList<FriendItem *> list = m_friendItemModel->toList();
    foreach(auto e, list)
    {
        if(e->get_groupName() == groupName)
            return true;
    }
    return false;
}

void FriendList::changeDataToModel(QString friendAccount/*,QString oldGroupName*/,QString newGroupName)
{
    if(m_friendItemModel->count() > 0)
    {
        FriendItem *copyOldItem;
        int i = 0;
        QList<FriendItem *> list = m_friendItemModel->toList();
        foreach (auto e, list) {
            //            if(e->get_groupName() == oldGroupName) {
            QList<FriendItem *> groupMemberList = e->get_chs()->toList();
            foreach(auto g,groupMemberList) {
                if(g->get_account() == friendAccount){
                    copyOldItem = g;
                    i = 1;
                    break;
                }
            }
            //            }

            if(i)
                break;
        }

        foreach (auto e, list) {
            if(e->get_groupName() == newGroupName) {
                e->get_chs()->append(copyOldItem);
                break;
            }
        }

        int j = 0;
        foreach (auto e, list) {
            //            if(e->get_groupName() == oldGroupName)
            //                e->get_chs()->remove(copyOldItem);
            QList<FriendItem *> groupMemberList = e->get_chs()->toList();
            if(e->get_groupName() != newGroupName) {
                foreach(auto g,groupMemberList) {
                    if(g->get_account() == friendAccount){
                        e->get_chs()->remove(g);
                        j = 1;
                        break;
                    }
                }
            }

            if(j)
                break;
        }
    }
}

//获取好友的model指针
FriendItemModel *FriendList::getFriendItemModel() const
{
    return m_friendItemModel.data();
}



//将从服务器获取到的数据添加进模型
void FriendList::loadDataToModel(QString groupName, QString account,
                                 QString nickName, QString remarkName, QString signup)
{
    FriendItem *chsItem;
    //当模型里面有FriendItem时
    if(m_friendItemModel->count() > 0)
    {
        QList<FriendItem *> list = m_friendItemModel->toList();
        foreach(auto e, list)
        {
            if(e->get_groupName() == groupName)
            {
                chsItem = new FriendItem(e);
                chsItem->set_groupName(groupName);
                chsItem->set_account(account);
                chsItem->set_nickName(nickName);
                chsItem->set_remarksName(remarkName);
                chsItem->set_signup(signup);
                //                chsItem->set_bSelected(false);
                e->get_chs()->append(chsItem);
                return;
            }
        }
    }

    FriendItem *rootItem = new FriendItem(this); //分组的Item
    rootItem->set_groupName(groupName);
    m_friendItemModel->append(rootItem);

    if(0 != account.length()) {
        chsItem = new FriendItem(rootItem); //每个好友的Item
        chsItem->set_groupName(groupName);
        chsItem->set_account(account);
        chsItem->set_nickName(nickName);
        chsItem->set_remarksName(remarkName);
        chsItem->set_signup(signup);
        //    chsItem->set_bSelected(false);
        rootItem->get_chs()->append(chsItem);
    }
    return;
}

void FriendList::slot_loadDataToFriendList(QStringList stringList)
{
    //when the group have no member,just set the groupName
    if(1 == stringList.length()) {
        loadDataToModel(stringList.at(0),"","","","");
    } else
        loadDataToModel(stringList.at(0),stringList.at(1),
                        stringList.at(2),stringList.at(3),stringList.at(4));
}
