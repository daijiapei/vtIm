#include <WinSock2.h>
#include "MasterRequest.h"
#include "BuildMasterBytes.h"
#include "../vtdef.h"
#include "iocp/socketutils.h"
#include <process.h>
#include "Jsoncpp/json.h"

#include "Encode/Encode.h"

typedef struct __conect_struct
{
    HANDLE hEvent;
    SOCKADDR_IN server;
    TCHAR strUserId[128];
    TCHAR strPassword[128];
}CONNECTSTRUCT;

typedef struct __SocketUserData
{
    DWORD dwRecvSize;
}SOCKUSERDATA;


HIOCPBASE * CMasterRequest::m_hIocp = NULL;
SOCKADDR_IN CMasterRequest::m_server = { 0 };
HIOCPSOCKET * CMasterRequest::m_hSocket = NULL;
NETBUFFER CMasterRequest::m_buffer = { 0 };

MYUI::IUserHandle * CMasterRequest::m_pUserHandle = NULL;

CMasterRequest::CMasterRequest()
{
    //memset(m_strPassword, 0, sizeof(m_strPassword));
    //memset(m_strUserId, 0, sizeof(m_strUserId));
    //m_dwState = NULL;
    //m_pCallback = NULL;
    //memset(&m_server, 0, sizeof(m_server));
    //m_hIocp = NULL;
    //memset(&m_buffer, 0, sizeof(m_buffer));
}

CMasterRequest::~CMasterRequest()
{

}

void CMasterRequest::SetEnv(MYUI::IUserHandle * pUserHandle, HIOCPBASE * hIocp)
{
    m_hIocp = hIocp;
    m_pUserHandle = pUserHandle;
}

void CMasterRequest::CloseSocket()
{
    if (m_hSocket)
    {
        ::iocp_socket_close(m_hSocket, 0);
    }
}

void CMasterRequest::Release()
{
    if (m_hSocket)
    {
        ::iocp_socket_free(m_hSocket);
        m_hSocket = NULL;
    }
}

HANDLE CMasterRequest::Login(const SOCKADDR_IN * server, LPCTSTR strUserId, LPCTSTR strPassword)
{
    CONNECTSTRUCT * stu = (CONNECTSTRUCT *)malloc(sizeof(CONNECTSTRUCT));
    memset(stu, 0, sizeof(CONNECTSTRUCT));

    stu->hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    stu->server = *server;
    _tcscpy_s(stu->strUserId, strUserId);
    _tcscpy_s(stu->strPassword, strPassword);

    HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, CMasterRequest::login_thread, (LPVOID)stu, 0, 0);

    WaitForSingleObject(stu->hEvent, INFINITE);
    CloseHandle(stu->hEvent);
    return hThread;
}

BOOL CMasterRequest::GetUserData(char * strCacheTime)
{
    int size = BuildMasterBytes::GetUserData(&m_buffer, strCacheTime);
    return SendData(&m_buffer, size);
}

BOOL CMasterRequest::Logout()
{
    int size = BuildMasterBytes::Logout(&m_buffer);
    return SendData(&m_buffer, size);
}

BOOL CMasterRequest::FindFriend(LPCTSTR strFriendId)
{
    int size = BuildMasterBytes::FindFriend(&m_buffer, strFriendId);
    return SendData(&m_buffer, size);
}

BOOL CMasterRequest::AddFriend(LPCTSTR strFriendId)
{
    int size = BuildMasterBytes::AddFriend(&m_buffer, strFriendId);
    return SendData(&m_buffer, size);
}

BOOL CMasterRequest::AgreeAddFriend(LPCTSTR strFriendId, BOOL bArgee)
{
    int size = BuildMasterBytes::AgreeAddFriend(&m_buffer, strFriendId, bArgee);
    return SendData(&m_buffer, size);
}

BOOL CMasterRequest::SendMsg(LPCTSTR strFriendId, LPCTSTR strMsg)
{
    int size = BuildMasterBytes::SendMsg(&m_buffer, strFriendId, strMsg);
    return SendData(&m_buffer, size);
}

BOOL CMasterRequest::SendData(void * buffer, unsigned int bufsize)
{
    if (NULL == m_hSocket || 0 == bufsize) return FALSE;

    return iocp_socket_write(m_hSocket, (const char*)buffer, bufsize) == bufsize;
}

void CMasterRequest::socket_event_cb(HIOCPSOCKET * hSocket, void * userdata, DWORD dwEvent)
{
    JSONSTRING strJson;
    unsigned int size = 0;
    SOCKUSERDATA * pUserData = reinterpret_cast<SOCKUSERDATA *>(userdata);

    if (IOEVENT_READ == dwEvent)
    {
        if (pUserData->dwRecvSize > 0)
        {
            size = pUserData->dwRecvSize / 512;
            strJson = (JSONSTRING)malloc((size + 1) * 512);
            iocp_socket_read(hSocket, strJson, sizeof(pUserData->dwRecvSize));
            strJson[pUserData->dwRecvSize] = '\0';
            pUserData->dwRecvSize = size;
            CMasterRequest::m_pUserHandle->PostMessage(VTIM_MASTER_EVENT, pUserData->dwRecvSize, (LPARAM)strJson);
            iocp_socket_set_input_mark(hSocket, 4);
        }
        else
        {
            iocp_socket_read(hSocket, (char*)&size, sizeof(size));
            size = ntohl(size);
            pUserData->dwRecvSize = size;
            iocp_socket_set_input_mark(hSocket, size);
        }
    }
    
}

