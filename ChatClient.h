
#ifndef __VTIM_CHATCLIENT_H__
#define __VTIM_CHATCLIENT_H__

#include "iocp/iocpsocket.h"
#include "iocp/iocpbase.h"
#include "iocp/iocptimer.h"
#include "http/httputils.h"

class CChatClient 
{
public:
    
public:
    CChatClient();
    ~CChatClient();


    const char * GetServerIp();
    int GetServerPort();

    BOOL Send(const char * data,unsigned int size);

    BOOL CallUi(UINT message, WPARAM wParam, LPARAM lParam);
private:
    int m_nNextMsgSize;
    DWORD m_hUiThreadId;

};


#endif