#include <App/V8/DataModel/Lighting.hpp>
#include <App/V8/World/World.hpp>
#include <Helpers/XML.hpp>

namespace RNR
{
    Lighting::Lighting()
    {
        setName("Lighting");
        setClearColor(Ogre::Vector3(255.f/255.f,255.f/255.f,255.f/255.f));
        setShadowColor(Ogre::Vector3(127.f/255.f,127.f/255.f,127.f/255.f));
        setBottomAmbient(Ogre::Vector3(122.f/255.f,134.f/255.f,120.f/255.f));
        setTopAmbient(Ogre::Vector3(209.f/255.f,208.f/255.f,217.f/255.f));
        setSpotLight(Ogre::Vector3(191.f/255.f,191.f/255.f,191.f/255.f));
        m_timeOfDay = "14:00:00";
        m_geographicLatitude = 41.7332993f;
    }

    void Lighting::lateInit()
    {
        Ogre::BillboardSet* sunAndMoonSet = world->getOgreSceneManager()->createBillboardSet("sun&moon");
        sunAndMoonSet->getMaterial()->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
        sunAndMoonSet->getMaterial()->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
        m_sun = sunAndMoonSet->createBillboard(Ogre::Vector3(0,0,0));
        m_sun->setDimensions(1,1);
        m_sunNode = world->getOgreSceneManager()->createSceneNode();
        m_sunNode->attachObject(sunAndMoonSet);
        m_sunNode->setInheritOrientation(false);
        m_sunNode->setInheritScale(false);
        world->getOgreSceneManager()->getRootSceneNode()->addChild(m_sunNode);
        Ogre::Light* l = world->getOgreSceneManager()->getLight("SunLight");
        m_sunOrigin = Ogre::Vector3(0,0,0);

        updateSunPosition();
    }

    void Lighting::updateSunPosition()
    {
        int hours = std::stoi(m_timeOfDay.substr(0,2));
        int minutes = std::stoi(m_timeOfDay.substr(3,2));
        int seconds = std::stoi(m_timeOfDay.substr(6,2));

        float time = ((hours * 60 * 60) + (minutes * 60) + seconds) / (float)(12 * 60 * 60);
        float sun_offset = ((4 * 60 * 60)) / (float)(12 * 60 * 60); // this is the time that the sun will reach y 0 at
        float sun_time = time - sun_offset; // adjust for sun time
        float sun_cycle = sun_time * M_PI;
        float sunY = sinf(sun_cycle);
        float sunX = sinf(sun_cycle - (m_geographicLatitude * (M_PI / 180.f)));
        float sunZ = cosf(sun_cycle - (m_geographicLatitude * (M_PI / 180.f)));

        m_sunBrightness = std::max(0.f,sunY);
        m_sun->setColour(Ogre::ColourValue(m_sunBrightness,m_sunBrightness,m_sunBrightness,m_sunBrightness));

        float sundist = 5.f;
        Ogre::Light* l = world->getOgreSceneManager()->getLight("SunLight");
        l->getParentSceneNode()->setPosition(sunX,sunY,sunZ);
        l->getParentSceneNode()->lookAt(Ogre::Vector3(0,0,0), Ogre::Node::TS_PARENT);
        m_sunNode->setPosition(m_sunOrigin.x + sunX*sundist,m_sunOrigin.y + sunY*sundist,m_sunOrigin.z + sunZ*sundist);
    }

    void Lighting::setSunOrigin(Ogre::Vector3 vec)
    {
        m_sunOrigin = vec;
        updateSunPosition();
    }

    void Lighting::setMinutesAfterMidnight(int minutes)
    {
        int hours = (minutes / 60) % 24;
        int minutes2 = minutes % 60;
        char time[9];
        snprintf(time,9,"%02i:%02i:00", hours, minutes2);
        m_timeOfDay = std::string(time);
        updateSunPosition();
    }

