#include <Helpers/NormalId.hpp>

#include <OGRE/Ogre.h>

namespace RNR {
    static Ogre::Vector3 normalIdVectors[] = {
        Ogre::Vector3(1, 0, 0),
        Ogre::Vector3(0, 1, 0),
        Ogre::Vector3(0, 0, 1),
        Ogre::Vector3(-1, 0, 0),
        Ogre::Vector3(0, -1, 0),
        Ogre::Vector3(0, 0, -1),
    };

    bool validNormalId(NormalId normalId) {
        return ((normalId >= 0) && (normalId < 6)); 
    }

    NormalId intToNormalId(int i)
    {
        return static_cast<NormalId>(i);
    }

    NormalId normalIdOpposite(NormalId normalId)
    {
        return (NormalId)((normalId + 3) % 6);
    }

    NormalId normalIdToU(NormalId normalId)
    {
        switch (normalId)
        {
            case NORM_X:
                return NORM_Z;
            case NORM_Y:
                return NORM_X;
            case NORM_Z:
                return NORM_Y;
            case NORM_X_NEG:
                return NORM_Z_NEG;
            case NORM_Y_NEG:
                return NORM_X_NEG;
            case NORM_Z_NEG:
                return NORM_Y_NEG;
            default:
                return NORM_Y;
        }
    }

    NormalId normalIdToV(NormalId normalId)
    {
        switch (normalId)
        {
            case NORM_X:
                return NORM_Y;
            case NORM_Y:
                return NORM_Z;
            case NORM_Z:
                return NORM_X;
            case NORM_X_NEG:
                return NORM_Y_NEG;
            case NORM_Y_NEG:
                return NORM_Z_NEG;
            case NORM_Z_NEG:
                return NORM_X_NEG;
            default:
                return NORM_Y;
        }
    }

    const Ogre::Vector3& normalIdToVector3(NormalId id)
    {
        return normalIdVectors[id];
    }


    // UVW to Object
    template<>
    Ogre::Vector3 uvwToObject<NORM_X>(const Ogre::Vector3& vector)
    {
        return Ogre::Vector3(vector.z, vector.y, -vector.x);
    }

    template<>
    Ogre::Vector3 uvwToObject<NORM_Y>(const Ogre::Vector3& vector)
    {
        return Ogre::Vector3(-vector.x, vector.z, vector.y);
    }

    template<>
    Ogre::Vector3 uvwToObject<NORM_Z>(const Ogre::Vector3& vector)
    {
        return Ogre::Vector3(vector.x, vector.y, vector.z);
    }

    template<>
    Ogre::Vector3 uvwToObject<NORM_X_NEG>(const Ogre::Vector3& vector)
    {
        return Ogre::Vector3(-vector.z, vector.y, vector.x);
    }

    template<>
    Ogre::Vector3 uvwToObject<NORM_Y_NEG>(const Ogre::Vector3& vector)
    {
        return Ogre::Vector3(vector.x, -vector.z, vector.y);
    }

    template<>
    Ogre::Vector3 uvwToObject<NORM_Z_NEG>(const Ogre::Vector3& vector)
    {
        return Ogre::Vector3(-vector.x, vector.y, -vector.z);
    }

    // Object to UVW
    template<>
    Ogre::Vector3 objectToUvw<NORM_X>(const Ogre::Vector3& vector) 
    {
        return Ogre::Vector3(-vector.z, vector.y, vector.x);
    }

    template<>
    Ogre::Vector3 objectToUvw<NORM_Y>(const Ogre::Vector3& vector) 
    {
        return Ogre::Vector3(-vector.x, vector.z, vector.y);
    }

    template<>
    Ogre::Vector3 objectToUvw<NORM_Z>(const Ogre::Vector3& vector) 
    {
        return Ogre::Vector3(vector.x, vector.y, vector.z);
    }

    template<>
    Ogre::Vector3 objectToUvw<NORM_X_NEG>(const Ogre::Vector3& vector) 
    {
        return Ogre::Vector3(vector.z, vector.y, -vector.x);
    }

    template<>
    Ogre::Vector3 objectToUvw<NORM_Y_NEG>(const Ogre::Vector3& vector) 
    {
        return Ogre::Vector3(vector.x, vector.z, -vector.y);
    }

    template<>
    Ogre::Vector3 objectToUvw<NORM_Z_NEG>(const Ogre::Vector3& vector) 
    {
        return Ogre::Vector3(-vector.x, vector.y, -vector.z);
    }

    Ogre::Vector3 uvwToObject(const Ogre::Vector3& uvw, NormalId faceId)
    {
        switch (faceId)
        {
        case NORM_X:
            return uvwToObject<NORM_X>(uvw);
        case NORM_Y:
            return uvwToObject<NORM_Y>(uvw);
        case NORM_Z:
            return uvwToObject<NORM_Z>(uvw);
        case NORM_X_NEG:
            return uvwToObject<NORM_X_NEG>(uvw);
        case NORM_Y_NEG:
            return uvwToObject<NORM_Y_NEG>(uvw);
        case NORM_Z_NEG:
            return uvwToObject<NORM_Z_NEG>(uvw);
        default:
            return Ogre::Vector3::UNIT_X;
        }
    }
 
    Ogre::Vector3 objectToUvw(const Ogre::Vector3& object, NormalId faceId)
    {
        switch (faceId)
        {
        case NORM_X:	
            return objectToUvw<NORM_X>(object);
        case NORM_Y:
            return objectToUvw<NORM_Y>(object);
        case NORM_Z:
            return objectToUvw<NORM_Z>(object);
        case NORM_X_NEG:
            return objectToUvw<NORM_X_NEG>(object);
        case NORM_Y_NEG:
            return objectToUvw<NORM_Y_NEG>(object);
        case NORM_Z_NEG:
            return objectToUvw<NORM_Z_NEG>(object);
        default:
            return Ogre::Vector3::UNIT_X;
        }
    }
};