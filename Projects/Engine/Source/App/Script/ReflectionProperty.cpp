#include <App/Script/ReflectionProperty.hpp>
#include <App/V8/Tree/Instance.hpp>
#include <App/CoordinateFrame.hpp>
#include <App/BrickColor.hpp>
#include <Helpers/Strings.hpp>

namespace RNR
{
    ReflectionProperty::ReflectionProperty(const void* object,
                                           std::string name, 
                                           std::string description, 
                                           ReflectionPropertyAccess access, 
                                           ReflectionPropertyOperation op,
                                           ReflectionPropertyType type,
                                           std::function<const void* (const void* /* owner */)> getter,
                                           std::function<void (void* /* owner */, const void* /* value */)> setter)
    {
        this->m_object = object;
        this->m_name = name;
        this->m_access = access;
        this->m_op = op;
        this->m_type = type;
        this->m_getter = getter;
        this->m_setter = setter;
    }

    std::string ReflectionProperty::toString()
    {
        if(m_getter(m_object) == 0)
            return std::string("NULL");
        switch(m_type)
        {
            case PROPERTY_STD_STRING:
                return *(std::string*)m_getter(m_object);
            case PROPERTY_BOOL:
                return std::string((*(bool*)m_getter(m_object))?"true":"false");
            case PROPERTY_INSTANCE:
                {
                    Instance* instance = (Instance*)m_getter(m_object);
                    return instance->getName();
                }
            case PROPERTY_VECTOR3:
                {
                    Ogre::Vector3 vector = *(Ogre::Vector3*)m_getter(m_object);
                    return Strings::string_format("(%f,%f,%f)", 
                        vector.x,
                        vector.y,
                        vector.z);
                }
            case PROPERTY_FLOAT:
                return Strings::string_format("%f",
                       *(float*)m_getter(m_object));
            case PROPERTY_CFRAME:
                {
                    CoordinateFrame cframe = *(CoordinateFrame*)m_getter(m_object);
                    Ogre::Vector3 cframe_position = cframe.getPosition();
                    Ogre::Quaternion cframe_rotation = Ogre::Quaternion(cframe.getRotation());
                    return Strings::string_format("(%f,%f,%f), (%f,%f,%f,%f)", 
                        cframe_position.x,
                        cframe_position.y,
                        cframe_position.z,
                        cframe_rotation.x,
                        cframe_rotation.y,
                        cframe_rotation.z,
                        cframe_rotation.w);
                }
            case PROPERTY_BRICKCOLOR:
                return std::string(BrickColor::name(*(int*)m_getter(m_object)));
            default:
                return std::string("???");
        }
    }
}