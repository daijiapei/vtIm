
#include "ChatClient.h"
#include "HandleProxy.h"


CChatClient::CChatClient()
    : m_hUiThreadId(NULL)
    , m_nNextMsgSize(NULL)
{
    //����ཫ���ڶ�Ӧ�Ĵ��ڴ���
    m_hUiThreadId = GetCurrentThreadId();
}

CChatClient::~CChatClient()
{
}



const char * CChatClient::GetServerIp()
{
    //return "192.168.48.136";
    return "127.0.0.1";
}

int CChatClient::GetServerPort()
{
    return 9995;
}

BOOL CChatClient::Send(const char * data,unsigned int size)
{
    return FALSE;
}

BOOL CChatClient::CallUi(UINT message, WPARAM wParam, LPARAM lParam)
{
    MYUI::IUserHandle * pHandle =  MYUI::CUserHandleTable::GetThreadHandle(m_hUiThreadId);

    if(pHandle)
    {
        return pHandle->PostMessage(message, wParam, lParam);
    }

    return FALSE;
}