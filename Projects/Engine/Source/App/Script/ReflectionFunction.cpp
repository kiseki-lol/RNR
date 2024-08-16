#include <App/Script/ReflectionFunction.hpp>

namespace RNR
{
    ReflectionFunction::ReflectionFunction(std::string name, lua_CFunction function)
    {
        this->m_functionName = name;
        this->m_functionBinding = function;
    }
}