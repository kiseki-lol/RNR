#include <App/V8/World/JointsService.hpp>
#include <App/V8/World/World.hpp>
#include <App/V8/World/ComPlicitNgine.hpp>
#include <Helpers/Bullet.hpp>

namespace RNR
{
    JointsService::JointsService()
    {
        setName("JointsService");
        m_jointsToDo = 0;
        m_jointsDone = 0;
    }

    Snap* JointsService::snap(PartInstance* a, PartInstance* b, Ogre::Vector3 contact)
    {
        //if(isWelded(a, b))
        //    return NULL;
        Snap* snap = new Snap();
        snap->setParent(this);
        snap->setBodies(a, b);
        CoordinateFrame contactFrame;
        contactFrame.setPosition(contact);

        btVector3 p_axis(1.f,0.f,0.f);
        btVector3 c_axis(0.f,0.f,1.f);
        btVector3 anchor(0.f,0.f,0.f);

        btVector3 z_axis = p_axis.normalize();
        btVector3 y_axis = c_axis.normalize();
        btVector3 x_axis = y_axis.cross(z_axis);

        btTransform frame_in;
        frame_in.setIdentity();
        frame_in.getBasis().setValue(
            x_axis[0], y_axis[0], z_axis[0],
            x_axis[1], y_axis[1], z_axis[1],
            x_axis[2], y_axis[2], z_axis[2]
        );
        frame_in.setOrigin(anchor);

        ComPlicitNgine* ngine = world->getComPlicitNgine();

        btTransform frame_inA = ngine->getBody(a)->getCenterOfMassTransform().inverse() * frame_in;
        btTransform frame_inB = ngine->getBody(b)->getCenterOfMassTransform().inverse() * frame_in;

        snap->setC0(Bullet::tfToCoordinateFrame(frame_inA));
        snap->setC1(Bullet::tfToCoordinateFrame(frame_inB));
        return snap;
    }

    void JointsService::fixWelds()
    {
        for(auto& child : *getChildren())
        {
            JointInstance* weld = dynamic_cast<JointInstance*>(child);
            if(weld->getDirty())
                weld->link();
        }
    }

    void JointsService::makeJoints(Instance* w)
    {
        btDynamicsWorld* dynamicsWorld = world->getDynamicsWorld();
        dynamicsWorld->performDiscreteCollisionDetection();
        m_jointsToDo = dynamicsWorld->getDispatcher()->getNumManifolds();
        m_jointsDone = 0;
        for(int i = 0; i < m_jointsToDo; i++)
        {
            world->setLoadProgress(m_jointsDone);
            world->setMaxLoadProgress(m_jointsToDo);
            if(world->getLoadListener())
                world->getLoadListener()->updateWorldLoad();
            m_jointsDone++;

            btPersistentManifold* contact = dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
            const btCollisionObject* obj0 = contact->getBody0();
            const btCollisionObject* obj1 = contact->getBody1();
            PartInstance* part0 = (PartInstance*)obj0->getUserPointer(); // A
            PartInstance* part1 = (PartInstance*)obj1->getUserPointer(); // B

            if(!w->isAncestorOf(part0) && !w->isAncestorOf(part1))
                continue;
            if(isWelded(part0, part1))
                continue;

            if(part0 && part1)
            {
                for(int j = 0; j < contact->getNumContacts(); j++)
                {
                    btManifoldPoint& pt = contact->getContactPoint(j);
                    btVector3 ptA = pt.getPositionWorldOnA();
                    btVector3 ptB = pt.getPositionWorldOnB();
                    btVector3 ptN = pt.m_normalWorldOnB;
                    
                    NormalId bFace = NORM_UNDEFINED;
                    if(ptN == btVector3(1, 0, 0))
                        bFace = NORM_RIGHT;
                    else if(ptN == btVector3(-1, 0, 0))
                        bFace = NORM_LEFT;
                    else if(ptN == btVector3(0, 1, 0))
                        bFace = NORM_UP;
                    else if(ptN == btVector3(0, -1, 0))
                        bFace = NORM_DOWN;
                    else if(ptN == btVector3(0, 0, 1))
                        bFace = NORM_FRONT;
                    else if(ptN == btVector3(0, 0, -1))
                        bFace = NORM_BACK;

                    if(bFace != NORM_UNDEFINED)
                    {
                        NormalId oFace = normalIdOpposite(bFace);
                        PartSurfaceInfo surfB = part1->getSurface(bFace);
                        PartSurfaceInfo surfA = part0->getSurface(oFace);
                        bool link = surfB.links(surfA);
                        if(link)
                        {
                            switch(part0->getSurface(bFace).type)
                            {
                            case SURFACE_WELD:
                            case SURFACE_UNIVERSAL:
                            case SURFACE_INLET:
                            case SURFACE_STUDS:
                                snap(part0,part1,Bullet::v3ToOgre(pt.m_positionWorldOnA));
                                break;
                            case SURFACE_SMOOTH:
                            default:
                                break;
                            }
                        }
                    }
                }
            }
        }
    }

    void JointsService::breakJoints(Instance* p)
    {
        for(auto& child : *getChildren())
        {
            JointInstance* snap = dynamic_cast<JointInstance*>(child);
            if(snap)
            {
                bool brk = false;
                if(snap->getABody() == p)
                    brk = true;
                else if(snap->getBBody() == p)
                    brk = true;
                if(brk)
                {
                    snap->setParent(NULL);
                    delete snap;
                }
            }
        }
    }

    bool JointsService::isWelded(PartInstance* a, PartInstance* b)
    {
        for(auto& child : *getChildren())
        {
            JointInstance* snap = dynamic_cast<JointInstance*>(child);
            if(snap)
            {
                if(snap->getABody() == a && snap->getBBody() == b)
                    return true;
                else if(snap->getABody() == b && snap->getBBody() == a)
                    return true;
            }
        }
        return false;
    }
}