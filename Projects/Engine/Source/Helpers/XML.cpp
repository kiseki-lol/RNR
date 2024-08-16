#include <Helpers/XML.hpp>

namespace RNR
{
    Ogre::Vector3 XML::getVector3(pugi::xml_node node)
    {
        return Ogre::Vector3(
            node.child("X").text().as_float(),
            node.child("Y").text().as_float(),
            node.child("Z").text().as_float()
        );
    }

    Ogre::Vector3 XML::getColor(pugi::xml_node node)
    {
        Ogre::Vector3 rgb;
        unsigned int hex = node.text().as_uint();
        rgb.x = ((hex >> 16) & 0xff) / 255.0;
        rgb.y = ((hex >> 8)  & 0xff) / 255.0;
        rgb.z = ((hex)       & 0xff) / 255.0;
        return rgb;
    }

    CoordinateFrame XML::getCFrame(pugi::xml_node node)
    {
        CoordinateFrame cframe;

        cframe.setPosition(
            Ogre::Vector3(
                node.child("X").text().as_float(),
                node.child("Y").text().as_float(),
                node.child("Z").text().as_float()
            )
        );

        cframe.setRotation(
            Ogre::Matrix3(
                node.child("R00").text().as_float(), node.child("R01").text().as_float(),  node.child("R02").text().as_float(), 
                node.child("R10").text().as_float(), node.child("R11").text().as_float(),  node.child("R12").text().as_float(), 
                node.child("R20").text().as_float(), node.child("R21").text().as_float(),  node.child("R22").text().as_float()
            )
        );

        return cframe;
    }
}