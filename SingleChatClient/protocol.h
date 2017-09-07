#ifndef PROTOCOL_H
#define PROTOCOL_H

#define REGISTER_INFO   "r"
#define VALIDATION_INFO  "v"
#define ACCOUNT_INFO    "a"
#define GROUP_INFO      "g"
#define OUTLINE_INFO    "o"
#define FRIEND_INFO     "f"
#define SEND_MESSAGE_INFO   "s"
#define CHATRECORD          "c"
#define MESSAGE             'm'
#define EXIT_INFO           "e"
#define CREATE_GROUP        "g"
#define MODIFY_REMARKS      "k"
#define GET_USER_INFO       "n"
#define ADD_FRIEND          "i"         //请求添加别人为好友
#define ADD_FRIEND_NOTICE   'i'         //判断服务器发送的消息是否为通知消息
#define FRIEND_FEEDBACK     "b"
#define GET_NOTICE          "t"         //请求获取离线通知
#define CHANGE_GROUP      "p"           //移动好友分组
#endif // PROTOCOL_H
