

#ifndef __IM_FRAME_WND_H__
#define __IM_FRAME_WND_H__

#include <myui.h>

class CFrameWnd : public MYUI::CWindowUI
{
public:
    CFrameWnd();
    ~CFrameWnd();

    void OnNotify(MYUI::TNOTIFYUI &notify);
	LRESULT OnEvent(MYUI::TEVENT &event);

private:

    LRESULT OnCreate(WPARAM wParam, LPARAM lParam);
	LRESULT OnReady(WPARAM wParam, LPARAM lParam);

	LRESULT OnTimer(WPARAM wParam, LPARAM lParam);

private:


};


#endif