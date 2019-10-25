
#ifndef __BUILD_MASTER_BYTES_H__
#define __BUILD_MASTER_BYTES_H__

#include <Windows.h>
#include "ChatEvent.h"

namespace BuildMasterBytes
{
    int Login(PNETBUFFER pBuffer, LPCTSTR strUserId, LPCTSTR strPassword);
    int GetUserData(PNETBUFFER pBuffer, const char * strCacheTime);
    int Logout(PNETBUFFER pBuffer);

    int FindFriend(PNETBUFFER pBuffer, LPCTSTR strFriendId);
    int AddFriend(PNETBUFFER pBuffer, LPCTSTR strFriendId);
    int AgreeAddFriend(PNETBUFFER pBuffer, LPCTSTR strFriendId, BOOL bAgree);

    int SendMsg(PNETBUFFER pBuffer, LPCTSTR strFriendId, LPCTSTR strMsg);
};

#endif