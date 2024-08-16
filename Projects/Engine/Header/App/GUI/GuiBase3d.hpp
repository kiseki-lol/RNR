#pragma once
#include <App/V8/Tree/Instance.hpp>
#include <OGRE/Ogre.h>

namespace RNR
{
    class GuiBase3d : public Instance, public Ogre::ManualObject
    {
    protected:
        Ogre::MaterialPtr m_material;
    private:
        Ogre::Vector3 m_color;
        float m_transparency;
        bool m_visible;
    public:
        GuiBase3d();

        void setVisible(bool visible) { m_visible = visible; }
        bool getVisible() { return m_visible; }
        void setColor(Ogre::Vector3 color) { m_color = color; }
        Ogre::Vector3 getColor() { return m_color; }
        void setTransparency(float transparency) { m_transparency = transparency; }
        float getTransparency() { return m_transparency; }
        virtual std::string getClassName() { return "GuiBase3d"; }
    };
}