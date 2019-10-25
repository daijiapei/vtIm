
#ifndef __VTIM_STATE_H__
#define __VTIM_STATE_H__

enum ClientState
{
    None,//��״̬,δ��½
    Logining,//��½��
    Online,//����
    Invisible,//����
    Offline,//����
    Logouting,//�ǳ���
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