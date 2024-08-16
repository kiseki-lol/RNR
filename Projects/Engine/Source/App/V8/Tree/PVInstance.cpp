#include <App/V8/Tree/PVInstance.hpp>
#include <App/V8/World/World.hpp>
#include <Helpers/XML.hpp>

namespace RNR
{
    PVInstance::PVInstance() : m_cframe(), Instance()
    {

    }

    void PVInstance::deserializeProperty(char* prop_name, pugi::xml_node node)
    {
        if(prop_name == std::string("CFrame"))
        {
            setCFrame(XML::getCFrame(node));
        }
    }

    void PVInstance::addProperties(std::vector<ReflectionProperty>& properties)
    {
        ReflectionProperty _properties[]  = {
            { this, std::string("CFrame"), std::string(""), 
              ACCESS_NONE, OPERATION_READWRITE, PROPERTY_CFRAME,         
              REFLECTION_GETTER(PVInstance* instance = (PVInstance*)object; return &instance->m_cframe; ), 
              REFLECTION_SETTER(PVInstance* instance = (PVInstance*)object; instance->setCFrame(*(CoordinateFrame*)value); ) },
        };

        properties.insert(properties.end(), _properties, _properties+(sizeof(_properties)/sizeof(ReflectionProperty)));
    }

    Ogre::Vector3 PVInstance::relativePositionTo(PVInstance* point)
    {
        return point->getPosition() - getPosition();
    }
}