#include <App/V8/DataModel/PartInstance.hpp>
#include <App/V8/World/World.hpp>
#include <Helpers/XML.hpp>

namespace RNR
{
    bool PartSurfaceInfo::intersects(PartSurfaceInfo other)
    {
        if(other.plane != plane)
            return false;
        if(start.x > other.end.x || other.start.x > end.x)
            return false;
        if(end.y > other.start.y || other.end.y > start.y)
            return false;
        return true;
    }

    bool PartSurfaceInfo::links(PartSurfaceInfo other)
    {
        bool allowed_surf[__SURFACE_COUNT][__SURFACE_COUNT] = {
            // SMOOTH, GLUE,  WELD,  STUDS, INLET, UNIVERSAL, HINGE, MOTOR, STEPPINGMOTOR, UNJOINABLE
            {  false,  false, false, false, false, false,     false, false, false,         false  }, // SMOOTH
            {  true,   true,  true,  true,  true,  true,      false, false, false,         false  }, // GLUE
            {  true,   true,  true,  true,  true,  true,      false, false, false,         false  }, // WELD
            {  false,  false, false, false, true,  true,      false, false, false,         false  }, // STUDS
            {  false,  false, false, true,  false, true,      false, false, false,         false  }, // INLET
            {  false,  false, true,  true,  true,  true,      false, false, false,         false  }, // UNIVERSAL
            {  true,   true,  true,  true,  true,  true,      false, false, false,         false  }, // HINGE
            {  true,   true,  true,  true,  true,  true,      false, false, false,         false  }, // MOTOR
            {  true,   true,  true,  true,  true,  true,      false, false, false,         false  }, // STEPPINGMOTOR
            {  false,  false, false, false, false, false,     false, false, false,         false  }, // UNJOINABLE
        };

        return allowed_surf[type][other.type];
    }

    Ogre::Vector3 PartSurfaceInfo::position()
    {
        Ogre::Vector3 pos;
        switch(face)
        {
        case NORM_UP:
            pos.y = (plane + part->getPosition().y);
            break;
        case NORM_DOWN:
            pos.y = (plane - part->getPosition().y);
            break;
        case NORM_LEFT:
            pos.x = (plane - part->getPosition().x);
            break;
        case NORM_RIGHT:
            pos.x = (plane + part->getPosition().x);
            break;
        case NORM_FRONT:
            pos.z = (plane + part->getPosition().z);
            break;
        case NORM_BACK:
            pos.z = (plane - part->getPosition().z);
            break;
        }
        return pos;
    }

    PartInstance::PartInstance() : m_matrix(), PVInstance(),  m_size(2.f, STUD_HEIGHT, 4.f)
    {
        setName("Part");

        m_color = Ogre::Vector4(0.63, 0.64, 0.63, 1.0);
        m_transparency = 0.0;
        m_reflectance = 0.0;

        setNode(world->getOgreSceneManager()->getRootSceneNode()->createChildSceneNode());
        setObject(world->getOgreSceneManager()->createEntity("meshes/Cube.mesh"));
        getNode()->attachObject(getObject());

        for(int i = 0; i < __NORM_COUNT; i++)
        {
            NormalId n = (NormalId)i;
            switch(n)
            {
                case NORM_UP:
                    m_surfaces[i].type = SURFACE_STUDS;
                    break;
                case NORM_DOWN:
                    m_surfaces[i].type = SURFACE_INLET;
                    break;
                default:
                    m_surfaces[i].type = SURFACE_SMOOTH;
                    break;
            }
            m_surfaces[i].surf = 0;
            m_surfaces[i].part = this;
            m_surfaceNode[i] = 0;
        }

        updateMatrix();
    }

    PartInstance::~PartInstance()
    {
        getNode()->removeAndDestroyAllChildren();
    }

    void PartInstance::updateMatrix()
    {
        m_matrix = m_cframe.getMatrix(); 
        m_position = m_cframe.getPosition();

        if(getNode())
        {
            getNode()->setOrientation(Ogre::Quaternion(m_cframe.getRotation()));
            getNode()->setPosition(m_position);
            getNode()->setScale(m_size);
        }

        if(getObject() && dynamic_cast<Ogre::InstancedEntity*>(getObject()))
        {
            Ogre::InstancedEntity* object = (Ogre::InstancedEntity*)getObject();

            object->setOrientation(Ogre::Quaternion(m_cframe.getRotation()));
            object->setPosition(m_position);
            object->setScale(m_size);

            object->updateTransforms();
        }

        /*Ogre::Entity* entity = (Ogre::Entity*)getObject();
        for(auto& subentity : entity->getSubEntities())
        {
            subentity->setMaterial(BrickColor::material(m_brickColor));
            subentity->getMaterial()->setShininess(64);
            subentity->getMaterial()->setLightingEnabled(true);
        }
        entity->setCastShadows(true);*/
    }

