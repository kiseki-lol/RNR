#pragma once
#include <App/V8/Tree/Instance.hpp>
#include <App/V8/DataModel/PartInstance.hpp>
#include <App/V8/World/ComPlicitNgine.hpp>

namespace RNR
{
    class JointInstance : public Instance
    {
    protected:
        PartInstance *m_aInstance, *m_bInstance;
        btRigidBody *m_aBody, *m_bBody;
        CoordinateFrame m_c0, m_c1;
        btTypedConstraint *m_constraint;
        bool m_dirty;
        bool m_collidesWithSelf;
        virtual void addProperties(std::vector<ReflectionProperty>& properties);
        virtual void deserializeProperty(char* prop_name, pugi::xml_node prop);

        virtual btTypedConstraint* constraint() { return NULL; };
    public:
        JointInstance();
        ~JointInstance();

        void setBodies(PartInstance* a, PartInstance* b);
        
        PartInstance* getABody() { return m_aInstance; }
        void setABody(PartInstance* a) { m_aInstance = a; m_dirty = true; }
        PartInstance* getBBody() { return m_bInstance; }
        void setBBody(PartInstance* b) { m_bInstance = b; m_dirty = true; }

        CoordinateFrame& getC0() { return m_c0; }
        void setC0(CoordinateFrame c) { m_c0 = c; m_dirty = true; }
        CoordinateFrame& getC1() { return m_c1; }
        void setC1(CoordinateFrame c) { m_c1 = c; m_dirty = true; }

        void link();
        bool getDirty() { return m_dirty; };
    };
}