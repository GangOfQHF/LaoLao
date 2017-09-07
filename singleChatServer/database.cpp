#include "database.h"
#include <mariadb/mysql.h>
#include <cstring>
#include <iostream>
#include <vector>
#include "friend.h"
#include "group.h"


using std::cout;
using std::endl;
using std::vector;
using std::string;


Database::Database()
{

}

Account Database::validation(string accountNum,string password)
{

    MYSQL *con = mysql_init(NULL);//初始化连接句柄
    if(!mysql_real_connect(con,"localhost","422",
                           "123","Laolao",3306,NULL,0))
    {
        cout << "Connection To MySQL failed!" << endl;
    }

    //验证并初始化Account

    const char *ac = accountNum.data();
    const char *pa = password.data();
    char sql1[100];
    std::sprintf(sql1,"SELECT * FROM Account WHERE accountNum = %s AND password = %s",ac,pa);
    size_t len1 = strlen(sql1);
    int res1 = mysql_real_query(con,sql1,len1);
    MYSQL_RES * result;
    MYSQL_ROW sql_row;
    if(!res1){
        result = mysql_store_result(con);//存储帐户的查询结果
        if(result) {
            //初始化Account
            while((sql_row = mysql_fetch_row(result))){
                Account act(sql_row[2],sql_row[0],sql_row[3]);
                account = act;
                account.setOnlineStatus();
            }

        }

        //设置数据库中帐户的在线状态
        char sql2[100];
        std::sprintf(sql2,"UPDATE Account SET status = 1 WHERE accountNum  = %s",ac);
        size_t len2 = strlen(sql2);
        int res2 = mysql_real_query(con,sql2,len2);
        if(res2){
            cout << "sql update online status failed" << endl;
        }

        //查找组
        string groupName = accountNum + "Group";
        const char *gn = groupName.data();
        char sql3[100];
        std::sprintf(sql3,"SELECT * FROM %s",gn);
        size_t len3 = strlen(sql3);
        int res3 = mysql_real_query(con,sql3,len3);
        if(!res3)
        {
            result = mysql_store_result(con);//存储组的查询结果
            if(result) {
                //初始化Group
                while((sql_row = mysql_fetch_row(result))){
                    Group group(sql_row[0]);
                    vgroup.push_back(group);
                }

            }

        }
        else
        {
            cout << "sql select group failed" << endl;
        }


        //查找好友(Friend类对应的表）
        string friendName = accountNum + "Friend";
        const char *fn = friendName.data();
        char sql4[100];
        std::sprintf(sql4,"SELECT * FROM %s",fn);
        size_t len4 = strlen(sql4);
        int res4 = mysql_real_query(con,sql4,len4);
        if(!res4)
        {
            MYSQL_RES * result1;
            result1 = mysql_store_result(con);//存储好友的查询结果
            if(result1) {
                //初始化Friend
                while((sql_row = mysql_fetch_row(result1))){
                    string fAccountNum = sql_row[1];
                    Friend friends(sql_row[2],sql_row[1],sql_row[0]);
                    vfriend.push_back(friends);

                    //查找好友帐户
                    const char *facn = fAccountNum.data();
                    char sql5[100];
                    std::sprintf(sql5,"SELECT * FROM Account WHERE accountNum=%s",facn);
                    size_t len5 = strlen(sql5);
                    int res5 = mysql_real_query(con,sql5,len5);
                    if(!res5)
                    {
                        MYSQL_RES * result2;
                        result2 = mysql_store_result(con);
                        if(result2){
                            //初始化好友Account
                            while((sql_row = mysql_fetch_row(result2))){
                                Account faccount(sql_row[2],sql_row[0],sql_row[3]);
                                int i = std::atoi(sql_row[7]);
                                faccount.setStatus(i);
                                vaccount.push_back(faccount);
                            }
                        }
                        if(result2 != NULL)
                            mysql_free_result(result2);
                    }
                    else{
                        cout << "sql select friend account failed" << endl;
                    }

                }
                if(result1 != NULL)
                    mysql_free_result(result1);

            }

        }
        else
        {
            cout << "sql select friend failed" << endl;
        }


    } else{
        cout << "sql select account failed" << endl;
    }

    if(result != NULL)
        mysql_free_result(result);

    mysql_close(con);
    return relate();


}

