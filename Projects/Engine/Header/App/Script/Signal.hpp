#pragma once
#include <lua.h>
#include <functional>
#include <App/Script/Bridge.hpp>

namespace RNR::Lua
{
    typedef std::function<int (lua_State*)> SignalFunction;

    // RNR will probably have a unified system for reflection
    // across all classes in the future, but we dont have that yet
    class Signal
    {
        std::vector<lua_State*> m_connections;
    public:
        Signal();

        int connect(lua_State* l);
        void fire(lua_State* l);        
    };

    class SignalBridge : public IBridge
    {
        static int lua_index(lua_State* l);
    public:
        virtual std::string className() { return "Signal"; }
        virtual void registerClassLibrary();
        Signal* toSignal(lua_State* l, int index);
        void fromSignal(lua_State* l, Signal* signal);
        SignalBridge() { load(); }

        static const int TAG = 0x6e;

        static SignalBridge* singleton() { return (SignalBridge*)IBridge::bridges["Signal"]; }
    };
}