#include <App/V8/World/Weld.hpp>
#include <App/V8/World/World.hpp>
#include <Helpers/Bullet.hpp>

namespace RNR
{
    Weld::Weld() : JointInstance()
    {
        setName("Weld");
        m_collidesWithSelf = true;
    }

    Snap::Snap() : Weld()
    {
        setName("Snap");
    }

    btTypedConstraint* Weld::constraint()
    {
        btTransform frameInA, frameInB;
        frameInA = btTransform::getIdentity();
        frameInA.setOrigin(Bullet::v3ToBullet(m_c0.getPosition()));
        frameInA.setRotation(Bullet::qtToBullet(m_c0.getRotation()));
        frameInB = btTransform::getIdentity();
        frameInB.setOrigin(Bullet::v3ToBullet(m_c1.getPosition()));
        frameInB.setRotation(Bullet::qtToBullet(m_c1.getRotation()));

        return new btFixedConstraint(*m_aBody, *m_bBody, frameInA, frameInB);
    }
}