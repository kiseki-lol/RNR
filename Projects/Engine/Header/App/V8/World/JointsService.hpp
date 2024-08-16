#pragma once
#include <App/V8/Tree/Instance.hpp>
#include <App/V8/Tree/ModelInstance.hpp>
#include <App/V8/DataModel/PartInstance.hpp>
#include <App/V8/World/Weld.hpp>
#include <Helpers/NormalId.hpp>

namespace RNR
{
    class JointsService : public Instance
    {
        int m_jointsToDo;
        int m_jointsDone;
    public:
        JointsService();

        virtual std::string getClassName() { return "JointsService"; }
        Snap* snap(PartInstance* a, PartInstance* b, Ogre::Vector3 contact);

        void makeJoints(Instance* m);
        void breakJoints(Instance* b);

        void fixWelds();

        bool isWelded(PartInstance* a, PartInstance* b);
    };
}