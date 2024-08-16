#include <App/Script/Signal.hpp>

namespace RNR::Lua
{
    Signal::Signal()
    {

    }

    int Signal::connect(lua_State* l)
    {
        int nargs = lua_gettop(l);
        lua_State* thread_connect = lua_newthread(l);
        m_connections.push_back(thread_connect);
    }

    void Signal::fire(lua_State* l)
    {
        for(auto connection : m_connections)
        {
            int nargs = lua_gettop(connection);
            lua_pcall(connection, nargs, 0, 0);
        }
    }

    Signal* SignalBridge::toSignal(lua_State* l, int index)
    {
        Signal* signal = *(Signal**)lua_touserdatatagged(l, index, TAG);
        return signal;
    }

    void SignalBridge::fromSignal(lua_State* l, Signal* signal)
    {
        Signal** sref = (Signal**)lua_newuserdatatagged(l, sizeof(Signal*), TAG);
        *(sref) = signal;
    }

    void SignalBridge::registerClassLibrary()
    {

    }
}