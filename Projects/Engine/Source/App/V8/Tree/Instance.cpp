#include <App/V8/Tree/Instance.hpp>
#include <App/Script/Bridge.hpp>
#include <App/V8/World/World.hpp>
#include <Network/NetworkPeer.hpp>
#include <Helpers/Strings.hpp>
#include <lua.h>

namespace RNR
{
    Instance::Instance(World* world)
    {
        this->world = world;
        m_node = 0;
        m_object = 0;
        m_parent = 0;
        setName("Instance");
    }

    Instance::~Instance()
    {
        setParent(NULL);
        if(getNode())
        {
            getNode()->removeAndDestroyAllChildren();
            delete getNode();
        }
        if(getObject())
        {
            if(getObject()->getParentNode() != 0)
                getObject()->detachFromParent();
            delete getObject();
        }
    }

    std::vector<ReflectionProperty> Instance::getProperties()
    {
        ReflectionProperty properties[]  = {
            { this, std::string("Name"), std::string("This is the name of this Instance."), 
              ACCESS_NONE, OPERATION_READWRITE, PROPERTY_STD_STRING,         
              REFLECTION_GETTER(Instance* instance = (Instance*)object; return &instance->m_name; ), 
              REFLECTION_SETTER(Instance* instance = (Instance*)object; instance->setName(*(std::string*)value); ) },
            { this, std::string("Parent"), std::string("This is the parent of this Instance."), 
              ACCESS_NONE, OPERATION_READ, PROPERTY_INSTANCE,         
              REFLECTION_GETTER(Instance* instance = (Instance*)object; return instance->m_parent; ), 
              REFLECTION_SETTER(Instance* instance = (Instance*)object; instance->setParent((Instance*)value); ) },
            { this, std::string("Archivable"), std::string("This determines whether this Instance may be saved or replicated."), 
              ACCESS_NONE, OPERATION_READWRITE, PROPERTY_BOOL,         
              REFLECTION_GETTER(Instance* instance = (Instance*)object; return &instance->m_archivable; ), 
              REFLECTION_SETTER(Instance* instance = (Instance*)object; instance->m_archivable = *(bool*)value; instance->replicatorAddChangedProperty("Archivable"); ) },
        };

        std::vector<ReflectionProperty> _properties(properties, properties+(sizeof(properties)/sizeof(ReflectionProperty)));
        addProperties(_properties);

        return _properties;
    }

    std::vector<ReflectionFunction> Instance::getFunctions()
    {
        ReflectionFunction functions[] = {
            { "IsA", [](lua_State* l){ Instance* instance = Lua::InstanceBridge::singleton()->toInstance(l, 1); lua_pushboolean(l, instance->isA(lua_tostring(l, -1))); return 1; } },
            { "Clone", [](lua_State* l){ Instance* instance = Lua::InstanceBridge::singleton()->toInstance(l, 1); Lua::InstanceBridge::singleton()->fromInstance(l, instance->clone()); return 1; }},
            { "Destroy", [](lua_State* l){ Instance* instance = Lua::InstanceBridge::singleton()->toInstance(l, 1); instance->setParent(NULL); return 0; }},
            { "Remove", [](lua_State* l){ Instance* instance = Lua::InstanceBridge::singleton()->toInstance(l, 1); instance->setParent(NULL); return 0; }}
        };

        std::vector<ReflectionFunction> _functions(functions, functions+(sizeof(functions)/sizeof(ReflectionFunction)));
        addFunctions(_functions);
        return _functions;
    }

    void Instance::deserializeXmlProperty(pugi::xml_node prop)
    {
        pugi::xml_attribute prop_name = prop.attribute("name");
        if(prop_name.as_string() == std::string("Name"))
            setName(prop.text().as_string());
        else if(prop_name.as_string() == std::string("archivable"))
            m_archivable = prop.text().as_bool();
        else
            deserializeProperty((char*)prop_name.as_string(), prop);
    }

    bool Instance::contains(Instance* child)
    {
        auto child_it = std::find(m_children.begin(), m_children.end(), child);            

        return child_it != m_children.end();
    }

    bool Instance::isAncestorOf(Instance* instance)
    {
        Instance* instance_parent = instance->m_parent;
        while (instance_parent != 0)
        {
            instance_parent = instance_parent->m_parent;

            if (instance_parent == this)
                return true;
        }

        return false;
    }

    bool Instance::askSetParent(Instance* instance)
    {
        return true;
    }

    bool Instance::canSetParent(Instance* instance)
    {
        return !instance || instance->canAddChild(this);
    }

