#include <iostream>
#include <mysql/mysql.h>
using std::cout;
using std::endl;

bool createConnection()
{
    MYSQL *con = mysql_init(NULL);
    if(!mysql_real_connect(con,"localhost","laolao",
                           "laolao","Laolao",3306,NULL,0))
    {
        cout << "Connection To MySQL failed!" << endl;
        return false;
    }
    mysql_query(con,"DROP TABLE Account;");
    mysql_query(con,"DROP TABLE account;");
    mysql_query(con,"CREATE TABLE Account("
                    "accountNum char(6) NOT NULL PRIMARY KEY,"
                    "password varchar(13) NOT NULL,"
                    "nickname varchar(7),"
                    "explains varchar(50),"
                    "birthday date,"
                    "job varchar(10),"
                    "address varchar(20),"
                    "status int(11) NOT NULL);");
    mysql_query(con,"INSERT INTO Account(accountNum,password,nickname,explains,birthday,"
                    "job,address,status) VALUES('111111','123456','robot','我是机器人',"
                    "'1991-01-01','work','china','1');");
    return true;
}

int main()
{
    if(createConnection()) {
        cout << "Create table Account successfully" << endl;
    } else
        cout << "Create table false" << endl;
    return 0;
}
