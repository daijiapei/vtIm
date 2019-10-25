
#include <Windows.h>

#ifdef _DEBUG
#pragma comment(lib, "../Debug/myui.lib")
#else
#pragma comment(lib, "../Release/myui.lib")
#endif
#pragma comment(lib,"ws2_32.lib")

#include "HandleProxy.h"
#include "iocp/iocpdef.h"
#include "Dialog/LoginDlg.h"
#include "Config.h"

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
					LPSTR szCmdLine, int iCmdShow)
{
    int nRet = 0;
	TCHAR strFloder[MAX_PATH];
    GetModuleFileName(NULL, strFloder, MAX_PATH);
    *(_tcsrchr(strFloder, '\\') + 1) = '\0';
    SetCurrentDirectory(strFloder);

    iocp_init_env();

	CoInitialize(NULL);
	//_Module.Init( 0, hInstance );

    ConfigPtr->LoadConfig("vtim.ini");

    CHandleProxy * pHandleProxy = new CHandleProxy();
    MYUI::CUserHandleTable::InitHandleTable();
    MYUI::CUserHandleTable::AddHandle(dynamic_cast<MYUI::IUserHandle *>(pHandleProxy));

	CFrameWnd * window = new CFrameWnd();
	window->Create(hInstance,NULL,WS_OVERLAPPEDWINDOW , TEXT("vtIM"), TEXT("HelloWin"), NULL);
	//window->SetIcon(IDI_ICON_APP);
	window->CenterWindow();
	nRet = window->ShowModal(true);
	delete window;

    MYUI::CUserHandleTable::ClearHandleTable();
    MYUI::CUserHandleTable::FreeHandleTable();
	//_Module.Term();
	CoUninitialize();

    iocp_uninit_env();

    return nRet;
}