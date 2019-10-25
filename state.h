
#ifndef __VTIM_STATE_H__
#define __VTIM_STATE_H__

enum ClientState
{
    None,//无状态,未登陆
    Logining,//登陆中
    Online,//在线
    Invisible,//隐身
    Offline,//离线
    Logouting,//登出中
};

class CState
{
    
public:
    static ClientState Get();
    static ClientState Set(ClientState state);

private:
    CState() { 0; };
    ~CState() { 0; };

    static long s_state;
};

#endif