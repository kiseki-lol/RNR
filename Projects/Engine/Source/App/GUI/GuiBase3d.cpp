#include <App/GUI/InstanceAdornment.hpp>
#include <OGRE/Ogre.h>

namespace RNR
{
    GuiBase3d::GuiBase3d() : m_color(0.f,0.f,1.f), ManualObject("GuiBase3d")
    {
        m_transparency = 0.f;
        m_visible = true;

        m_material = Ogre::MaterialManager::getSingletonPtr()->getByName("materials/GuiBase3d");
    }
}
