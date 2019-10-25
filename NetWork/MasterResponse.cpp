

#include <assert.h>
#include "ChatEvent.h"
#include "MasterResponse.h"
#include <windows.h>
#include "Jsoncpp/json.h"

#include "Encode/Encode.h"

CMaterResponse::CMaterResponse()
{

}

CMaterResponse::~CMaterResponse()
{

}

ULONGPTR CMaterResponse::ResponseHandle(void * data, unsigned size)
{
    Json::Value json;
    ParseJson((char*)data, json);

    int command = json["command"].asInt();

    switch (command)
    {
    case ChatEvent::ceLogin:
    {
        return Login(json);
    }break;
    case ChatEvent::ceLogout:
    {
        return Logout(json);
    }break;
    case ChatEvent::ceGetUserInfo:
    {
        return GetUserInfo(json);
    }break;
    case ChatEvent::ceGetFriendGroup:
    {
        return GetFriendGroup(json);
    }break;
    case ChatEvent::ceGetFriendList:
    {
        return FindFriend(json);
    }break;
    case ChatEvent::ceFindFriend:
    {

    }break;
    case ChatEvent::ceAddFriendRequst:
    {

    }break;
    case ChatEvent::ceAgreeAddFriendReque:
    {

    }break;
    case ChatEvent::ceSendMessage:
    {

    }break;
    case ChatEvent::ceRecvMessage:
    {

    }break;
    default:
    {
        assert(0 && "未能识别的网络指令");
    }break;
    }

    return 0;
}


bool CMaterResponse::ParseJson(char * strJson, Json::Value & json)
{
    bool bRet = true;
    Json::Reader reader;

    if (!reader.parse(strJson, json))
    {
        assert(0 && "无效的JSON格式！");
        bRet = false;
    }
    free(strJson);
    return bRet;
}


ULONGPTR CMaterResponse::Login(Json::Value &json)
{
    bool success = json["state"].asBool();
    LPCTSTR strError = NULL;
    
    return 0;
}

ULONGPTR CMaterResponse::Logout(Json::Value &json)
{
    return 0;
}

ULONGPTR CMaterResponse::GetUserInfo(Json::Value &json)
{
    return 0;
}

ULONGPTR CMaterResponse::GetFriendGroup(Json::Value &json)
{
    return 0;
}

ULONGPTR CMaterResponse::GetFriendList(Json::Value &json)
{
    return 0;
}

ULONGPTR CMaterResponse::FindFriend(Json::Value &json)
{
    return 0;
}