    void PartInstance::updateSurfaces()
    {
        for(int i = 0; i < __NORM_COUNT; i++)
        {
            m_surfaces[i].face = (NormalId)i;
            PartSurfaceInfo& surf = m_surfaces[i];
            Ogre::Vector3 size = ((getSize() / 2.f) );
            Ogre::Vector3 pos = getPosition();
            switch(surf.face)
            {
                case NORM_DOWN:
                    surf.size  = Ogre::Vector2(size.x, size.z);
                    surf.start = Ogre::Vector2(pos.x-size.x,pos.z-size.z);
                    surf.end =   Ogre::Vector2(pos.x+size.x,pos.z+size.z);
                    surf.plane = pos.y - size.y;
                    break;
                case NORM_UP:
                    surf.size  = Ogre::Vector2(size.x, size.z);
                    surf.start = Ogre::Vector2(pos.x-size.x,pos.z-size.z);
                    surf.end =   Ogre::Vector2(pos.x+size.x,pos.z+size.z);
                    surf.plane = pos.y + size.y;
                    break;
                case NORM_LEFT:
                    surf.size  = Ogre::Vector2(size.x, size.y);
                    surf.start = Ogre::Vector2(pos.z-size.z,pos.y-size.y);
                    surf.end =   Ogre::Vector2(pos.z+size.z,pos.y+size.y);
                    surf.plane = pos.x - size.x;
                    break;
                case NORM_RIGHT:
                    surf.size  = Ogre::Vector2(size.x, size.y);
                    surf.start = Ogre::Vector2(pos.z-size.z,pos.y-size.y);
                    surf.end =   Ogre::Vector2(pos.z+size.z,pos.y+size.y);
                    surf.plane = pos.x + size.x;
                    break;
                case NORM_FRONT:
                    surf.size  = Ogre::Vector2(size.z, size.y);
                    surf.start = Ogre::Vector2(pos.x-size.x,pos.y-size.y);
                    surf.end =   Ogre::Vector2(pos.x+size.x,pos.y+size.y);
                    surf.plane = pos.z + size.z;
                    break;
                case NORM_BACK:
                    surf.size  = Ogre::Vector2(size.z, size.y);
                    surf.start = Ogre::Vector2(pos.x-size.x,pos.y-size.y);
                    surf.end =   Ogre::Vector2(pos.x+size.x,pos.y+size.y);
                    surf.plane = pos.z - size.z;
                    break;
            }
            if(surf.surf)
            {
                surf.surf->detachFromParent();
                delete surf.surf;
            }
            Ogre::SceneNode* surfaceNode = m_surfaceNode[i];
            if(!surfaceNode)
            {
                surfaceNode = getNode()->createChildSceneNode();
                m_surfaceNode[i] = surfaceNode;
            }  
            Ogre::Vector3 f = normalIdToVector3(surf.face);
            switch(surf.type)
            {
            case SURFACE_STUDS:
                surf.surf = world->getOgreSceneManager()->createEntity("meshes/Stud.mesh");
                surfaceNode->attachObject(surf.surf);
                surfaceNode->setDirection(f, Ogre::Node::TS_PARENT, Ogre::Vector3::UNIT_Y);
                surfaceNode->setPosition(surf.position());
                surfaceNode->setScale(Ogre::Vector3(surf.size.x, surf.size.y, 1));
                break;
            default:
                break;
            }
        }

    }

    void PartInstance::deserializeProperty(char* prop_name, pugi::xml_node node)
    {
        if(prop_name == std::string("size"))
        {
            setSize(XML::getVector3(node));
        }
        else if(prop_name == std::string("BrickColor"))
        {
            setBrickColor(node.text().as_int());
            if(!BrickColor::valid(getBrickColor()))
            {
                printf("PartInstance::deserializeProperty: BrickColor not valid number (%i)\n", getBrickColor());
            }
        }
        else if(prop_name == std::string("Reflectance"))
        {
            setReflectance(node.text().as_float());
        }
        else if(prop_name == std::string("Transparency"))
        {
            setTransparency(node.text().as_float());
        }
        else if(prop_name == std::string("Anchored"))
        {
            setAnchored(node.text().as_bool());
        }
        else
            PVInstance::deserializeProperty(prop_name, node);
        updateMatrix();
    }

    void PartInstance::addProperties(std::vector<ReflectionProperty>& properties)
    {
        ReflectionProperty _properties[]  = {
            { this, std::string("Size"), std::string(""), 
              ACCESS_NONE, OPERATION_READWRITE, PROPERTY_VECTOR3,         
              REFLECTION_GETTER(PartInstance* instance = (PartInstance*)object; return &instance->m_size; ), 
              REFLECTION_SETTER(PartInstance* instance = (PartInstance*)object; instance->setSize(*(Ogre::Vector3*)value); ) },
            { this, std::string("BrickColor"), std::string(""), 
              ACCESS_NONE, OPERATION_READWRITE, PROPERTY_BRICKCOLOR,         
              REFLECTION_GETTER(PartInstance* instance = (PartInstance*)object; return &instance->m_brickColor; ), 
              REFLECTION_SETTER(PartInstance* instance = (PartInstance*)object; instance->setBrickColor(*(int*)value); ) },
            { this, std::string("Reflectance"), std::string(""), 
              ACCESS_NONE, OPERATION_READWRITE, PROPERTY_FLOAT,         
              REFLECTION_GETTER(PartInstance* instance = (PartInstance*)object; return &instance->m_reflectance; ), 
              REFLECTION_SETTER(PartInstance* instance = (PartInstance*)object; instance->setReflectance(*(float*)value); ) },
            { this, std::string("Transparency"), std::string(""), 
              ACCESS_NONE, OPERATION_READWRITE, PROPERTY_FLOAT,         
              REFLECTION_GETTER(PartInstance* instance = (PartInstance*)object; return &instance->m_transparency; ), 
              REFLECTION_SETTER(PartInstance* instance = (PartInstance*)object; instance->setTransparency(*(float*)value); ) },
        };

        PVInstance::addProperties(properties);
        properties.insert(properties.end(), _properties, _properties+(sizeof(_properties)/sizeof(ReflectionProperty)));
    }
}