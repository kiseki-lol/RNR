#include <App/V8/DataModel/Workspace.hpp>
#include <App/V8/World/World.hpp>
#include <App/V8/DataModel/PartInstance.hpp>
#include <App/V8/DataModel/Camera.hpp>
#include <OGRE/Ogre.h>

namespace RNR
{
    Workspace::Workspace() : ModelInstance()
    {
        setName("Workspace");
        m_batchMode = BATCH_INSTANCED;
    }

    void Workspace::lateInit()
    {
        m_worldspawn = world->getOgreSceneManager()->getRootSceneNode()->createChildSceneNode();    

        if(world->getHasRender())
        {
            switch(m_batchMode)
            {
                case BATCH_INSTANCED:
                    m_instanceManager = world->getOgreSceneManager()->createInstanceManager("workspaceInstanceManager", "meshes/Cube_Instanced.mesh", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Ogre::InstanceManager::InstancingTechnique::HWInstancingBasic, 255);
                    m_instanceManager->setNumCustomParams(2);
                    break;
                case BATCH_STATIC_GEOMETRY:
                    m_geom = world->getOgreSceneManager()->createStaticGeometry("workspaceGeom");
                    m_geom->setRegionDimensions(Ogre::Vector3(4096, 4096, 4096));
                    m_geom->setCastShadows(true);
                    break;
                case BATCH_DONT:
                    break;
            }
        }
    }

    void Workspace::onDescendantAdded(Instance* childAdded)
    {
        PartInstance* part = dynamic_cast<PartInstance*>(childAdded);
        if(part)
        {
            if(world->getHasRender())
            {
                Ogre::SceneNode* child_node = childAdded->getNode();
                switch(m_batchMode)
                {
                    case BATCH_INSTANCED:
                        {
                            Ogre::InstancedEntity* replica;
                            if(part->getTransparency() != 0.0)
                                replica = m_instanceManager->createInstancedEntity("InstancedMaterialTransparent");
                            else
                                replica = m_instanceManager->createInstancedEntity("InstancedMaterial");
                            part->updateMatrix();
                            replica->setScale(part->getSize());
                            replica->setPosition(part->getCFrame().getPosition());
                            replica->setOrientation(part->getCFrame().getRotation());
                            Ogre::Vector3 brickColor = BrickColor::color(part->getBrickColor());
                            replica->setCustomParam(0, Ogre::Vector4f(brickColor.x, brickColor.y, brickColor.z, 1.0-part->getTransparency()));
                            replica->setCustomParam(1, Ogre::Vector4f(part->getSize().x, part->getSize().y, part->getSize().z, 0.0));
                            m_worldspawn->attachObject(replica);
                            childAdded->setObject(replica);
                            if(child_node)
                                child_node->setVisible(false);
                        }
                        break;
                    case BATCH_STATIC_GEOMETRY:
                        m_geom->addEntity((Ogre::Entity*)childAdded->getObject(), 
                            part->getPosition(), 
                            part->getCFrame().getRotation(),
                            part->getSize());
                        if(child_node)
                            child_node->setVisible(false);
                        m_geomDirty = true;
                        break;
                    default:
                    case BATCH_DONT:
                        if(child_node)
                            child_node->setVisible(true);
                        break;
                }
            }
            world->getComPlicitNgine()->registerPhysicsPart(part);
        }
    }

    void Workspace::buildGeomInstance(Instance* instance)
    {

    }

    void Workspace::buildGeom()
    {
        if(!m_geomDirty || m_batchMode != BATCH_STATIC_GEOMETRY)
            return;
        m_geom->build();
        m_geomDirty = false;
    }

    void Workspace::onDescendantRemoved(Instance* childRemoved)
    {
        PartInstance* part = dynamic_cast<PartInstance*>(childRemoved);
        if(part)
        {
            if(m_batchMode == BATCH_INSTANCED)
            {
                Ogre::InstancedEntity* entity = (Ogre::InstancedEntity*)part->getObject();
                world->getOgreSceneManager()->destroyInstancedEntity(entity);
            }
            world->getComPlicitNgine()->deletePhysicsPart(part);
        }
        m_geomDirty = true;        
    }

    Camera* Workspace::getCurrentCamera() const
    {
        return currentCamera.get();
    }

    void Workspace::setCurrentCamera(Camera *newCamera)
    {
        if (newCamera != currentCamera.get())
        {
            currentCamera = std::shared_ptr<Camera>(newCamera);

            // TODO: raise propertyChanged and currentCameraChangedSignal
        }
    }

    void Workspace::deserializeProperty(char* prop_name, pugi::xml_node node)
    {
        if(prop_name == std::string("CurrentCamera"))
        {
            Instance* new_cam = world->getRef(node.text().as_string());
            if(new_cam)
                setCurrentCamera((Camera*)new_cam);
            else
                printf("Workspace::deserializeProperty: camera ref invalid (%s)\n", node.text().as_string());
        }
    }

    void Workspace::addProperties(std::vector<ReflectionProperty>& properties)
    {
        ReflectionProperty _properties[]  = {
            { this, std::string("CurrentCamera"), std::string(""), 
              ACCESS_NONE, OPERATION_READWRITE, PROPERTY_INSTANCE,         
              REFLECTION_GETTER(Workspace* instance = (Workspace*)object; return instance->currentCamera.get(); ), 
              REFLECTION_SETTER(Workspace* instance = (Workspace*)object; instance->setCurrentCamera((Camera*)value); ) },
        };

        properties.insert(properties.end(), _properties, _properties+(sizeof(_properties)/sizeof(ReflectionProperty)));
    }
}