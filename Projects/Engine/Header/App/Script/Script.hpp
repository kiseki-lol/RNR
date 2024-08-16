#pragma once
#include <App/V8/Tree/Instance.hpp>
#include <App/Script/Bridge.hpp>
#include <stdint.h>

#include <lua.h>

#define QUICK_GET_INSTANCE(l, i) Lua::InstanceBridge::singleton()->toInstance(l, i);

namespace RNR::Lua
{
    class ScriptContext;
    class Script : public Instance
    {
        ScriptContext* m_context;
        lua_State* m_state;
        std::string m_content;
        char* m_bytecode;
        size_t m_bytecodeSize;
        float m_resumeTime;
        bool m_paused;
    public:
        Script();
        Script(ScriptContext* context, std::string content);

        void compile();
        void load(lua_State* L);
        int pause(lua_State* L) { m_paused = true; return lua_yield(m_state, 0); }; // in Lua, this should be called at return
        int resume(lua_State* L) { m_paused = false; return lua_resume(m_state, L, 0); };
        void setResumeTime(float time) { m_resumeTime = time; };
        float getResumeTime() { return m_resumeTime; }
        void setScriptContext(ScriptContext* context);
        void setScriptThread(lua_State* state);
        bool getPaused() { return m_paused; }
        lua_State* getScriptThread() { return m_state; };
        virtual std::string getClassName() { return "Script"; }
        virtual void deserializeProperty(char* prop_name, pugi::xml_node prop);
    }; 
}