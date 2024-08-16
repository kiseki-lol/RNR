#pragma once

#include <pugixml.hpp>
#include <OGRE/Ogre.h>
#include <App/CoordinateFrame.hpp>

namespace RNR
{
    class XML
    {
    public:
        static Ogre::Vector3 getVector3(pugi::xml_node node);
        static Ogre::Vector3 getColor(pugi::xml_node node);
        static CoordinateFrame getCFrame(pugi::xml_node node);
    };
}