ChatRecord Database::getChatRecords(std::string pNum, std::string fNum)
{
    MYSQL *con = mysql_init(NULL);//初始化连接句柄
    if(!mysql_real_connect(con,"localhost","422",
                           "123","Laolao",3306,NULL,0))
    {
        cout << "Connection To MySQL failed!" << endl;
    }

    string num = pNum + "_" + fNum;
    const char *ac = num.data();
    char sql1[100];
    std::sprintf(sql1,"SELECT * FROM %s order by id desc limit 3",ac);
    size_t len1 = strlen(sql1);
    int res1 = mysql_real_query(con,sql1,len1);
    MYSQL_RES * result;
    MYSQL_ROW sql_row;
    if(!res1){
        result = mysql_store_result(con);//存储查询结果
        if(result) {
            while((sql_row = mysql_fetch_row(result))){
                Message msg(sql_row[0],sql_row[1],sql_row[2]);
                vmessage.push_back(msg);

            }

        }
    }
    else
    {
        cout << "sql select chatReord failed" << endl;
    }

    if(result != NULL)
        mysql_free_result(result);

    mysql_close(con);
    return cm_relate();
}


//建立好友分组、好友、自己帐户之间的相互关系
Account Database::relate()
{
    for(auto &f:vfriend)
    {
        for(auto &a:vaccount)
        {
            if(f.getFriendAccountNum()==a.getAccountNum())
            {
                f.setAccount(&a);
            }
        }
    }


    for(auto &g:vgroup)
    {
        for(auto &f:vfriend)
        {
            if(g.getGroupName()==f.getGroupName())
            {
                g.setFriend(&f);
            }
        }
    }





    for(auto &g:vgroup)
    {
        account.setpGroup(&g);
    }


    return account;
}

ChatRecord Database::cm_relate()
{
    ChatRecord cr;
    for(auto &m:vmessage)
    {
        cr.setMessage(&m);
    }
    return cr;
}

bool Database::friendStatus(std::string accountNum)
{
    MYSQL *con = mysql_init(NULL);//初始化连接句柄
    if(!mysql_real_connect(con,"localhost","422",
                           "123","Laolao",3306,NULL,0))
    {
        cout << "Connection To MySQL failed!" << endl;
    }

    const char *ac = accountNum.data();
    char sql1[100];
    std::sprintf(sql1,"SELECT status FROM Account WHERE accountNum = %s",ac);
    size_t len1 = strlen(sql1);
    int res1 = mysql_real_query(con,sql1,len1);
    MYSQL_RES * result;
    MYSQL_ROW sql_row;
    if(!res1){
        result = mysql_store_result(con);//存储查询结果
        if(result) {
            //初始化Account
            while((sql_row = mysql_fetch_row(result))){
                if(std::atoi(sql_row[0]) == 1)
                {
                    if(result != NULL)
                        mysql_free_result(result);
                    mysql_close(con);
                    return true;
                }
                else
                {
                    if(result != NULL)
                        mysql_free_result(result);
                    mysql_close(con);
                    return false;
                }
            }

        }
    }
    else{
        cout << "sql select the status of friend failed" << endl;
        mysql_close(con);
        return false;
    }
    mysql_close(con);
    return true;

}

