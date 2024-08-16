#pragma once
#include <App/V8/Tree/Instance.hpp>
#include <OGRE/Ogre.h>

namespace RNR
{
    class Lighting : public Instance
    {
        float m_brightness;
        float m_geographicLatitude;
        float m_sunBrightness;

        std::string m_timeOfDay;
        Ogre::Vector3 m_bottomAmbient;
        Ogre::Vector3 m_topAmbient;
        Ogre::Vector3 m_spotLight;
        Ogre::Vector3 m_shadowColor;
        Ogre::Vector3 m_clearColor;
        Ogre::Billboard* m_sun;
        Ogre::SceneNode* m_sunNode;
        Ogre::Vector3 m_sunOrigin;

        void updateSunPosition();

        virtual void addProperties(std::vector<ReflectionProperty>& properties);
        virtual void deserializeProperty(char* prop_name, pugi::xml_node prop);
    public:
        Lighting();
        virtual void lateInit();

        void setSunOrigin(Ogre::Vector3 vec);
        void setMinutesAfterMidnight(int m);

        void setBottomAmbient(Ogre::Vector3 color) { m_bottomAmbient = color; }
        Ogre::Vector3 getBottomAmbient() { return m_bottomAmbient; }
        void setTopAmbient(Ogre::Vector3 color) { m_topAmbient = color; }
        Ogre::Vector3 getTopAmbient() { return m_topAmbient; }
        void setSpotLight(Ogre::Vector3 color) { m_spotLight = color; }
        Ogre::Vector3 getSpotLight() { return m_spotLight; }
        void setShadowColor(Ogre::Vector3 color) { m_shadowColor = color; }
        Ogre::Vector3 getShadowColor() { return m_shadowColor; }
        void setClearColor(Ogre::Vector3 color) { m_clearColor = color; }
        Ogre::Vector3 getClearColor() { return m_clearColor; }
        void setGeographicLatitude(float lat) { m_geographicLatitude = lat; updateSunPosition(); }
        float getGeographicLatitude() { return m_geographicLatitude; }
        void setTimeOfDay(std::string time) { m_timeOfDay = time; updateSunPosition(); }
        std::string getTimeOfDay() { return m_timeOfDay; }
        float getSunBrightness() { return m_sunBrightness; }

        virtual std::string getClassName() { return "Lighting"; }
        virtual std::string getExplorerIcon() { return "PointLight"; }
    };
}