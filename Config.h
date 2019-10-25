
#ifndef __VTIM_CONFIG_H__
#define __VTIM_CONFIG_H__

#define ConfigPtr CConfig::GetInstance()

class CConfig
{
public:
    CConfig();
    ~CConfig();
    static CConfig * GetInstance();


    int LoadConfig(const char * strFileName);

    const char * GetChatServer();
    const char * GetImageServer();
    const char * GetFileServer();
private:
    static CConfig * s_hInstance;

    char m_strChatServer[64];
    char m_strImageServer[64];
    char m_strFileServer[64];
    char m_strConfigFile[320];
};

#endif