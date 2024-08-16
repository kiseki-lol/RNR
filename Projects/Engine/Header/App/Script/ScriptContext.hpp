#pragma once
#include <App/Script/Script.hpp>
#include <App/V8/Tree/Instance.hpp>
#include <App/V8/DataModel/RunService.hpp>

namespace RNR::Lua
{
    class ScriptContext : public Instance
    {
        lua_State* m_state;
        std::vector<Script*> m_scripts;
        std::vector<Script*> m_pendingScripts;
        RunService* m_runService;
    public:
        ScriptContext();
        virtual void lateInit();

        void addScript(Script* script);
        void removeScript(Script* script);
        void registerBridge(IBridge* bridge);
        void update();

        virtual std::string getClassName() { return "ScriptContext"; }
        lua_State* getInternalState() { return m_state; }
    };
}