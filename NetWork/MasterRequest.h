
#ifndef __MASTER_SOCKET_H__
#define __MASTER_SOCKET_H__

#include "iocp/iocpsocket.h"
#include "iocp/iocpbase.h"
#include "iocp/iocptimer.h"
#include "BuildMasterBytes.h"
#include "../HandleProxy.h"


class CMasterRequest
{
private:
    CMasterRequest();
    ~CMasterRequest();

public:

    static void SetEnv(MYUI::IUserHandle * pUserHandle, HIOCPBASE * hIocp);
    static void CloseSocket();
    static void Release();

    static HANDLE Login(const SOCKADDR_IN * server, LPCTSTR strUserId, LPCTSTR strPassword);
    static BOOL GetUserData(char * strCacheTime);
    static BOOL Logout();

    static BOOL FindFriend(LPCTSTR strFriendId);
    static BOOL AddFriend(LPCTSTR strFriendId);
    static BOOL AgreeAddFriend(LPCTSTR strFriendId, BOOL bArgee);
    
    static BOOL SendMsg(LPCTSTR strFriendId, LPCTSTR strMsg);
    
public:
    

private:
    static BOOL SendData(void * buffer, unsigned int bufsize);
    
    static void socket_event_cb(HIOCPSOCKET * hSocket, void * userdata, DWORD dwEvent);
    static void socket_signal_cb(HIOCPSOCKET * hSocket, void * userdata , DWORD dwEvent, DWORD dwError);
    static unsigned int __stdcall login_thread(void * arg);
private:

    static HIOCPBASE * m_hIocp;
    static SOCKADDR_IN m_server;
    static HIOCPSOCKET * m_hSocket;
    static NETBUFFER m_buffer;

    static MYUI::IUserHandle * m_pUserHandle;
};

#endif