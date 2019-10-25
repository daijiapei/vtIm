
#include "HandleProxy.H"
#include <tchar.h>
#include "NetWork/MasterRequest.h"
#include "NetWork/MasterResponse.h"
#include "Dialog/LoginDlg.h"
#include "Dialog/ImMessageBox.h"
#include "Config.h"
#include "state.h"
#include "iocp/socketutils.h"

using namespace MYUI;

CHandleProxy::CHandleProxy()
    : m_hIocp(NULL)
    , m_pChatClient(NULL)
    , m_pFrameWnd(NULL)
    , m_pLoginDlg(NULL)
    , m_hLoginThread(NULL)
{

}

CHandleProxy::~CHandleProxy()
{
}

void CHandleProxy::OnAttach()
{
    m_hIocp = iocp_base_new();
    iocp_base_dispatch_in_thread(m_hIocp);
    CState::Set(ClientState::None);
    CMasterRequest::SetEnv(this, m_hIocp);
}

void CHandleProxy::OnDetach()
{
    if (m_hLoginThread)
    {
        WaitForSingleObject(m_hLoginThread, INFINITE);
        CloseHandle(m_hLoginThread);
        m_hLoginThread = NULL;
    }
    iocp_base_loopexit(m_hIocp);
    iocp_base_free(m_hIocp);
    m_hIocp = NULL;
}

LRESULT CHandleProxy::Callback(UINT message, WPARAM wParam , LPARAM lParam)
{
    LRESULT lResult = 0;
    SOCKADDR_IN addr = { 0 };

    switch (message)
    {
    case VTIM_SET_FRAME_HANDLE:
    {
        m_pFrameWnd = reinterpret_cast<CFrameWnd *>(lParam);
    }break;
    case VTIM_SHOW_LOGIN:
    {
        m_pLoginDlg = new CLoginDlg();

        m_pLoginDlg->Create(this->m_hInstance, NULL, WS_OVERLAPPEDWINDOW, TEXT("vtim_login"), TEXT("登陆"), NULL);
        //window->SetIcon(IDI_ICON_APP);
        m_pLoginDlg->CenterWindow();
        lResult = m_pLoginDlg->ShowModal();
        delete m_pLoginDlg;
        m_pLoginDlg = NULL;
        return lResult;
    }break;
    case VTIM_MASTER_LOGIN:
    {
        //在线或者存在登陆线程,没必要登陆
        if (ClientState::Online == CState::Get() || m_hLoginThread)
        {
            return 0;
        }
        
        if (socket_addr(&addr, ConfigPtr->GetChatServer()))
        {
            m_hLoginThread = CMasterRequest::Login(&addr, (LPCTSTR)wParam, (LPCTSTR)lParam);
        }
        else
        {
            IMMessageBox(NULL, _T("无效的网络地址"), _T("提示"), NULL);
        }
    }break;
    case VTIM_MASTER_LOGIN_CALL:
    {
        assert(m_hLoginThread && "CHandleProxy::Callback m_hLoginThread 此时应该是有效的句柄");

        CloseHandle(m_hLoginThread);
        m_hLoginThread = NULL;

        //wParam : 登陆是否成功
        if (m_pLoginDlg)
        {
            m_pLoginDlg->LoginCall(wParam, (LPCTSTR)lParam);
        }
        //如果 m_pLoginDlg 不存在, 那么就是登陆后断线了, 现在在重新登陆
    }break;
    case VTIM_MASTER_EVENT:
    {
        CMaterResponse::ResponseHandle((void*)lParam, wParam);
    }break;
    case VTIM_MASTER_SIGNAL:
    {
        if (IOEVENT_CLOSE == wParam)
        {
            CMasterRequest::Release();//关闭完成,释放套接字
            CState::Set(ClientState::Offline);//断线了, 离线状态
        }
        else
        {
            CMasterRequest::CloseSocket();//套接字错误, 关闭
        }
    }break;
    default:
        break;
    }

    return lResult;
}

LRESULT CHandleProxy::OnMasterCall(WPARAM wParam, LPARAM lParam)
{


    return TRUE;
}
