#pragma once

#include <OGRE/Ogre.h>

namespace RNR {
    enum NormalId
    {
        NORM_X, NORM_RIGHT = NORM_X,
        NORM_Y, NORM_UP = NORM_Y,
        NORM_Z, NORM_BACK = NORM_Z,
        NORM_X_NEG, NORM_LEFT = NORM_X_NEG,
        NORM_Y_NEG, NORM_DOWN = NORM_Y_NEG,
        NORM_Z_NEG, NORM_FRONT = NORM_Z_NEG,
        __NORM_COUNT,
        NORM_UNDEFINED,
    };

    bool validNormalId(NormalId normalId);

    NormalId normalIdOpposite(NormalId normalId);
	NormalId normalIdToU(NormalId normalId);
	NormalId normalIdToV(NormalId normalId);

    const Ogre::Vector3& normalIdToVector3(NormalId normalId);
	const Ogre::Matrix3& normalIdToMatrix3(NormalId normalId);

    NormalId Vector3ToNormalId(const Ogre::Vector3& vector);
	NormalId Matrix3ToNormalId(const Ogre::Matrix3& matrix);
	NormalId intToNormalId(int i);

    Ogre::Vector3 uvwToObject(const Ogre::Vector3& uvw, NormalId faceId);
    Ogre::Vector3 objectToUvw(const Ogre::Vector3& object, NormalId faceId);

    template<NormalId faceId>
    Ogre::Vector3 uvwToObject(const Ogre::Vector3& vector);
    template<NormalId faceId>
    Ogre::Vector3 objectToUvw(const Ogre::Vector3& vector);
}