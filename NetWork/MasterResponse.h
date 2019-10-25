
#ifndef __VTIM_MASTER_RESPONSE_H__
#define __VTIM_MASTER_RESPONSE_H__

#include "Jsoncpp/json.h"

typedef unsigned long *  ULONGPTR;

class CMaterResponse
{
public:
    static ULONGPTR ResponseHandle(void * data, unsigned size);

private:
    CMaterResponse();
    ~CMaterResponse();

    static bool ParseJson(char * strJson, Json::Value & json);

    //连接与登陆都在一条线程内同时完成了,并不会收到这条消息
    static ULONGPTR Login(Json::Value &json);
    static ULONGPTR Logout(Json::Value &json);
    static ULONGPTR GetUserInfo(Json::Value &json);
    static ULONGPTR GetFriendGroup(Json::Value &json);
    static ULONGPTR GetFriendList(Json::Value &json);
    static ULONGPTR FindFriend(Json::Value &json);
};

#endif