void Database::addOutLine(std::string self_accountNum,std::string frd_accountNum,std::string self_nickname,std::string text)
{
    MYSQL *con = mysql_init(NULL);//初始化连接句柄
    if(!mysql_real_connect(con,"localhost","422",
                           "123","Laolao",3306,NULL,0))
    {
        cout << "Connection To MySQL failed!" << endl;
    }

    std::string out_name = frd_accountNum + "OutLine";
    const char *nm = out_name.data();//离线表名
    const char *ac = self_accountNum.data();//发送者帐号
    //    const char *st = sendTime.data();//发送时间
    const char *txt = text.data();//发送内容
    const char *sn = self_nickname.data();//发送者昵称
    //    cout << sn << endl;

    //    std::string self_chatRecord_name = self_accountNum + "_" + frd_accountNum;
    //    const char *cn = self_chatRecord_name.data();//自己的聊天记录表名

    //    std::string frd_chatRecord_name = frd_accountNum + "_" + self_accountNum;
    //    const char *fc = frd_chatRecord_name.data();//好友的聊天记录表名
    char sql1[120];
    //    char sql2[120];
    //    char sql3[120];

    std::sprintf(sql1,"Insert into %s(accountNum,content,nickname)values('%s','%s','%s')",nm,ac,txt,sn);//添加到好友的离线表中


    //    std::sprintf(sql2,"Insert into %s(text,sendtime,nickname)values('%s','%s','%s')",fc,txt,st,sn);//添加到好友聊天记录表中

    //    std::sprintf(sql3,"Insert into %s(text,sendtime,nickname)values('%s','%s','%s')",cn,txt,st,sn);//添加到自己聊天记录表中


    size_t len1 = strlen(sql1);
    //    size_t len2 = strlen(sql2);
    //    size_t len3 = strlen(sql3);
    int res1 = mysql_real_query(con,sql1,len1);
    //    int res2 = mysql_real_query(con,sql2,len2);
    //    int res3 = mysql_real_query(con,sql3,len3);

    if(res1){
        cout << "insert outline message failed" << endl;
        mysql_close(con);
        //        return false;

    }
    //    else
    //    {
    //        if(res2)
    //        {
    //            cout << "insert friend database " << fc <<  " message failed" << endl;
    //            mysql_close(con);
    //            return false;
    //        }
    //        else
    //        {
    //            if(res3)
    //            {
    //                cout << "insert my database " << cn <<  " message failed" << endl;
    //                mysql_close(con);
    //                return false;
    //            }
    //            else
    //            {
    //                mysql_close(con);
    //                return true;
    //            }
    //        }


    //    }


}
void Database::addChatRecord(std::string self_accountNum, std::string frd_accountNum, std::string self_nickname, std::string sendTime, std::string text)
{
    MYSQL *con = mysql_init(NULL);//初始化连接句柄
    if(!mysql_real_connect(con,"localhost","422",
                           "123","Laolao",3306,NULL,0))
    {
        cout << "Connection To MySQL failed!" << endl;
    }
    const char *sn = self_nickname.data();//发送者昵称
    const char *txt = text.data();//发送内容
    const char *st = sendTime.data();//发送时间

    std::string self_chatRecord_name = self_accountNum + "_" + frd_accountNum;
    const char *cn = self_chatRecord_name.data();//自己的聊天记录表名

    std::string frd_chatRecord_name = frd_accountNum + "_" + self_accountNum;
    const char *fc = frd_chatRecord_name.data();//好友的聊天记录表名
    char sql1[120];
    char sql2[120];

    std::sprintf(sql1,"Insert into %s(text,sendtime,nickname)values('%s','%s','%s')",fc,txt,st,sn);//添加到好友聊天记录表中

    std::sprintf(sql2,"Insert into %s(text,sendtime,nickname)values('%s','%s','%s')",cn,txt,st,sn);//添加到自己聊天记录表中
    size_t len1 = strlen(sql1);
    size_t len2 = strlen(sql2);
    int res1 = mysql_real_query(con,sql1,len1);
    int res2 = mysql_real_query(con,sql2,len2);
    if(res1)
    {
        cout << "insert friend database " << fc <<  " message failed" << endl;
        mysql_close(con);
    }
    else
    {
        if(res2)
        {
            cout << "insert my database " << cn <<  " message failed" << endl;
            mysql_close(con);
        }
        else
        {
            mysql_close(con);
        }
    }

}

bool Database::setStatusOut(std::string accountNum)
{
    MYSQL *con = mysql_init(NULL);//初始化连接句柄
    if(!mysql_real_connect(con,"localhost","422",
                           "123","Laolao",3306,NULL,0))
    {
        cout << "Connection To MySQL failed!" << endl;
    }
    const char *ac = accountNum.data();
    char sql2[100];
    std::sprintf(sql2,"UPDATE Account SET status = 0 WHERE accountNum  = %s",ac);
    size_t len2 = strlen(sql2);
    int res2 = mysql_real_query(con,sql2,len2);
    if(res2){
        cout << "sql update outline status failed" << endl;
        mysql_close(con);
        return false;
    }
    else
    {
        mysql_close(con);
        return true;
    }

}

