#pragma once
#include <OGRE/Ogre.h>

namespace RNR
{
    class CoordinateFrame
    {
        Ogre::Vector3 m_position;
        Ogre::Matrix3 m_rotation;
public:
        CoordinateFrame();

        void lookAt(Ogre::Vector3 point);
        CoordinateFrame inverse();

        void setRotation(Ogre::Matrix3 rotation) { m_rotation = rotation; }
        void setPosition(Ogre::Vector3 position) { m_position = position; }
        CoordinateFrame toObjectSpace(CoordinateFrame cframe);
        Ogre::Matrix3 getRotation() { return m_rotation; }
        Ogre::Vector3 getPosition() { return m_position; }

        Ogre::Matrix4 getMatrix();

        CoordinateFrame operator+(Ogre::Vector3 vector);
        CoordinateFrame operator*(CoordinateFrame frame);
    };
}