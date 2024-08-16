#include <App/V8/Tree/ModelInstance.hpp>
#include <App/V8/DataModel/PartInstance.hpp>
#include <App/V8/World/World.hpp>
#include <App/V8/World/JointsService.hpp>
#include <App/Humanoid/Humanoid.hpp>

namespace RNR
{
    ModelInstance::ModelInstance() : PVInstance()
    {

    }

    void ModelInstance::build()
    {
        m_boundingbox.setNull();
        for(auto& child : *getChildren())
            childAddBoundingBox(child);
    }

    void ModelInstance::makeJoints()
    {
        JointsService* joints = world->getJointsService();
        if(joints)
            joints->makeJoints(this);
    }

    void ModelInstance::breakJoints()
    {
        JointsService* joints = world->getJointsService();
        if(joints)
            joints->makeJoints(this);
    }

    void ModelInstance::childAddBoundingBox(Instance* child)
    {
        for(auto& child2 : *child->getChildren())
            childAddBoundingBox(child2);
        PartInstance* child_pv = dynamic_cast<PartInstance*>(child);
        if(child_pv)
        {
            Ogre::Vector3 size = child_pv->getSize() / 2.0;
            m_boundingbox.merge(child_pv->getPosition() + size);
            m_boundingbox.merge(child_pv->getPosition() - size);
        }
    }

    void ModelInstance::onChildAdded(Instance* instance)
    {
        if(instance->getClassName() == "Humanoid")
        {
            Humanoid* instance_humanoid = (Humanoid*)instance;
            //instance_humanoid->createHealthBar();
        }
    }
}