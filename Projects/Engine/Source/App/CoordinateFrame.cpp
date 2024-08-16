#include <App/CoordinateFrame.hpp>

namespace RNR
{
    CoordinateFrame::CoordinateFrame() : m_position(0.f,0.f,0.f), m_rotation()
    {
        Ogre::Quaternion quat(0, 0, 0, 0);
        quat.ToRotationMatrix(m_rotation);
    }

    Ogre::Matrix4 CoordinateFrame::getMatrix()
    {
        Ogre::Matrix4 res = Ogre::Matrix4();
        res.makeTransform(m_position, Ogre::Vector3(1.f,1.f,1.f), Ogre::Quaternion(m_rotation));
        return res;
    }

    CoordinateFrame CoordinateFrame::inverse()
    {
        CoordinateFrame res;
        res.m_position = -res.m_position;
        res.m_rotation = res.m_rotation.inverse();
        return res;
    }

    CoordinateFrame CoordinateFrame::operator+(Ogre::Vector3 vector)
    {
        CoordinateFrame res;
        res.m_position = m_position + vector;
        return res;
    }

    CoordinateFrame CoordinateFrame::operator*(CoordinateFrame frame)
    {
        CoordinateFrame res;
        res.m_position = m_position * frame.m_position;
        res.m_rotation = m_rotation * frame.m_rotation;
        return res;
    }

    void CoordinateFrame::lookAt(Ogre::Vector3 point)
    {
        Ogre::Vector3 dir = m_position - point;
        dir.normalise();
        Ogre::Vector3 right = Ogre::Vector3(dir.z,0,-dir.x);
        right.normalise();
        Ogre::Vector3 up = dir.crossProduct(right);
        Ogre::Quaternion orientation = Ogre::Quaternion(right, up, dir);
        orientation.ToRotationMatrix(m_rotation);
    }

    CoordinateFrame CoordinateFrame::toObjectSpace(CoordinateFrame other)
    {
        CoordinateFrame rel;
        rel.setPosition(getPosition() - other.getPosition());
        rel.setRotation(other.getRotation());
        return rel;
    }
}