std::vector<std::string> Database::getOutlineMsg(std::string accountNum)
{
    MYSQL *con = mysql_init(NULL);//初始化连接句柄
    if(!mysql_real_connect(con,"localhost","422",
                           "123","Laolao",3306,NULL,0))
    {
        cout << "Connection To MySQL failed!" << endl;
    }



    string s = accountNum + "OutLine";
    const char *ac = s.data();
    char sql1[100];
    std::sprintf(sql1,"SELECT * FROM %s",ac);
    size_t len1 = strlen(sql1);
    int res1 = mysql_real_query(con,sql1,len1);
    MYSQL_RES * result;
    MYSQL_ROW sql_row;
    vector<string> msg;
    if(!res1){
        result = mysql_store_result(con);//存储查询结果
        if(result) {
            while((sql_row = mysql_fetch_row(result))){
                cout << sql_row[0] << "," << sql_row[1] << "," << sql_row[2] << endl;
                string m = sql_row[2];
                m += "^";
                m += sql_row[0];
                m += "^";
                m += sql_row[1];
                msg.push_back(m);
            }
        }
    }
    else{
        cout << "sql select the outline message failed" << endl;
    }
    if(result != NULL)
        mysql_free_result(result);

    char sql2[100];
    std::sprintf(sql2,"DELETE FROM %s",ac);
    size_t len2 = strlen(sql2);
    int res2 = mysql_real_query(con,sql2,len2);
    if(res2)
    {
        cout << "sql delete the outline message failed" << endl;
    }
    mysql_close(con);
    return msg;
}


void Database::addUserData(std::vector<std::string> user_info)
{
    MYSQL *con = mysql_init(NULL);//初始化连接句柄
    if(!mysql_real_connect(con,"localhost","422",
                           "123","Laolao",3306,NULL,0))
    {
        cout << "Connection To MySQL failed!" << endl;
    }
    const char *userNum = user_info[0].data();
    const char *pas = user_info[1].data();
    const char *nick = user_info[2].data();
    const char *exp = user_info[3].data();
    const char *birth = user_info[4].data();
    const char *job = user_info[5].data();
    const char *addr = user_info[6].data();

    string friendname = user_info[0];
    friendname +="Friend";
    const char * fn = friendname.data();

    string groupname = user_info[0];
    groupname += "Group";
    const char *gn = groupname.data();

    string outline = user_info[0];
    outline += "OutLine";
    const char *ol = outline.data();

    string chatrecord = user_info[0];
    chatrecord += "_111111";
    const char *cr = chatrecord.data();

    string friend_request = user_info[0];
    friend_request += "Friend_Request";
    const char *frd_rqst = friend_request.data();

    //插入一个新的用户到用户表中
    char sql1[200];
    std::sprintf(sql1,"INSERT INTO Account(accountNum,password,nickName,explains,birthday,job,address) VALUES('%s','%s','%s','%s','%s','%s','%s')",userNum,pas,nick,exp,birth,job,addr);

    //创建一个好友分组
    char sql3[200];
    std::sprintf(sql3,"CREATE TABLE %s(name char(10),totalNum int,inlineNum int)",gn);


    //创建一个好友表
    char sql2[200];
    std::sprintf(sql2,"create table %s(groupname char(10),friendAccountNum char(6),remarks char(7))",fn);

    //创建一个聊天记录表
    char sql4[200];
    std::sprintf(sql4,"CREATE TABLE %s(text char(100),sendtime datetime,nickname char(7),id int not null AUTO_INCREMENT,primary key (id))",cr);

    //创建离线表
    char sql5[200];
    std::sprintf(sql5,"CREATE TABLE %s(accountNum char(6),content char(100),nickname char(7))",ol);

    //插入好友分组
    char sql6[200];
    std::sprintf(sql6,"INSERT INTO  %s(name,totalNum,inlineNum)VALUES('Robot',1,1)",gn);


    //插入一个好友
    char sql7[200];
    std::sprintf(sql7,"INSERT INTO %s(groupname,friendAccountNum,remarks)VALUES('Robot','111111','robot')",fn);

    //创建好友请求表
    char sql8[200];
    std::sprintf(sql8,"CREATE TABLE %s(accountNum char(6),nickName char(7),instruction char(30),time char(20))",frd_rqst);

    size_t len1 = strlen(sql1);
    int res1= mysql_real_query(con,sql1,len1);
    if(res1){
        cout << "sql insert into Account failed" << endl;
    }

    size_t len2 = strlen(sql2);
    int res2 = mysql_real_query(con,sql2,len2);
    if(res2){
        cout << "sql create group failed" << endl;
    }


    size_t len3 = strlen(sql3);
    int res3 = mysql_real_query(con,sql3,len3);
    if(res3)
    {
        cout << "sql create friend failed" << endl;
    }

    size_t len4 = strlen(sql4);
    int res4 = mysql_real_query(con,sql4,len4);
    if(res4)
    {
        cout << "sql create chatrecord failed" << endl;
    }

    size_t len5 = strlen(sql5);
    int res5 = mysql_real_query(con,sql5,len5);
    if(res5)
    {
        cout << "sql create outline failed" << endl;
    }

    size_t len6 = strlen(sql6);
    int res6 = mysql_real_query(con,sql6,len6);
    if(res6)
    {
        cout << "sql insert into group failed" << endl;
    }

    size_t len7 = strlen(sql7);
    int res7 = mysql_real_query(con,sql7,len7);
    if(res7)
    {
        cout << "sql insert into friend failed" << endl;
    }

    size_t len8 = strlen(sql8);
    int res8= mysql_real_query(con,sql8,len8);
    if(res8){
        cout << "sql create friend_request failed" << endl;
    }

    mysql_close(con);
}

