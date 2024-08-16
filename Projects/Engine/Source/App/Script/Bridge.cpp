#include <App/Script/Bridge.hpp>
#include <App/V8/Tree/InstanceFactory.hpp>
#include <stdexcept>

namespace RNR::Lua
{
    std::map<std::string, IBridge*> IBridge::bridges;

    void IBridge::load()
    {
        registerClassLibrary();
        luaL_Reg sentinel;
        sentinel.name = 0;
        sentinel.func = 0;
        m_classLibrary.push_back(sentinel);
        printf("IBridge::load: loaded bridge for %s\n", className().c_str());
        bridges[className()] = this;
    }

    void IBridge::registerClassLibrary()
    {

    }

    void IBridge::addMetatable(lua_State* l)
    {
        luaL_newmetatable(l, ("MetaTable" + className()).c_str());
        for(auto reg : m_metatableLibrary)
        {
            lua_pushcfunction(l, reg.func, reg.name);
            lua_setfield(l, -2, reg.name);
        }
    }

    std::string IBridge::className()
    {
        return "IBridge";
    }

    void Vector3Bridge::registerClassLibrary()
    {
        luaL_Reg function;
        function.name = "new";
        function.func = Vector3Bridge::lua_new;
        m_classLibrary.push_back(function);
    }

    void Vector3Bridge::fromVector3(lua_State* l, Ogre::Vector3 vec)
    {
        lua_createtable(l, 0, 0);
        lua_pushstring(l, "X");
        lua_pushnumber(l, vec.x);   
        lua_settable(l, -3);

        lua_pushstring(l, "Y");
        lua_pushnumber(l, vec.y);
        lua_settable(l, -3);

        lua_pushstring(l, "Z");
        lua_pushnumber(l, vec.z);
        lua_settable(l, -3);
    }

    Ogre::Vector3 Vector3Bridge::toVector3(lua_State* l, int index)
    {
        Ogre::Vector3 vec;
        lua_pushstring(l, "X");
        lua_gettable(l, index - 1);
        vec.x = lua_tonumber(l, -1);
        lua_pop(l, 1);
        lua_pushstring(l, "Y");
        lua_gettable(l, index - 1);
        vec.y = lua_tonumber(l, -1);
        lua_pop(l, 1);
        lua_pushstring(l, "Z");
        lua_gettable(l, index - 1);
        vec.z = lua_tonumber(l, -1);
        lua_pop(l, 1);
        return vec;
    }

    int Vector3Bridge::lua_new(lua_State* l)
    {
        Vector3Bridge* bridge = (Vector3Bridge*)IBridge::bridges["Vector3"];
        bridge->fromVector3(l, Ogre::Vector3(0,0,0));
        
        return 1;
    }

    void InstanceBridge::registerClassLibrary()
    {
        luaL_Reg function;
        function.name = "new";
        function.func = InstanceBridge::lua_new;
        m_classLibrary.push_back(function);        
        
        function.name = "__index";
        function.func = InstanceBridge::lua_index;
        m_metatableLibrary.push_back(function);

        function.name = "__newindex";
        function.func = InstanceBridge::lua_new_index;
        m_metatableLibrary.push_back(function);
    }

    int InstanceBridge::lua_index(lua_State* l)
    {
        InstanceBridge* bridge = (InstanceBridge*)IBridge::bridges["Instance"];
        Instance* new_instance = bridge->toInstance(l, -2);
        std::vector<ReflectionProperty> properties = new_instance->getProperties();
        std::vector<ReflectionFunction> functions = new_instance->getFunctions();
        std::string propname = lua_tostring(l, -1);

        bool foundprop = false;
        int r = 0;
        for(auto property : properties)
        {
            if(property.name() == propname)
            {
                const void* v = property.rawGetter();
                switch(property.type())
                {
                case PROPERTY_BOOL:
                    lua_pushboolean(l, *(bool*)v);
                    break;
                case PROPERTY_INSTANCE:
                    bridge->fromInstance(l, (Instance*)v);
                    break;
                case PROPERTY_VECTOR3:
                    {
                        Vector3Bridge* v3b = (Vector3Bridge*)IBridge::bridges["Vector3Bridge"];
                        v3b->fromVector3(l, *(Ogre::Vector3*)v);
                    }
                    break;
                case PROPERTY_INTEGER:
                    lua_pushinteger(l, *(int*)v);
                    break;
                case PROPERTY_FLOAT:
                    lua_pushnumber(l, *(float*)v);
                    break;
                case PROPERTY_STD_STRING:
                    lua_pushstring(l, (*(std::string*)v).c_str());
                    break;
                }
                foundprop = true;
                r = 1;
                break;
            }
        }
        if(!foundprop) // it could be a child instance
        {
            Instance* test = new_instance->findFirstChild(propname);
            if(test)
                bridge->fromInstance(l, test);
            else
            {
                // check for a function
                for(auto function : functions)
                {
                    if(function.name() == propname)
                    {
                        lua_pushcfunction(l, function.function(), function.name().c_str());
                        r = 1;
                    }
                }
            }
        }

        return r;
    }

