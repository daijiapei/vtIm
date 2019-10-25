
#ifndef __HANDLE_PROXY_H__
#define __HANDLE_PROXY_H__

#include "iocp/iocpsocket.h"
#include "iocp/iocpbase.h"
#include "iocp/iocptimer.h"
#include "Dialog/FrameWnd.h"
#include "Dialog/LoginDlg.h"
#include "ChatClient.h"
#include "vtdef.h"
typedef struct _user_info
{
    TCHAR strUserId[64];
    TCHAR strUserName[64];
    TCHAR strPassword[64];
    DWORD dwStatus;
}USERINFO, * PUSERINFO;


//这个类主要是用来同步socket与UI的业务逻辑的, 它并不显示窗口
class CHandleProxy : public MYUI::IUserHandle

{
public:
    CHandleProxy();
    ~CHandleProxy();

    
protected:
    virtual LRESULT Callback(UINT message, WPARAM wParam , LPARAM lParam);

    //BOOL ChatHandle(int nMsgSize, CIocpClient * pClient);
    virtual void OnAttach();
	virtual void OnDetach();


private:
    LRESULT OnMasterCall(WPARAM wParam, LPARAM lParam);

private:
    
    HIOCPBASE * m_hIocp;
    CChatClient * m_pChatClient;
    CFrameWnd * m_pFrameWnd;
    CLoginDlg * m_pLoginDlg;
    HANDLE m_hLoginThread;
};


#endif