//修改好友备注
void Database::changeRemark(std::vector<std::string> vec)
{
    MYSQL *con = mysql_init(NULL);//初始化连接句柄
    if(!mysql_real_connect(con,"localhost","422",
                           "123","Laolao",3306,NULL,0))
    {
        cout << "Connection To MySQL failed!" << endl;
    }
    std::string self_friendList = vec[0] + "Friend";
    const char *frdList = self_friendList.data();
    const char *frd_Num = vec[1].data();
    const char *remark = vec[2].data();
    char sql[100];
    std::sprintf(sql,"UPDATE %s SET remarks = '%s' WHERE friendAccountNum = %s",frdList,remark,frd_Num);
    size_t len = strlen(sql);
    int res = mysql_real_query(con,sql,len);
    if(res) {
        cout << "sql update friend's remark failed" << endl;
        mysql_close(con);
        return;
    }else {
        mysql_close(con);
        return;
    }
}

void Database::addFriendGroup(std::string accontNum, std::string groupName)
{
    MYSQL *con = mysql_init(NULL);//初始化连接句柄
    if(!mysql_real_connect(con,"localhost","422",
                           "123","Laolao",3306,NULL,0))
    {
        cout << "Connection To MySQL failed!" << endl;
    }
    std::string self_groupList = accontNum + "Group";
    const char *grpList = self_groupList.data();
    const char *grpName = groupName.data();
    char sql[100];
    std::sprintf(sql,"INSERT INTO %s(name,totalNum,inlineNum) VALUES('%s',0,0)",grpList,grpName);
    size_t len = strlen(sql);
    int res = mysql_real_query(con,sql,len);
    if(res) {
        cout << "sql add group failed" << endl;
        mysql_close(con);
        return;
    }else {
        mysql_close(con);
        return;
    }
}

//删除好友
void Database::deleteFriend(std::string accountNum, std::string friendNum)
{
    MYSQL *con = mysql_init(NULL);//初始化连接句柄
    if(!mysql_real_connect(con,"localhost","422",
                           "123","Laolao",3306,NULL,0))
    {
        cout << "Connection To MySQL failed!" << endl;
    }
    //删除用户好友表中的好友
    std::string str = accountNum + "Friend";
    const char *tableNm = str.data();
    const char *frd = friendNum.data();
    char sql[100];
    std::sprintf(sql,"DELETE FROM %s WHERE friendAccountNum = %s",tableNm,frd);
    size_t len = strlen(sql);
    mysql_real_query(con,sql,len);
    //删除好友的好友表中的用户
    std::string str1 = friendNum + "Friend";
    const char *tableNm1 = str1.data();
    const char *frd1 = accountNum.data();
    char sql1[100];
    std::sprintf(sql1,"DELETE FROM %s WHERE friendAccountNum = %s",tableNm1,frd1);
    size_t len1 = strlen(sql1);
    mysql_real_query(con,sql1,len1);
    //删除好友与用户的聊天记录表
    std::string self_table = accountNum + "_" + friendNum;
    const char *self_cd = self_table.data();
    std::string frd_table = friendNum + "_" + accountNum;
    const char *frd_cd = frd_table.data();
    char sql2[100];
    char sql3[100];
    std::sprintf(sql2,"DROP TABLE %s",self_cd);
    std::sprintf(sql3,"DROP TABLE %s",frd_cd);
    size_t len2 = strlen(sql2);
    mysql_real_query(con,sql1,len2);
    size_t len3 = strlen(sql3);
    int res = mysql_real_query(con,sql2,len3);
    if(res){
        cout << "Delete friend failed" << endl;
        mysql_close(con);
        return;
    }else {
        mysql_close(con);
        return;
    }
}

