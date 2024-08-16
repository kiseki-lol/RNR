#pragma once
#include <thread>
#include <OGRE/Ogre.h>
#include <App/V8/DataModel/PartInstance.hpp>
#include "LinearMath/btVector3.h"
#include "btBulletDynamicsCommon.h"
#include <map>

namespace RNR
{
    class World;

    class ComPlicitNgine
    {
        std::thread m_physicsThread;
        Ogre::Timer* m_physicsTimer;
        float m_lastPhysicsDelta;
        float m_physicsTime;
        btDiscreteDynamicsWorld* m_dynamicsWorld;
        World* m_world;
        std::map<PartInstance*, btRigidBody*> m_physicsParts;

        int m_physicsTicks;
        int m_sleepingObjects;
        int m_activeObjects;

        void thread();
    public:
        ComPlicitNgine(World* world);
        ~ComPlicitNgine();

        void updateTree();
        void updateTreeRender();

        Ogre::Timer* getPhysicsTimer() { return m_physicsTimer; }
        float getLastPhysicsDelta() { return m_lastPhysicsDelta; }
        float getPhysicsTime() { return m_physicsTime; }

        btRigidBody* getBody(PartInstance* part) { return m_physicsParts[part]; };

        void registerPhysicsPart(PartInstance* partRegistered);
        void updatePhysicsPart(PartInstance* partUpdate);
        void deletePhysicsPart(PartInstance* partDelete);

        int getSleepingObjects() { return m_sleepingObjects; }
        int getActiveObjects() { return m_activeObjects; }
        int getPhysicsTicks() { return m_physicsTicks; };
    };
}