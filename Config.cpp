
#include "Config.h"
#include "ini/inirw.h"


static void empty(void * p, int size)
{
    while (size--)
    {
        ((char*)p)[size] = '\0';
    }
}

CConfig * CConfig::s_hInstance = 0;

CConfig::CConfig()
{

}

CConfig::~CConfig()
{

}

CConfig * CConfig::GetInstance()
{
    if (0 == s_hInstance)
    {
        s_hInstance = new CConfig();
    }

    return s_hInstance;
}

int CConfig::LoadConfig(const char * strFileName)
{
    empty(this, sizeof(CConfig));

    HINI hIni = iniCreate(strFileName, 0, 0);

    if (0 == hIni) return 0;

    iniGetString(hIni, "server", "chatserver", m_strChatServer, sizeof(m_strChatServer) - 1, "");
    iniGetString(hIni, "server", "imageserver", m_strImageServer, sizeof(m_strImageServer) - 1, "");
    iniGetString(hIni, "server", "fileserver", m_strFileServer, sizeof(m_strFileServer) - 1, "");

    iniClose(hIni);

    return !!hIni;
}

const char * CConfig::GetChatServer()
{
    return m_strChatServer;
}

const char * CConfig::GetImageServer()
{
    return m_strImageServer;
}

const char * CConfig::GetFileServer()
{
    return m_strFileServer;
}