#pragma once 

#include <string>
#include <boost/type_index.hpp>
#include <functional>

#define REFLECTION_GETTER(c) \
    [](const void* object) { c }
#define REFLECTION_NO_GETTER() \
    [](const void* object) { return (const void*)NULL; }
#define REFLECTION_SETTER(c) \
    [](void* object, const void* value) { c }
#define REFLECTION_NO_SETTER() \
    [](void* object, const void* value) { }

namespace RNR
{
    enum ReflectionPropertyAccess
    {
        ACCESS_NONE,
        ACCESS_PLUGIN,
        ACCESS_LOCALUSER,
        ACCESS_WRITEPLAYER,
        ACCESS_AUTHORIZEDSCRIPT,
        ACCESS_AUTHORIZED,
    };

    enum ReflectionPropertyOperation
    {
        OPERATION_READ,
        OPERATION_READWRITE,
    };

    enum ReflectionPropertyType
    {
        PROPERTY_BOOL,
        PROPERTY_STD_STRING,
        PROPERTY_INTEGER,
        PROPERTY_VECTOR2,
        PROPERTY_VECTOR3,
        PROPERTY_CFRAME,
        PROPERTY_INSTANCE,
        PROPERTY_FLOAT,
        PROPERTY_BRICKCOLOR,
        PROPERTY_EVENT,
    };

    class ReflectionProperty
    {
    private:
        const void* m_object;
        std::string m_name;
        std::string m_description;
        ReflectionPropertyAccess m_access;
        ReflectionPropertyOperation m_op;
        ReflectionPropertyType m_type;
        std::function<const void* (const void* /* owner */)> m_getter;
        std::function<void (void* /* owner */, const void* /* value */)> m_setter;
    public:
        // this == m_object
        // getter returns an address to the value,
        // setter sets the value to the contents of the address
        ReflectionProperty(const void* m_object,
                           std::string name, 
                           std::string description, 
                           ReflectionPropertyAccess access, 
                           ReflectionPropertyOperation op,
                           ReflectionPropertyType m_type,
                           std::function<const void* (const void* /* owner */)> getter,
                           std::function<void (void* /* owner */, const void* /* value */)> setter);

        bool access(ReflectionPropertyAccess accessor) { return this->m_access <= accessor; }
        std::string name() { return m_name; }
        std::string description() { return m_description; }
        ReflectionPropertyType type() { return m_type; }
        ReflectionPropertyAccess accessLevel() { return m_access; }
        ReflectionPropertyOperation op() { return m_op; }
        const void* object() { return m_object; }

        const void* rawGetter() { return m_getter(m_object); }
        void rawSetter(const void* value) { return m_setter((void*)m_object, value); }

        template<typename T>
        void setterA(T value) { m_setter((void*)m_object, (const void*)&value); }
        template<typename T>
        T getterA() { return *(T*)m_getter(m_object); }

        std::string toString();
    };
}