void CMasterRequest::socket_signal_cb(HIOCPSOCKET * hSocket, void * userdata, DWORD dwEvent, DWORD dwError)
{
    CMasterRequest::m_pUserHandle->PostMessage(VTIM_MASTER_SIGNAL, dwEvent, dwError);
}

unsigned int CMasterRequest::login_thread(void * arg)
{
    assert(NULL == m_hSocket && "CMasterRequest::login_thread m_hSocket不为空, 框架流程处理是否有冲突?");

    static TCHAR strError[256] = { 0 };
    memset(strError, 0, sizeof(strError));

    SOCKET fd = SOCKET_ERROR;
    CONNECTSTRUCT * stu = reinterpret_cast<CONNECTSTRUCT*>(arg);
    NETBUFFER buffer = { 0 };
    unsigned long lMode = 1;
    struct timeval TimeOut = { 0 };
    int size = 0;
    int ret = 0;
    int offset = 0;

    BOOL bLogin = FALSE;

    Json::Value json;
    Json::Reader reader;

    struct {
        u_int fd_count;
        SOCKET  fd_array[2];
    } FileSet = { 0 };

    SetEvent(stu->hEvent);

    //5秒连接超时
    fd = ::socket_connect(&stu->server, 3, 5);

    if (SOCKET_ERROR == fd)
    {
        _tcscpy_s(strError, _T("连接服务器失败! 请检查你的网络！"));
        goto FINISH;
    }

    if (SOCKET_ERROR == ioctlsocket(fd, FIONBIO, (unsigned long *)&lMode))
    {
        _tcscpy_s(strError, _T("套接字非阻塞属性设置失败！"));
        goto FINISH;
    }

    size = BuildMasterBytes::Login(&buffer, stu->strUserId, stu->strPassword);
    while (size > offset)
    {
        ret = send(fd, buffer.ptr + offset, size - offset, NULL);
        if (SOCKET_ERROR == ret)
        {
            _tcscpy_s(strError, _T("发送登陆请求失败！"));
            goto FINISH;
        }
        offset += ret;
    }

    //5秒登陆超时
    FD_SET(fd, &FileSet);
    TimeOut.tv_sec = 5;
    TimeOut.tv_usec = 0;

    if (0 >= ::select(0, (fd_set*)&FileSet, 0, 0, &TimeOut))
    {
        _tcscpy_s(strError, _T("登陆请求超时！"));
        goto FINISH;
    }

    Sleep(1000);//等待所有数据发送过来
    size = recv(fd, buffer.ptr, sizeof(buffer), NULL);

    buffer.dwSize = ntohl(buffer.dwSize);

    if ((buffer.dwSize + sizeof(buffer.dwSize)) != size)
    {
        _tcscpy_s(strError, _T("接受到不完整的数据！"));
        goto FINISH;
    }

    lMode = 0;
    if (SOCKET_ERROR == ioctlsocket(fd, FIONBIO, (unsigned long *)&lMode))
    {
        _tcscpy_s(strError, _T("套接字阻塞属性设置失败！"));
        goto FINISH;
    }


    if (!reader.parse(buffer.cache, json))
    {
        _tcscpy_s(strError, _T("登陆时，返回无效的数据格式！"));
        goto FINISH;
    }

    int command = json["command"].asInt();
    int success = json["state"].asBool();

    if (ChatEvent::ceLogin != command)
    {
        _tcscpy_s(strError, _T("登陆时，返回无效的数据格式！"));
        goto FINISH;
    }

    if (false == success)
    {
        std::string strText = json["error"].asString();
        _tcscpy_s(strError, Encode::utf8_to_tchar(strText.c_str()).c_str());
        goto FINISH;
    }

    bLogin = TRUE;
    _tcscpy_s(strError, _T("登陆成功！"));

FINISH:

    if (FALSE == bLogin)
    {
        closesocket(fd);
        fd = SOCKET_ERROR;
    }
    else
    {
        m_hSocket = ::iocp_socket_new(m_hIocp, fd, NULL);
        ::iocp_socket_setcb(m_hSocket, CMasterRequest::socket_event_cb, CMasterRequest::socket_signal_cb, NULL);
        ::iocp_socket_active_read(m_hSocket);
    }

    CMasterRequest::m_pUserHandle->PostMessage(VTIM_MASTER_LOGIN_CALL, bLogin, (LPARAM)strError);
    free(stu);

    return fd;
}