    bool Instance::askAddChild(Instance* instance)
    {
        return true;
    }

    bool Instance::canAddChild(Instance* instance)
    {
        if (instance->contains(this) || instance->m_parent == this)
            return false;
        
        if (askAddChild(instance))
            return true;
        
        return instance->askSetParent(this);
    }

    void Instance::setName(std::string name)
    {
        if (name != this->m_name)
        {
            this->m_name = name;
            replicatorAddChangedProperty("Name");
            // raise property changed
        }
    }

    void Instance::setParent(Instance* newParent)
    {
        if (newParent != m_parent)
        {
            char error_text[255];
            if (this == newParent)
            {
                snprintf(error_text, 255, "Attempt to set %s as its own parent", m_name.c_str());
                throw std::runtime_error(error_text);
            }
            if (newParent && isAncestorOf(newParent))
            {
                snprintf(error_text, 255, "Attempt to set parent of %s to %s results in circular reference", newParent->getName().c_str(), m_name.c_str());
                throw std::runtime_error(error_text);
            }

            if (m_parent)
            {
                std::vector<Instance*>* children = m_parent->getChildren();
                auto child_it = std::find(children->begin(), children->end(), this);

                if (child_it != children->end())
                {
                    children->erase(child_it);
                    m_parent->onChildRemoved(this);
                    m_parent->onDescendantRemoved(this);
                    for(auto& child : m_children)
                        descendantRemovedChildren(m_parent, child);
                
                    if (m_parent->numChildren() == 0)
                    {
                        // signal onlastchildremoved
                    }
                }
            }

            onSetParent(newParent);
            m_parent = newParent;

            if(m_parent)
            {
                m_parent->m_children.push_back(this);
                newParent->onChildAdded(this);
                newParent->onDescendantAdded(this);
                for(auto& child : m_children)
                    descendantAddedChildren(m_parent, child);
            }
        }
    }

    void Instance::descendantAddedChildren(Instance* p, Instance* c)
    {
        for(auto& child : *c->getChildren())
            descendantAddedChildren(p, child);
        p->onDescendantAdded(c);
    }

    void Instance::descendantRemovedChildren(Instance* p, Instance* c)
    {
        for(auto& child : *c->getChildren())
            descendantRemovedChildren(p, child);
        p->onDescendantRemoved(c);
    }

    void Instance::replicatorAddChangedProperty(const char* name)
    {
        if(!world)
            return;
        Instance* dm = world->getDatamodel();
        if(!dm)
            return;
        for(auto prop : getProperties())
        {
            if(prop.name() == name) 
            {
                NetworkPeer* s = dynamic_cast<NetworkPeer*>(dm->findFirstChildOfType("NetworkServer"));
                if(s && canReplicate(true))
                    s->addChangedProperty(&prop);
                NetworkPeer* c = dynamic_cast<NetworkPeer*>(dm->findFirstChildOfType("NetworkClient"));
                if(c && canReplicate(false))
                    c->addChangedProperty(&prop);
            }
        }
    }

    void Instance::onChildAdded(Instance* childAdded)
    {
        //
    }

    void Instance::onDescendantAdded(Instance* descendantAdded)
    {
        if(m_parent)
            m_parent->onDescendantAdded(descendantAdded);        
    }

    void Instance::onChildRemoved(RNR::Instance* childRemoved)
    {

    }

    void Instance::onDescendantRemoved(Instance* descendantRemoved)
    {
        if(m_parent)
            m_parent->onDescendantRemoved(descendantRemoved);
    }

    void Instance::onSetParent(Instance* newParent)
    {
        replicatorAddChangedProperty("Parent");
    }

    bool Instance::isA(std::string type)
    {
        if (type == getClassName())
            return true; // TODO: check if type is any of parent types
        return false;
    }

    Instance* Instance::findFirstChild(std::string name)
    {
        return NULL;
        for(auto& child : m_children)
        {
            if(child->getName() == name)
                return child;
        }
        return NULL;
    }

    Instance* Instance::findFirstChildOfType(std::string type)
    {
        for(auto& child : m_children)
        {
            if(child->isA(type))
                return child;
        }
        return NULL;
    }

    Instance* Instance::clone()
    {
        // TODO
        return NULL;
    }

    void Instance::addReplicate(bool server)
    {
        if(canReplicate(server))
        {
            DataModel* dm = world->getDatamodel();
            std::string new_guid = Strings::random_hex(8);
            dm->registerInstanceByGuid(this, new_guid, true);
        }
    }
}