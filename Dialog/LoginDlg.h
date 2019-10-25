
#ifndef __IM_LOGIN_DLG_H__
#define __IM_LOGIN_DLG_H__

#include <myui.h>

#define WMU_LOGINCALL  (WM_APP + 1)

class CLoginDlg : public MYUI::CWindowUI
{
public:
    CLoginDlg();
    ~CLoginDlg();

    void OnNotify(MYUI::TNOTIFYUI &notify);
	LRESULT OnEvent(MYUI::TEVENT &event);

    LPCTSTR GetUserId();
    LPCTSTR GetPassword();
    LRESULT LoginCall(BOOL bLogin, LPCTSTR strError);
private:
    
    LRESULT OnCreate(WPARAM wParam, LPARAM lParam);
	LRESULT OnReady(WPARAM wParam, LPARAM lParam);

	LRESULT OnTimer(WPARAM wParam, LPARAM lParam);


private:
    MYUI::CMuiString m_strUserId;
    MYUI::CMuiString m_strPassword;
};

#endif