
#include "LoginDlg.h"
#include "ImMessageBox.h"
#include "../vtdef.h"
using namespace MYUI; 

#define TIMER_LOGIN_TIMEOUT    0x01

CLoginDlg::CLoginDlg()
{
}

CLoginDlg::~CLoginDlg()
{
}

LPCTSTR CLoginDlg::GetUserId()
{
    return m_strUserId.GetData();
}

LPCTSTR CLoginDlg::GetPassword()
{
    return m_strPassword.GetData();
}

void CLoginDlg::OnNotify(TNOTIFYUI &notify)
{
	//想知道wParam, lParam对应了什么参数吗？说实话，由于项目还处于初期，
	//我也说不准wParam, lParam到底对应了什么参数，自己在断点中拦截到对应
	//的通知后，看[调用堆栈]追踪一下吧。
	CControlUI * pConntrol = (CControlUI*) notify.pSender;
	switch(notify.dwType)
	{
	case EnumNotifyMsg::ClickItem:
		{
			//if(CButtonUI::g_strClassName == pConntrol->GetClassName())
			//{
			//	MessageBox(m_hWnd, pConntrol->GetText(), _T("你点击了："), NULL);
			//}

            if(pConntrol->GetName() == _T("btnLogin"))
            {
                CEditUI * pEdtUserId = static_cast<CEditUI *>(m_pViewInfo->pRootControl->FindControlByName(_T("edtUserId")));
                CEditUI * pEdtPassword = static_cast<CEditUI *>(m_pViewInfo->pRootControl->FindControlByName(_T("edtPassword")));

                m_strUserId = pEdtUserId->GetText();
                m_strPassword = pEdtPassword->GetText();

                if (m_strUserId.IsEmpty())
                {
                    IMMessageBox(m_hWnd, _T("请输入你的账号"), _T("提示"), NULL);
                    return;
                }

                if (m_strPassword.IsEmpty())
                {
                    IMMessageBox(m_hWnd, _T("请输入你的密码"), _T("提示"), NULL);
                    return;
                }

                IUserHandle * pHandle = CUserHandleTable::GetThreadHandle(NULL);

                pHandle->PostMessage(VTIM_MASTER_LOGIN, 
                    (WPARAM)m_strUserId.GetData(), (LPARAM)m_strPassword.GetData());
            }
		}break;
	}

	//return __super::OnNotify(notify);
};

LRESULT CLoginDlg::OnEvent(TEVENT &event)
{
	CDragItemUI * pDragItem = NULL;
	switch (event.dwType)
	{
	case EnumEventType::WindowInit:
	{
		OnCreate(event.wParam, event.lParam);
	}break;
	case EnumEventType::WindowReady:
	{
		OnReady(event.wParam, event.lParam);
	}break;
	case EnumEventType::DragOver:
	{
		pDragItem = (CDragItemUI *)event.wParam;
		pDragItem->Destroy();
		delete pDragItem;

	}break;
	case EnumEventType::OnTimer:
	{
        OnTimer(event.wParam, event.lParam);
	}break;
	case EnumEventType::WindowDestroy:
	{
		SetSkin(NULL);
	}break;
	default:
		break;
	}

	return 0;
	//return __super::OnEvent(event);
}


LRESULT CLoginDlg::OnCreate(WPARAM wParam, LPARAM lParam)
{
	TCHAR strSkin[MAX_PATH] = {0};

	wsprintf(strSkin, _T("file='%s'"), _T("../res/skin/"));

	SetSkin(strSkin);
	//SetSkin(_T("skin\\"));

	CControlUI * pRootLayout = nullptr;

	CBuilder * pBuilder = new CBuilder(this, NULL);
	pRootLayout = pBuilder->Create(strSkin, _T("LoginDlg.xml"));
	delete pBuilder;
	this->AttachFrameView(pRootLayout);
	
	return 0;
}

LRESULT CLoginDlg::OnReady(WPARAM wParam, LPARAM lParam)
{

	return 0;
}

LRESULT CLoginDlg::OnTimer(WPARAM wParam, LPARAM lParam)
{
    IUserHandle * pHandle = NULL;
    if (TIMER_LOGIN_TIMEOUT == wParam)
    {
        pHandle = CUserHandleTable::GetThreadHandle(NULL);
    }
	return 0;
}

LRESULT CLoginDlg::LoginCall(BOOL bLogin, LPCTSTR strError)
{
    if (TRUE == bLogin)
    {
        //登陆成功
        return this->Close(bLogin);
    }
    else
    {
        //lParam = 错误信息
        IMMessageBox(m_hWnd, strError, _T("提示:"), NULL);
    }
    
    return bLogin;
}