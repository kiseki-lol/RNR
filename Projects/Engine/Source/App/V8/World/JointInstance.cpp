#include <App/V8/World/JointInstance.hpp>
#include <App/V8/World/World.hpp>

namespace RNR
{
    JointInstance::JointInstance()
    {
        setName("Joint");
        m_collidesWithSelf = true;
        m_dirty = false;
        m_c0 = CoordinateFrame();
        m_c1 = CoordinateFrame();
    }

    JointInstance::~JointInstance()
    {
        world->getDynamicsWorld()->removeConstraint(m_constraint);
        delete m_constraint;
    }

    void JointInstance::setBodies(PartInstance* a, PartInstance* b)
    {
        m_aInstance = a;
        m_bInstance = b;

        m_aBody = world->getComPlicitNgine()->getBody(a);
        m_bBody = world->getComPlicitNgine()->getBody(b);

        m_dirty = true;
    }

    void JointInstance::addProperties(std::vector<ReflectionProperty>& properties)
    {
        ReflectionProperty _properties[]  = {
            { this, std::string("Part0"), std::string(""), 
              ACCESS_NONE, OPERATION_READWRITE, PROPERTY_INSTANCE,         
              REFLECTION_GETTER(JointInstance* instance = (JointInstance*)object; return instance->m_aInstance; ), 
              REFLECTION_SETTER(JointInstance* instance = (JointInstance*)object;  ) },
            { this, std::string("C0"), std::string(""), 
              ACCESS_NONE, OPERATION_READWRITE, PROPERTY_CFRAME,         
              REFLECTION_GETTER(JointInstance* instance = (JointInstance*)object; return &instance->m_c0; ), 
              REFLECTION_SETTER(JointInstance* instance = (JointInstance*)object;  ) },
            { this, std::string("Part1"), std::string(""), 
              ACCESS_NONE, OPERATION_READWRITE, PROPERTY_INSTANCE,         
              REFLECTION_GETTER(JointInstance* instance = (JointInstance*)object; return instance->m_bInstance; ), 
              REFLECTION_SETTER(JointInstance* instance = (JointInstance*)object;  ) },
            { this, std::string("C1"), std::string(""), 
              ACCESS_NONE, OPERATION_READWRITE, PROPERTY_CFRAME,         
              REFLECTION_GETTER(JointInstance* instance = (JointInstance*)object; return &instance->m_c1; ), 
              REFLECTION_SETTER(JointInstance* instance = (JointInstance*)object;  ) },
        };

        properties.insert(properties.end(), _properties, _properties+(sizeof(_properties)/sizeof(ReflectionProperty)));
    }

    void JointInstance::deserializeProperty(char* prop_name, pugi::xml_node prop)
    {

    }

    void JointInstance::link()
    {
        if(!m_aBody || !m_bBody)
        {
            m_aBody = world->getComPlicitNgine()->getBody(m_aInstance);
            m_bBody = world->getComPlicitNgine()->getBody(m_bInstance);

            if(!m_aBody || !m_bBody)
                return;
        }
        m_constraint = constraint();
        m_dirty = false;
        if(m_constraint)
        {
            world->getDynamicsWorld()->addConstraint(m_constraint, !m_collidesWithSelf);
        }
        else
            printf("JointInstance::link: constraint returned NULL\n");
    }
}