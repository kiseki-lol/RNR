#pragma once

#include <App/V8/Tree/Instance.hpp>
#include <App/V8/Tree/PVInstance.hpp>
#include <App/CoordinateFrame.hpp>
#include <OGRE/Ogre.h>

namespace RNR
{
    class World;

    enum CameraType
    {
        CAMERA_FOLLOW,
    };

    class Camera : public Instance
    {
        private:
            CoordinateFrame m_cframe;
            CoordinateFrame m_focus;
            PVInstance* m_cameraSubject;
            Ogre::Matrix3 m_lookMat;
            CameraType m_type;
            Ogre::Radian m_yaw;
            Ogre::Radian m_pitch;
            virtual void deserializeProperty(char* prop_name, pugi::xml_node prop);
            virtual void addProperties(std::vector<ReflectionProperty>& properties);
        public:
            Camera();
            ~Camera();

            void cameraFrame(float xd, float yd, bool movement_disable = true);

            virtual std::string getClassName() { return "Camera"; }
            CoordinateFrame& getCFrame() { return m_cframe; };
            CoordinateFrame& getFocus() { return m_focus; }
            Ogre::Radian getYaw() { return m_yaw; }
            Ogre::Radian getPitch() { return m_pitch; }
            void setCFrame(CoordinateFrame cframe) { m_cframe = cframe; };
            void setFocus(CoordinateFrame focus) { m_focus = focus; }
            bool zoom(float distance);

            void setSubject(PVInstance* subject) { m_cameraSubject = subject; }
            PVInstance* getSubject() { return m_cameraSubject; }
    };
}