    void Lighting::deserializeProperty(char* prop_name, pugi::xml_node node)
    {
        if(prop_name == std::string("ClearColor"))
        {
            setClearColor(XML::getColor(node));
        }
        else if(prop_name == std::string("ShadowColor"))
        {
            setShadowColor(XML::getColor(node));
        }
        else if(prop_name == std::string("SpotLightV9"))
        {
            setSpotLight(XML::getColor(node));
        }
        else if(prop_name == std::string("BottomAmbientV9") || prop_name == "ColorShift_Bottom")
        {
            setBottomAmbient(XML::getColor(node));
        }
        else if(prop_name == std::string("TopAmbientV9") || prop_name == "ColorShift_Top")
        {
            setTopAmbient(XML::getColor(node));
        }
        else if(prop_name == std::string("GeographicLatitude"))
        {
            setGeographicLatitude(node.text().as_float());
        }
        else if(prop_name == std::string("TimeOfDay"))
        {
            setTimeOfDay(node.text().as_string());
        }
    }

    void Lighting::addProperties(std::vector<ReflectionProperty>& properties)
    {
        ReflectionProperty _properties[]  = {
            { this, std::string("BottomAmbientV9"), std::string(""), 
              ACCESS_NONE, OPERATION_READWRITE, PROPERTY_VECTOR3,         
              REFLECTION_GETTER(Lighting* instance = (Lighting*)object; return &instance->m_bottomAmbient; ), 
              REFLECTION_SETTER(Lighting* instance = (Lighting*)object; instance->setBottomAmbient(*(Ogre::Vector3*)value); ) },
            { this, std::string("TopAmbientV9"), std::string(""), 
              ACCESS_NONE, OPERATION_READWRITE, PROPERTY_VECTOR3,         
              REFLECTION_GETTER(Lighting* instance = (Lighting*)object; return &instance->m_topAmbient; ), 
              REFLECTION_SETTER(Lighting* instance = (Lighting*)object; instance->setTopAmbient(*(Ogre::Vector3*)value); ) },
            { this, std::string("ClearColor"), std::string(""), 
              ACCESS_NONE, OPERATION_READWRITE, PROPERTY_VECTOR3,         
              REFLECTION_GETTER(Lighting* instance = (Lighting*)object; return &instance->m_clearColor; ), 
              REFLECTION_SETTER(Lighting* instance = (Lighting*)object; instance->setClearColor(*(Ogre::Vector3*)value); ) },
            { this, std::string("ShadowColor"), std::string(""), 
              ACCESS_NONE, OPERATION_READWRITE, PROPERTY_VECTOR3,         
              REFLECTION_GETTER(Lighting* instance = (Lighting*)object; return &instance->m_shadowColor; ), 
              REFLECTION_SETTER(Lighting* instance = (Lighting*)object; instance->setShadowColor(*(Ogre::Vector3*)value); ) },
            { this, std::string("SpotLightV9"), std::string(""), 
              ACCESS_NONE, OPERATION_READWRITE, PROPERTY_VECTOR3,         
              REFLECTION_GETTER(Lighting* instance = (Lighting*)object; return &instance->m_spotLight; ), 
              REFLECTION_SETTER(Lighting* instance = (Lighting*)object; instance->setSpotLight(*(Ogre::Vector3*)value); ) },
            { this, std::string("GeographicLatitude"), std::string(""), 
              ACCESS_NONE, OPERATION_READWRITE, PROPERTY_FLOAT,         
              REFLECTION_GETTER(Lighting* instance = (Lighting*)object; return &instance->m_geographicLatitude; ), 
              REFLECTION_SETTER(Lighting* instance = (Lighting*)object; instance->setGeographicLatitude(*(float*)value); ) },
            { this, std::string("TimeOfDay"), std::string(""), 
              ACCESS_NONE, OPERATION_READWRITE, PROPERTY_STD_STRING,         
              REFLECTION_GETTER(Lighting* instance = (Lighting*)object; return &instance->m_timeOfDay; ), 
              REFLECTION_SETTER(Lighting* instance = (Lighting*)object; instance->setTimeOfDay(*(std::string*)value); ) },
        };

        properties.insert(properties.end(), _properties, _properties+(sizeof(_properties)/sizeof(ReflectionProperty)));
    }
}