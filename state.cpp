
#include <Windows.h>
#include "state.h"

long CState::s_state = (long)ClientState::None;

ClientState CState::Get()
{
    return (ClientState)InterlockedExchangeAdd(&CState::s_state, 0);
}

ClientState CState::Set(ClientState state)
{
    return (ClientState)InterlockedExchange(&CState::s_state, (long)state);
}
