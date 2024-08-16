#pragma once

#include <stdexcept>
#include <string>
#include <cstddef>

#include <OGRE/Ogre.h>
#include <boost/shared_ptr.hpp>

#include <Helpers/Name.hpp>
#include <Helpers/Lock.hpp>
#include <pugixml.hpp>

#include <App/Script/ReflectionProperty.hpp>
#include <App/Script/ReflectionFunction.hpp>

namespace RNR
{
    class World;

    class Instance
    {
        protected:
            World* world;
            virtual void deserializeProperty(char* prop_name, pugi::xml_node prop) {};
            virtual void addProperties(std::vector<ReflectionProperty>& properties) {};
            virtual void addFunctions(std::vector<ReflectionFunction>& functions) {};
        private:

            Ogre::SceneNode* m_node;
            Ogre::MovableObject* m_object;
            std::string m_name;
            RNR::Instance* m_parent;
            std::vector<RNR::Instance*> m_children;
            bool m_archivable;

            void descendantAddedChildren(Instance* p, Instance* c);
            void descendantRemovedChildren(Instance* p, Instance* c);
            void replicatorAddChangedProperty(const char* name); // call this in setters that replicate
        
        public:
            Instance(World* world = NULL);
            virtual void lateInit() {}; // called when the Instance gets added to a World
            virtual void lateDeInit() {}; // called before the Instance gets removed from a World
            virtual ~Instance();
            
            virtual std::vector<ReflectionProperty> getProperties();
            virtual std::vector<ReflectionFunction> getFunctions();
            void deserializeXmlProperty(pugi::xml_node prop); // TODO: eventually replace this with a method that uses getProperties

            bool contains(RNR::Instance* child);
            bool isAncestorOf(RNR::Instance* instance);
            bool isA(std::string type);

            Instance* findFirstChild(std::string name);
            Instance* findFirstChildOfType(std::string type);
            
            World* getWorld() { return world; }
            void setWorld(World* world) { this->world = world; }

            virtual bool askSetParent(RNR::Instance* instance); // derive this
            bool canSetParent(RNR::Instance* instance);
            virtual bool askAddChild(RNR::Instance* instance); // derive this
            bool canAddChild(RNR::Instance* instance);

            RNR::Instance* createChild(const RNR::Name *className);

            RNR::Instance* getParent() { return this->m_parent; };
            std::string getName() { return this->m_name; };
            virtual std::string getClassName() { return "Instance"; }
            virtual std::string getExplorerIcon() { return getClassName(); }

            void setParent(RNR::Instance* newParent);
            void setName(std::string name);
            void setNode(Ogre::SceneNode* node) { m_node = node; };
            Ogre::SceneNode* getNode() { return m_node; };
            void setObject(Ogre::MovableObject* object) { m_object = object; };
            Ogre::MovableObject* getObject() { return m_object; };
            std::vector<RNR::Instance*>* getChildren() { return &this->m_children; };
            int numChildren() { return this->m_children.size(); };

            virtual void onChildAdded(RNR::Instance* childAdded);
            virtual void onDescendantAdded(RNR::Instance* descendantAdded); // make sure this is called in any derived versions of this
            virtual void onChildRemoved(RNR::Instance* childRemoved);
            virtual void onDescendantRemoved(RNR::Instance* descendantRemoved); // make sure this is called in any derived versions of this
            virtual void onSetParent(RNR::Instance* newParent);

            virtual bool canReplicate(bool server = false) { return server; };
            void addReplicate(bool server = false);

            Instance* clone();

            Lock instanceLock;
    };
}