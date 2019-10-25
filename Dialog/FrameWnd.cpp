
#include "FrameWnd.h"
#include "LoginDlg.h"
#include "../vtdef.h"
using namespace MYUI; 

CFrameWnd::CFrameWnd()

{
}

CFrameWnd::~CFrameWnd()
{
}


void CFrameWnd::OnNotify(TNOTIFYUI &notify)
{
	//想知道wParam, lParam对应了什么参数吗？说实话，由于项目还处于初期，
	//我也说不准wParam, lParam到底对应了什么参数，自己在断点中拦截到对应
	//的通知后，看[调用堆栈]追踪一下吧。
	CControlUI * pConntrol = (CControlUI*) notify.pSender;
	switch(notify.dwType)
	{
	case EnumNotifyMsg::ClickItem:
		{
			if(CButtonUI::g_strClassName == pConntrol->GetClassName())
			{
				//MessageBox(m_hWnd, pConntrol->GetText(), _T("你点击了："), NULL);
			}
		}break;
	case EnumNotifyMsg::ActiveItem:
		{
			pConntrol = (CControlUI*) notify.lParam;
			MessageBox(m_hWnd, pConntrol->GetClassName(), pConntrol->GetText(), NULL);
		}break;
	case EnumNotifyMsg::TimerCall:
		{
			pConntrol->KillTimer(10);
			MessageBox(m_hWnd, pConntrol->GetClassName(), pConntrol->GetName(), NULL);
		}break;
	case EnumNotifyMsg::CheckItem:
		{
			CControlUI * pShow = NULL;
			if(FALSE == notify.wParam)
			{
				//FALSE = 取消选中
				//TRUE = 选中
				break;
			}

		}break;
	}

	//return __super::OnNotify(notify);
};

LRESULT CFrameWnd::OnEvent(TEVENT &event)
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

		}break;
	case EnumEventType::WindowDestroy:
		{
			SetSkin(NULL);
			PostQuitMessage(0);
		}break;
	default:
		break;
	}

	return 0;
	//return __super::OnEvent(event);
}


LRESULT CFrameWnd::OnCreate(WPARAM wParam, LPARAM lParam)
{
	TCHAR strSkin[MAX_PATH] = {0};

	wsprintf(strSkin, _T("file='%s'"), _T("../res/skin/"));

	SetSkin(strSkin);
	//SetSkin(_T("skin\\"));

	CControlUI * pRootLayout = nullptr;

	CBuilder * pBuilder = new CBuilder(this, NULL);
	pRootLayout = pBuilder->Create(strSkin, _T("FrameWnd.xml"));
	delete pBuilder;
	this->AttachFrameView(pRootLayout);
	
	return 0;
}

LRESULT CFrameWnd::OnReady(WPARAM wParam, LPARAM lParam)
{
    BOOL bRet = FALSE;
    IUserHandle * pHandle = CUserHandleTable::GetThreadHandle(NULL);
    if (FALSE == pHandle->SendMessage(VTIM_SHOW_LOGIN))
    {
        //没有进行成功的登陆
        this->Close(0);//退出
    }

    //登陆成功
    pHandle->SendMessage(VTIM_SET_FRAME_HANDLE, (WPARAM)m_hWnd, (LPARAM)this);
	return 0;
}

LRESULT CFrameWnd::OnTimer(WPARAM wParam, LPARAM lParam)
{
	return 0;
}