//查找用户搜索的好友
vector<std::string> Database::findFriend(std::string friendNum)
{
    MYSQL *con = mysql_init(NULL);//初始化连接句柄
    if(!mysql_real_connect(con,"localhost","422",
                           "123","Laolao",3306,NULL,0))
    {
        cout << "Connection To MySQL failed!" << endl;
    }
    const char *frd = friendNum.data();
    char sql[100];
    std::sprintf(sql,"SELECT * FROM Account WHERE accountNum = %s",frd);
    size_t len = strlen(sql);
    int res = mysql_real_query(con,sql,len);
    MYSQL_RES * result;
    MYSQL_ROW sql_row;
    vector<std::string> vec;
    if(!res){
        result = mysql_store_result(con);
        if(result){
            while ((sql_row = mysql_fetch_row(result))) {
                string m = sql_row[2];
                m += "^";
                m += sql_row[3];
                vec.push_back(m);
            }
        }
        mysql_close(con);
        return vec;
    }else {
        vec.push_back("The user account dose not exist!^");
        mysql_close(con);
        return vec;
    }
}

void Database::addFriendRequest(std::string frd_accountNum, std::string self_accountNum, std::string nickName, std::string instruction,std::string time)
{
    MYSQL *con = mysql_init(NULL);//初始化连接句柄
    if(!mysql_real_connect(con,"localhost","422",
                           "123","Laolao",3306,NULL,0))
    {
        cout << "Connection To MySQL failed!" << endl;
    }
    std::string name = self_accountNum + "Friend_Request";
    const char *tbl = name.data();
    const char *acnt = frd_accountNum.data();
    const char *nick = nickName.data();
    const char *ins = instruction.data();
    const char *tm = time.data();
    char sql[200];
    std::sprintf(sql,"INSERT INTO %s(accountNum,nickName,instruction,time) VALUE('%s','%s','%s','%s')",tbl,acnt,nick,ins,tm);

    size_t len = strlen(sql);
    int res = mysql_real_query(con,sql,len);
    if(res){
        cout << "insert friend request failed" << endl;
    }
    mysql_close(con);
}

std::vector<std::string> Database::getFriendRequest(std::string accountNum)
{
    MYSQL *con = mysql_init(NULL);//初始化连接句柄
    if(!mysql_real_connect(con,"localhost","422",
                           "123","Laolao",3306,NULL,0))
    {
        cout << "Connection To MySQL failed!" << endl;
    }
    string s = accountNum + "Friend_Request";
    const char *tbl = s.data();
    char sql[100];
    std::sprintf(sql,"SELECT * FROM %s",tbl);
    size_t len = strlen(sql);
    int res = mysql_real_query(con,sql,len);
    MYSQL_RES * result;
    MYSQL_ROW sql_row;
    vector<string> msg;
    if(!res){
        result = mysql_store_result(con);//存储查询结果
        if(result) {
            while((sql_row = mysql_fetch_row(result))){
                string m = sql_row[0];
                m += "^";
                m += sql_row[1];
                m += "^";
                m += sql_row[2];
                m += "^";
                m += sql_row[3];
                msg.push_back(m);
            }
        }
    }
    else {
        cout << "sql select the friend request failed" << endl;
    }
    if(result != NULL)
        mysql_free_result(result);
    char sql1[100];
    std::sprintf(sql1,"DELETE FROM %s",tbl);
    size_t len1 = strlen(sql1);
    int res1 = mysql_real_query(con,sql1,len1);
    if(res1){
        cout << "sql delete the friend request failed" << endl;
    }
    mysql_close(con);
    return msg;
}

