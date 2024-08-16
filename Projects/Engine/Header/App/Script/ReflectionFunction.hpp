#include <App/Script/ReflectionProperty.hpp>
#include <lua.h>

namespace RNR
{
    enum ReflectionFunctionAccess
    {

    };

    class ReflectionFunction
    {
    private:
        std::string m_functionName;
        lua_CFunction m_functionBinding;
    public:
        ReflectionFunction(std::string name, lua_CFunction func);

        std::string name() { return m_functionName; }
        lua_CFunction function() { return m_functionBinding; }
    };
}