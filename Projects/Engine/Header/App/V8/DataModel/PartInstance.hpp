#pragma once
#include <App/V8/Tree/PVInstance.hpp>
#include <App/BrickColor.hpp>
#include <Helpers/NormalId.hpp>
#include <OGRE/Ogre.h>

#define STUD_HEIGHT 1.18

namespace RNR
{
    enum PartSurfaceType
    {
        SURFACE_SMOOTH,
        SURFACE_GLUE,
        SURFACE_WELD,
        SURFACE_STUDS,
        SURFACE_INLET,
        SURFACE_UNIVERSAL,
        SURFACE_HINGE,
        SURFACE_MOTOR,
        SURFACE_STEPPINGMOTOR,
        SURFACE_UNJOINABLE,
        __SURFACE_COUNT,
    };

    class PartInstance;

    struct PartSurfaceInfo
    {
    private:
        friend class PartInstance;

        Ogre::Entity* surf; 
        PartInstance* part;
    public:
        NormalId face;
        PartSurfaceType type;

        Ogre::Vector2 start;
        Ogre::Vector2 end;
        Ogre::Vector2 size;
        float plane;

        bool intersects(PartSurfaceInfo other);
        bool links(PartSurfaceInfo other);

        Ogre::Vector3 position(); // relative to center of part
    };

    class PartInstance : public PVInstance
    {
    protected:
        int m_brickColor;
        float m_transparency;
        float m_reflectance;
        bool m_anchored;
        PartSurfaceInfo m_surfaces[__NORM_COUNT];
        Ogre::SceneNode *m_surfaceNode[__NORM_COUNT];
        Ogre::Matrix4 m_matrix;
        Ogre::Vector3 m_position;
        Ogre::Vector3 m_size;
        Ogre::Vector4 m_color;
        virtual void deserializeProperty(char* prop_name, pugi::xml_node prop);
        virtual void addProperties(std::vector<ReflectionProperty>& properties);
        virtual void addFunctions(std::vector<ReflectionFunction>& functions);
        void uploadInstancedProperties();
        std::string mesh_id;
    public:
        PartInstance();
        virtual void lateInit();
        virtual void lateDeInit();
        ~PartInstance();

        void updateMatrix();
        PartSurfaceInfo& getSurface(NormalId normal) { return m_surfaces[normal]; };
        void setSurface(NormalId normal, PartSurfaceInfo surface) { m_surfaces[normal] = surface; }

        virtual std::string getClassName() { return "Part"; }
        void setSize(Ogre::Vector3 size) { m_size = size; updateSurfaces(); }
        Ogre::Vector3 getSize() { return m_size; }
        Ogre::Vector4 getColor() { return m_color; }

        void setReflectance(float reflectance) { m_reflectance = reflectance; uploadInstancedProperties(); }
        float getReflectance() { return m_reflectance; }
        void setTransparency(float transparency) { m_transparency = transparency; uploadInstancedProperties(); }
        float getTransparency() { return m_transparency; }
        void setAnchored(bool anchored) { m_anchored = anchored; }
        bool getAnchored() { return m_anchored; }

        void makeJoints();
        void breakJoints();
        void updateSurfaces();
        //std::vector<btVector3>& getPartCollisions() { return m_partCollisions; }

        Ogre::Vector3 getOgreCenter() { return m_position + (m_size / 2.f); }

        void setBrickColor(int brickcolor) { m_brickColor = brickcolor; uploadInstancedProperties(); }
        int getBrickColor() { return m_brickColor; }
    };
}