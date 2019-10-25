
#include "BuildMasterBytes.h"
#include "Jsoncpp/json.h"

#include "Encode/Encode.h"
#include <assert.h>
using namespace std;

#ifdef _UNICODE

#define tsnprintf _snwprintf
#define tvsnprintf _vsnwprintf

#else

#define tsnprintf _snprintf
#define tvsnprintf _vsnprintf

#endif

namespace BuildMasterBytes
{
    static char s_strCurrentUser[64] = { 0 };

    static int Json2Buffer(PNETBUFFER pBuffer, std::string & strText)
    {
        unsigned int size = strText.length();
        pBuffer->dwSize = htonl(size);
        assert(size >= sizeof(pBuffer->cache) && "系统约定, 通信字节数不会大于通信缓冲区!");
        memcpy(pBuffer->cache, strText.c_str(), size);

        return size + sizeof(pBuffer->dwSize);
    }

    static int SetCurrentUser(LPCTSTR strUserId)
    {
        string strUser = Encode::tchar_to_utf8(strUserId);
        
        if (strUser.length() >= sizeof(s_strCurrentUser)) return -1;

        strcpy(s_strCurrentUser, strUser.c_str());

        return strUser.length();
    }

    int Login(PNETBUFFER pBuffer, LPCTSTR strUserId, LPCTSTR strPassword)
    {
        Json::Value json;
        BuildMasterBytes::SetCurrentUser(strUserId);

        json["command"] = ChatEvent::ceLogin;
        json["userid"] = s_strCurrentUser;
        json["password"] = Encode::tchar_to_utf8(strPassword);

        return Json2Buffer(pBuffer, json.toStyledString());
    }

    int GetUserData(PNETBUFFER pBuffer,const char * strLastTime)
    {
        Json::Value json;
        json["command"] = ChatEvent::ceGetUserData;
        json["lastTime"] = strLastTime;
        json["userid"] = s_strCurrentUser;
        return Json2Buffer(pBuffer, json.toStyledString());
    }

    int Logout(PNETBUFFER pBuffer)
    {
        Json::Value json;
        json["command"] = ChatEvent::ceGetUserData;
        json["userid"] = s_strCurrentUser;
        return Json2Buffer(pBuffer, json.toStyledString());
    }

    int FindFriend(PNETBUFFER pBuffer, LPCTSTR strFriendId)
    {
        Json::Value json;
        json["command"] = ChatEvent::ceFindFriend;
        json["friendId"] = Encode::tchar_to_utf8(strFriendId);
        json["userid"] = s_strCurrentUser;
        return Json2Buffer(pBuffer, json.toStyledString());
    }

    int AddFriend(PNETBUFFER pBuffer, LPCTSTR strFriendId)
    {
        Json::Value json;
        json["command"] = ChatEvent::ceAddFriend;
        json["friendId"] = Encode::tchar_to_utf8(strFriendId);
        json["userid"] = s_strCurrentUser;
        return Json2Buffer(pBuffer, json.toStyledString());
    }

    int AgreeAddFriend(PNETBUFFER pBuffer, LPCTSTR strFriendId, BOOL bAgree)
    {
        Json::Value json;
        json["command"] = ChatEvent::ceAgreeAddFriendReque;
        json["friendId"] = Encode::tchar_to_utf8(strFriendId);
        json["agree"] = !!bAgree;
        json["userid"] = s_strCurrentUser;
        return Json2Buffer(pBuffer, json.toStyledString());
    }

    int SendMsg(PNETBUFFER pBuffer, LPCTSTR strFriendId, LPCTSTR strMsg)
    {
        TCHAR strTime[32];
        SYSTEMTIME time = { 0 };
        GetLocalTime(&time);
        tsnprintf(strTime, sizeof(strTime), TEXT("%04d-%02d-%02d %02d:%02d:%02d"),
            time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);

        Json::Value json;
        json["command"] = ChatEvent::ceSendMessage;
        json["friendId"] = Encode::tchar_to_utf8(strFriendId);
        json["message"] = Encode::tchar_to_utf8(strMsg);
        json["time"] = Encode::tchar_to_utf8(strTime);
        json["userid"] = s_strCurrentUser;

        return Json2Buffer(pBuffer, json.toStyledString());
    }
};