    int InstanceBridge::lua_new_index(lua_State* l)
    {
        InstanceBridge* bridge = (InstanceBridge*)IBridge::bridges["Instance"];
        Instance* instance = bridge->toInstance(l, -3);
        std::vector<ReflectionProperty> properties = instance->getProperties();
        std::string propname = lua_tostring(l, -2);

        for(auto property : properties)
        {
            if(property.name() == propname)
            {
                lua_Type type = (lua_Type)lua_type(l, -1);
                switch(type)
                {
                case LUA_TSTRING:
                    if(property.type() == PROPERTY_STD_STRING)
                    {
                        std::string s = lua_tostring(l, -1);
                        property.rawSetter(&s);
                    }
                    break;
                case LUA_TUSERDATA:
                    if(property.type() == PROPERTY_INSTANCE)
                    {
                        Instance* instance = bridge->toInstance(l, -1);
                        property.rawSetter(instance);
                    }
                    break;
                case LUA_TBOOLEAN:
                    if(property.type() == PROPERTY_BOOL)
                    {
                        bool b = lua_toboolean(l, -1);
                        property.rawSetter(&b);
                    }
                    break;
                case LUA_TNUMBER:
                    if(property.type() == PROPERTY_INTEGER)
                    {
                        int n = lua_tointeger(l, -1);
                        property.rawSetter(&n);
                    }
                    else if(property.type() == PROPERTY_FLOAT)
                    {
                        float f = lua_tonumber(l, -1);
                        property.rawSetter(&f);
                    }
                    break;
                case LUA_TFUNCTION:                    
                    throw std::runtime_error("Unimplemented");
                    break;
                case LUA_TTHREAD:
                    throw std::runtime_error("Unimplemented");
                    break;
                case LUA_TTABLE:
                    if(property.type() == PROPERTY_VECTOR3)
                    {
                        Vector3Bridge* v3b = Vector3Bridge::singleton();
                        Ogre::Vector3 vec = v3b->toVector3(l, -1);
                        property.rawSetter(&vec);
                    }
                    break;
                case LUA_TNIL:
                case LUA_TNONE:
                default:
                    throw std::runtime_error("Property is nil, none, or missing");
                }
            }
        }

        return 0;
    }

    int InstanceBridge::lua_new(lua_State* l)
    {        
        InstanceBridge* bridge = (InstanceBridge*)IBridge::bridges["Instance"];
        Instance* new_instance = InstanceFactory::singleton()->build(lua_tostring(l, -1));
        new_instance->addReplicate(true);
        bridge->fromInstance(l, new_instance);
        return 1;
    }

    void InstanceBridge::fromInstance(lua_State* l, Instance* instance)
    {
        Instance** iref = (Instance**)lua_newuserdatatagged(l, sizeof(Instance*), TAG);
        luaL_getmetatable(l, "MetaTableInstance");
        lua_setmetatable(l, -2);
        *(iref) = instance;
    }

    Instance* InstanceBridge::toInstance(lua_State* l, int index)
    {
        if(!luaL_checkudata(l, index, "MetaTableInstance"))
        {
            throw new std::runtime_error("toInstance called on something that doesnt have MetaTableInstance");
            return NULL;
        }

        return *(Instance**)lua_touserdatatagged(l, index, TAG);
    }
}