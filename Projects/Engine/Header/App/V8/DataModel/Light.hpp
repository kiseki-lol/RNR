#pragma once
#include <App/V8/Tree/Instance.hpp>

namespace RNR
{
    class Light : public Instance
    {
    private:
        float m_brightness;
        Ogre::Vector3 m_color;
        bool m_enabled;
        bool m_shadows;
    public:
        Light();

        virtual void setupLight(Ogre::Light* light);
        virtual void onSetParent(RNR::Instance* newParent);
        virtual std::string getClassName() { return std::string("Light"); }

        void setColor(Ogre::Vector3 color) { m_color = color; setupLight((Ogre::Light*)getObject()); }
        Ogre::Vector3 getColor() { return m_color; }
        void setEnabled(bool enabled) { m_enabled = enabled; setupLight((Ogre::Light*)getObject()); }
        bool getEnabled() { return m_enabled; }
        void setShadows(bool shadows) { m_shadows = shadows; setupLight((Ogre::Light*)getObject()); }
        bool getShadows() { return m_shadows; }
    };
}