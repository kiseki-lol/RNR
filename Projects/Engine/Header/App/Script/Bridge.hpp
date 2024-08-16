#pragma once
#include <string>
#include <vector>
#include <lua.h>
#include <lualib.h>
#include <OGRE/Ogre.h>
#include <App/V8/Tree/Instance.hpp>

namespace RNR::Lua
{
    // TODO: rewrite this system in a potentional v2.0.0?

    class IBridge
    {
    protected:
        std::vector<luaL_Reg> m_classLibrary;
        std::vector<luaL_Reg> m_metatableLibrary;
        virtual void registerClassLibrary();
    public:
        static std::map<std::string, IBridge*> bridges;
    
        void load();
        virtual std::string className();
        luaL_Reg* classLibrary() { return m_classLibrary.data(); };

        void addMetatable(lua_State* l);
    };

    class Vector3Bridge : public IBridge
    {
    private: // lua methods
        static int lua_new(lua_State* l);
    protected:
        virtual void registerClassLibrary();
    public:
        void fromVector3(lua_State* l, Ogre::Vector3 vec);
        Ogre::Vector3 toVector3(lua_State* l, int index);
        Vector3Bridge() { load(); }
        virtual std::string className() { return "Vector3"; }

        static Vector3Bridge* singleton() { return (Vector3Bridge*)IBridge::bridges["Vector3"]; }
    };

    class InstanceBridge : public IBridge
    {
    private:
        static int lua_new(lua_State* l);
        static int lua_index(lua_State* l);    
        static int lua_new_index(lua_State* l);
    protected:
        virtual void registerClassLibrary();
    public:
        void fromInstance(lua_State* l, Instance* instance);
        Instance* toInstance(lua_State* l, int index);
        InstanceBridge() { load(); }
        virtual std::string className() { return "Instance"; }

        static const int TAG = 0x1a;

        static InstanceBridge* singleton() { return (InstanceBridge*)IBridge::bridges["Instance"]; }
    };
}