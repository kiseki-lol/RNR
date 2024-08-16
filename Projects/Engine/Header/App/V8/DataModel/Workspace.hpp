#pragma once

#include <App/V8/Tree/Instance.hpp>
#include <App/V8/Tree/ModelInstance.hpp>

#include <OGRE/Ogre.h>

#include <vector>

namespace RNR
{
    class Camera;

    enum WorkspaceBatchingMode
    {
        BATCH_DONT,
        BATCH_INSTANCED,
        BATCH_STATIC_GEOMETRY,
        BATCH_DYNAMIC_RENDERABLE,
    };

    class Workspace : public ModelInstance
    {
        friend class TopMenuBar;
    public:
        Workspace();
        virtual void lateInit();

        WorkspaceBatchingMode getBatchMode() { return m_batchMode; }
        virtual std::string getClassName() { return "Workspace"; }
        virtual void onDescendantAdded(RNR::Instance* childAdded);
        virtual void onDescendantRemoved(RNR::Instance* childRemoved);

        void buildGeom();
        
        Camera* getCurrentCamera() const;
	    void setCurrentCamera(Camera *value); 
        void setDirty() { m_geomDirty = true; }
    private:
        enum WorkspaceBatchingMode m_batchMode;
    
        virtual void addProperties(std::vector<ReflectionProperty>& properties);
        virtual void deserializeProperty(char* prop_name, pugi::xml_node prop);

        void buildGeomInstance(Instance* instance);

        bool m_geomDirty;
        Ogre::StaticGeometry* m_geom; 
        Ogre::InstanceManager* m_instanceManager;
        Ogre::SceneNode* m_worldspawn;
        Ogre::Entity* m_partEntity;

        std::shared_ptr<Camera> currentCamera;
    };
}
