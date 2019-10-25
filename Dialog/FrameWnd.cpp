
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
	//��֪��wParam, lParam��Ӧ��ʲô������˵ʵ����������Ŀ�����ڳ��ڣ�
	//��Ҳ˵��׼wParam, lParam���׶�Ӧ��ʲô�������Լ��ڶϵ������ص���Ӧ
	//��֪ͨ�󣬿�[���ö�ջ]׷��һ�°ɡ�
	CControlUI * pConntrol = (CControlUI*) notify.pSender;
	switch(notify.dwType)
	{
	case EnumNotifyMsg::ClickItem:
		{
			if(CButtonUI::g_strClassName == pConntrol->GetClassName())
			{
				//MessageBox(m_hWnd, pConntrol->GetText(), _T("�����ˣ�"), NULL);
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
				//FALSE = ȡ��ѡ��
				//TRUE = ѡ��
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
        //û�н��гɹ��ĵ�½
        this->Close(0);//�˳�
    }

    //��½�ɹ�
    pHandle->SendMessage(VTIM_SET_FRAME_HANDLE, (WPARAM)m_hWnd, (LPARAM)this);
	return 0;
}

LRESULT CFrameWnd::OnTimer(WPARAM wParam, LPARAM lParam)
{
	return 0;
}
