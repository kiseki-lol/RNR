#include <App/Script/Script.hpp>
#include <App/Script/ScriptContext.hpp>

#include <Luau/Parser.h>
#include <Luau/Bytecode.h>
#include <Luau/Error.h>
#include <lua.h>
#include <luacode.h>
#include <lualib.h>

namespace RNR::Lua
{
    Script::Script() : Instance()
    {
        setName("Script");
        m_context = NULL;
        m_content = std::string("");
        m_bytecode = 0;
        m_state = 0;
    }

    Script::Script(ScriptContext* context, std::string content) : Script()
    {
        m_context = context;
        m_content = content;
        m_bytecode = 0;

        m_context->addScript(this);
    }

    void Script::compile()
    {
        if(m_bytecode)
            free(m_bytecode);
        m_bytecode = luau_compile(m_content.c_str(), m_content.size(), NULL, &m_bytecodeSize);
        printf("Script::compile: compiled script (%i bytes)\n", m_bytecodeSize);        
    }

    void Script::load(lua_State* L)
    {        
        if(!m_state)
            throw new std::runtime_error("attempted to load script with no thread\n");
        if(!m_bytecode)
            throw new std::runtime_error("attempted to load uncompiled script\n");
        try{
            int result = luau_load(getScriptThread(), getName().c_str(), m_bytecode, m_bytecodeSize, 0);        
            if(result == 0)
                lua_resume(getScriptThread(), L, 0);
            else
                printf("Script::load: failed (%i)\n", lua_gettop(L));
        }catch(std::exception& error)
        {
            printf("Script::load: runtime error '%s'\n%s", error.what(), lua_debugtrace(L));
        }
    }

    void Script::deserializeProperty(char* prop_name, pugi::xml_node prop)
    {
        if(prop_name == std::string("Source"))
        {
            m_content = prop.text().as_string();
            if(m_context)
                m_context->addScript(this);
        }
    }

    void Script::setScriptContext(ScriptContext* context)
    {
        if(m_context)
            m_context->removeScript(this);
        m_context = context;
        context->addScript(this);
    }

    void Script::setScriptThread(lua_State* state)
    {
        m_state = state;
    }
}