void Database::addFriendFeedback(std::string self_accountNum, std::string frd_accountNum,std::string self_nickname,std::string frd_nickname)
{
    MYSQL *con = mysql_init(NULL);//初始化连接句柄
    if(!mysql_real_connect(con,"localhost","422",
                           "123","Laolao",3306,NULL,0))
    {
        cout << "Connection To MySQL failed!" << endl;
    }
    string str = frd_accountNum + "Friend";
    const char *frd_frd = str.data();
    string str1 = self_accountNum + "Friend";
    const char *self_frd = str1.data();
    string str2 = self_accountNum + "_" + frd_accountNum;
    const char *self_chat = str2.data();
    string str3 = frd_accountNum + "_" + self_accountNum;
    const char *frd_chat = str3.data();
    const char *frd_act = frd_accountNum.data();
    const char *self_act = self_accountNum.data();
    const char *frd_nick = frd_nickname.data();
    const char *self_nick = self_nickname.data();
    //在好友的好友表中插入自己
    char sql[200];
    std::sprintf(sql,"INSERT INTO %s(groupname,friendAccountNum,remarks) VALUE('Robot','%s','%s')",frd_frd,self_act,self_nick);
    size_t len = strlen(sql);
    int res = mysql_real_query(con,sql,len);
    //在自己的好友表中插入好友
    char sql1[200];
    std::sprintf(sql1,"INSERT INTO %s(groupname,friendAccountNum,remarks) VALUE('Robot','%s','%s')",self_frd,frd_act,frd_nick);
    size_t len1 = strlen(sql1);
    int res1 = mysql_real_query(con,sql1,len1);
    //创建自己的聊天记录表
    char sql2[200];
    std::sprintf(sql2,"CREATE TABLE %s(text char(100),sendtime datetime,nickname char(7),id int not null AUTO_INCREMENT,primary key (id))",self_chat);
    size_t len2 = strlen(sql2);
    int res2 = mysql_real_query(con,sql2,len2);
    //创建好友的聊天记录表
    char sql3[200];
    std::sprintf(sql3,"CREATE TABLE %s(text char(100),sendtime datetime,nickname char(7),id int not null AUTO_INCREMENT,primary key (id))",frd_chat);
    size_t len3 = strlen(sql3);
    int res3 = mysql_real_query(con,sql3,len3);
    if(res){
        cout << "sql insert into friend's friend table failed" << endl;
    }
    if(res1){
        cout << "sql insert into self friend table failed" << endl;
    }
    if(res2){
        cout << "sql create self chatrecord failed" << endl;
    }
    if(res3){
        cout << "sql create friend chatrecord failed" << endl;
    }
    mysql_close(con);
}

void Database::moveFriendGroup(std::string accountNum, std::string friendNum,std::string  oldGroupName,std::string newGroupName)
{
    MYSQL *con = mysql_init(NULL);//初始化连接句柄
    if(!mysql_real_connect(con,"localhost","422",
                           "123","Laolao",3306,NULL,0))
    {
        cout << "Connection To MySQL failed!" << endl;
    }
    string selfFriendTableName = accountNum + "Friend";
    string selfGroupTableName = accountNum + "Group";

    const char *friendN = friendNum.data();
    const char *newGroup = newGroupName.data();
    const char *friendTableName = selfFriendTableName.data();
    const char *groupTableName = selfGroupTableName.data();
    const char *oldGroup = oldGroupName.data();

    char sql[100];
    //修改账户的好友表里面的分组名
    std::sprintf(sql,"UPDATE %s SET groupname='%s' WHERE friendAccountNum=%s",
                 friendTableName,newGroup,friendN);
    size_t len = strlen(sql);
    int res = mysql_real_query(con,sql,len);
    if(res) {
        cout << "move friendGroup failed" << endl;
    }

    //将原来分组的总人数、在线人数减1
    std::sprintf(sql,"UPDATE %s SET totalNum=totalNum-1,inlineNum=inlineNum-1 WHERE name='%s'",
                 groupTableName,oldGroup);
    size_t len4 = strlen(sql);
    int res4 = mysql_real_query(con,sql,len4);
    if(res4) {
        cout << "update oldGroup failed" << endl;
    }

    //将现在分组的总人数、在线人数加1[错误提示：%s要加单引号]
    std::sprintf(sql,"UPDATE %s SET totalNum=totalNum+1,inlineNum=inlineNum+1 WHERE name='%s'",
                 groupTableName,newGroup);
    size_t len5 = strlen(sql);
    int res5 = mysql_real_query(con,sql,len5);
    if(res5) {
        cout << "update newGroup failed" << endl;
    }

    mysql_close(con);

}
