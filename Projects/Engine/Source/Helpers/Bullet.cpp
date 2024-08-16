#include <Helpers/Bullet.hpp>

namespace RNR
{
    btVector3 Bullet::v3ToBullet(Ogre::Vector3 v)
    {
        return btVector3(v.x, v.y, v.z);
    }

    Ogre::Vector3 Bullet::v3ToOgre(btVector3 v)
    {
        return Ogre::Vector3(v.getX(), v.getY(), v.getZ());
    }

    btQuaternion Bullet::qtToBullet(Ogre::Quaternion q)
    {
        return btQuaternion(q.x, q.y, q.z, q.w);
    }

    Ogre::Quaternion Bullet::qtToOgre(btQuaternion q)
    {
        return Ogre::Quaternion(q.getW(), q.getX(), q.getY(), q.getZ());
    }

    btTransform Bullet::cfToTransform(CoordinateFrame frame)
    {
        btTransform bframe;
        bframe.setIdentity();
        bframe.setOrigin(v3ToBullet(frame.getPosition()));
        bframe.setRotation(qtToBullet(frame.getRotation()));
        return bframe;
    }

    CoordinateFrame Bullet::tfToCoordinateFrame(btTransform transform)
    {
        CoordinateFrame cframe;
        cframe.setPosition(v3ToOgre(transform.getOrigin()));
        Ogre::Matrix3 rot_mat;
        Ogre::Quaternion rot_quat = qtToOgre(transform.getRotation());
        rot_quat.ToRotationMatrix(rot_mat);
        cframe.setRotation(rot_mat);
        return cframe;
    }
}