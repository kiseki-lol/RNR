#pragma once
#include <App/V8/Tree/Instance.hpp>
#include <App/CoordinateFrame.hpp>

namespace RNR
{
    class PVInstance : public Instance
    {
    protected:
        CoordinateFrame m_cframe;
        virtual void deserializeProperty(char* prop_name, pugi::xml_node prop);
        virtual void addProperties(std::vector<ReflectionProperty>& properties);
    public:
        PVInstance();

        CoordinateFrame& getCFrame() { return m_cframe; };
        void setCFrame(CoordinateFrame cframe) { m_cframe = cframe; };
        Ogre::Vector3 relativePositionTo(PVInstance* point);
        virtual std::string getClassName() { return "PVInstance"; }

        Ogre::Vector3 getRelativePosition(PVInstance* other) { return other->getPosition() - getPosition(); }
        Ogre::Vector3 getPosition() { return m_cframe.getPosition(); }
        Ogre::Matrix3 getRotation() { return m_cframe.getRotation(); }
    };
}
