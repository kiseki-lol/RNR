#include <App/V8/World/ComPlicitNgine.hpp>
#include <App/V8/World/World.hpp>
#include <App/V8/World/JointsService.hpp>
#include <Helpers/Bullet.hpp>

namespace RNR
{
    void ComPlicitNgine::thread()
    {
        float delta;
        float time;
        while(m_world->getPhysicsShouldBeRunningPleaseStopIfItIsStillRunning())
        {
            delta = m_physicsTimer->getMicroseconds() / 1000000.0;
            time += m_physicsTimer->getMilliseconds() / 1000.0;
            m_physicsTimer->reset();
            
            m_lastPhysicsDelta = delta;
            m_physicsTime = time;

            std::this_thread::sleep_for(std::chrono::duration<float>(std::max(0.f,1.f/1000.f - m_lastPhysicsDelta)));
            
            if(m_world->getRunService()->getRunning())
            {
                JointsService* joints = (JointsService*)m_world->getDatamodel()->getService("JointsService");
                joints->fixWelds();
                m_world->preStep();
                m_world->step(delta);

                m_physicsTicks++;
            }
            updateTree();


            m_lastPhysicsDelta = delta;
        }
    }

    ComPlicitNgine::ComPlicitNgine(World* world)
    {
        m_physicsTimer = new Ogre::Timer();
        m_physicsTime = 0.0;
        m_dynamicsWorld = world->getDynamicsWorld();
        m_world = world;

        m_physicsThread = std::thread(&ComPlicitNgine::thread, this);
    }

    ComPlicitNgine::~ComPlicitNgine()
    {
        m_physicsThread.join();
    }

    void ComPlicitNgine::updateTreeRender()
    {
        for(int j = m_dynamicsWorld->getNumCollisionObjects() - 1; j >= 0; j--)
        {
            btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[j];
            if(obj->getActivationState() != ACTIVE_TAG)
                continue;
            PartInstance* part = (PartInstance*)obj->getUserPointer();
            if(part)
            {
                part->updateMatrix();
            }
        }   
    }

    void ComPlicitNgine::updateTree()
    {
        /*int numContacts = 0;
        m_worldManifolds = m_dynamicsWorld->getDispatcher()->getNumManifolds();
        JointsService* joints = (JointsService*)m_world->getDatamodel()->getService("JointsService");
        for(int i = 0; i < m_worldManifolds; i++)
        {
            btPersistentManifold* contact = m_dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
            const btCollisionObject* obj0 = contact->getBody0();
            const btCollisionObject* obj1 = contact->getBody1();
            PartInstance* part0 = (PartInstance*)obj0->getUserPointer();
            PartInstance* part1 = (PartInstance*)obj1->getUserPointer();
            if(part0 && part1 && !joints->isWelded(part0, part1))
            {
                numContacts += contact->getNumContacts();
                for(int j = 0; j < contact->getNumContacts(); j++)
                {
                    btManifoldPoint& pt = contact->getContactPoint(j);
                    btVector3 ptA = pt.getPositionWorldOnA();
                    btVector3 ptB = pt.getPositionWorldOnB();
                    //joints->snap(part0, part1);
                }
            }
        }
        m_worldContacts = numContacts;*/
        int sleepingObjects = 0;
        int activeObjects = 0;
        int errorObjects = 0;
        for(int j = m_dynamicsWorld->getNumCollisionObjects() - 1; j >= 0; j--)
        {
            btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[j];
            if(!obj)
                continue;
            if(obj->getActivationState() != ACTIVE_TAG) { sleepingObjects++; continue; }
            activeObjects++;
            btRigidBody* body = btRigidBody::upcast(obj);
            btTransform trans;
            if(body && body->getMotionState())
            {
                body->getMotionState()->getWorldTransform(trans);
            }
            else
            {
                trans = obj->getWorldTransform();
            }
            PartInstance* part = (PartInstance*)obj->getUserPointer();
            if(part && m_world->getRunService()->getRunning())
            {
/*
                part->getCFrame().setPosition(Bullet::v3ToOgre(trans.getOrigin()));
                Ogre::Matrix3 partRot;
                Ogre::Quaternion transOgre = Bullet::qtToOgre(trans.getRotation());
                transOgre.ToRotationMatrix(partRot);
                part->getCFrame().setRotation(partRot);
                */
                part->setCFrame(Bullet::tfToCoordinateFrame(trans));
            }
        }
        m_sleepingObjects = sleepingObjects;
        m_activeObjects = activeObjects;
    }

    void ComPlicitNgine::registerPhysicsPart(PartInstance* partRegistered)
    {
        btCollisionShape* partShape = new btBoxShape(Bullet::v3ToBullet(partRegistered->getSize() / 2.f));
        partShape->setUserPointer(partRegistered);

        btTransform partTransform = Bullet::cfToTransform(partRegistered->getCFrame());

        btScalar mass = partRegistered->getSize().length();
        if(partRegistered->getAnchored())
            mass = 0;
        
        btVector3 localInertia = btVector3(0,0,0);        
        if(mass)
            partShape->calculateLocalInertia(mass, localInertia);

        btDefaultMotionState* partMotionState = new btDefaultMotionState(partTransform);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, partMotionState, partShape, localInertia);
        btRigidBody* body = new btRigidBody(rbInfo);
        body->setUserPointer(partRegistered);

        m_world->dynamicWorldLock.lock();
        m_dynamicsWorld->addRigidBody(body);        
        m_world->dynamicWorldLock.unlock();

        m_physicsParts[partRegistered] = body;
    }

    void ComPlicitNgine::updatePhysicsPart(PartInstance* partUpdate)
    {
        btRigidBody* toUpdate = m_physicsParts[partUpdate];
        if(toUpdate)
        {
            btTransform& bodytf = toUpdate->getWorldTransform();
            bodytf.setIdentity();
            bodytf.setOrigin(Bullet::v3ToBullet(partUpdate->getPosition()));
            bodytf.setRotation(Bullet::qtToBullet(partUpdate->getRotation()));
        }
    }

    void ComPlicitNgine::deletePhysicsPart(PartInstance* partDelete)
    {
        btRigidBody* toDelete = m_physicsParts[partDelete];
        if(toDelete)
        {
            
        }
    }
}