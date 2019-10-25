
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

        m_pLoginDlg->Create(this->m_hInstance, NULL, WS_OVERLAPPEDWINDOW, TEXT("vtim_login"), TEXT("��½"), NULL);
        //window->SetIcon(IDI_ICON_APP);
        m_pLoginDlg->CenterWindow();
        lResult = m_pLoginDlg->ShowModal();
        delete m_pLoginDlg;
        m_pLoginDlg = NULL;
        return lResult;
    }break;
    case VTIM_MASTER_LOGIN:
    {
        //���߻��ߴ��ڵ�½�߳�,û��Ҫ��½
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
            IMMessageBox(NULL, _T("��Ч�������ַ"), _T("��ʾ"), NULL);
        }
    }break;
    case VTIM_MASTER_LOGIN_CALL:
    {
        assert(m_hLoginThread && "CHandleProxy::Callback m_hLoginThread ��ʱӦ������Ч�ľ��");

        CloseHandle(m_hLoginThread);
        m_hLoginThread = NULL;

        //wParam : ��½�Ƿ�ɹ�
        if (m_pLoginDlg)
        {
            m_pLoginDlg->LoginCall(wParam, (LPCTSTR)lParam);
        }
        //��� m_pLoginDlg ������, ��ô���ǵ�½�������, ���������µ�½
    }break;
    case VTIM_MASTER_EVENT:
    {
        CMaterResponse::ResponseHandle((void*)lParam, wParam);
    }break;
    case VTIM_MASTER_SIGNAL:
    {
        if (IOEVENT_CLOSE == wParam)
        {
            CMasterRequest::Release();//�ر����,�ͷ��׽���
            CState::Set(ClientState::Offline);//������, ����״̬
        }
        else
        {
            CMasterRequest::CloseSocket();//�׽��ִ���, �ر�
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
