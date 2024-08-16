#pragma once
#include <App/V8/Tree/PVInstance.hpp>

namespace RNR
{
    class ModelInstance : public PVInstance
    {
    public:
        ModelInstance();

        virtual void build();
        void makeJoints();
        void breakJoints();
        Ogre::AxisAlignedBox getBoundingBox() { return m_boundingbox; }
        virtual std::string getClassName() { return "Model"; }
        virtual void onChildAdded(Instance* instance);
    private:
        Ogre::AxisAlignedBox m_boundingbox;
        
        void childAddBoundingBox(Instance* child);
    };
}