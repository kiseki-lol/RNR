#include <App/V8/DataModel/Camera.hpp>
#include <App/V8/World/World.hpp>
#include <App/InputManager.hpp>
#include <Network/Players.hpp>
#include <Helpers/XML.hpp>

namespace RNR
{
    Camera::Camera()
    {
        setName("Camera");
        m_type = CAMERA_FOLLOW;
        m_lookMat = Ogre::Matrix3::IDENTITY;
    }

    Camera::~Camera()
    {
        //
    }

    void Camera::deserializeProperty(char* prop_name, pugi::xml_node node)
    {
        if(prop_name == std::string("CoordinateFrame"))
        {
            setCFrame(XML::getCFrame(node));
        }
        else if(prop_name == std::string("Focus"))
        {
            setFocus(XML::getCFrame(node));
        }
    }

    void Camera::cameraFrame(float xd, float yd, bool movement_disable)
    {
        if(!world)
            return;
            
        Ogre::Radian pitch = Ogre::Radian(yd);
        Ogre::Radian yaw = Ogre::Radian(xd);
        Ogre::Radian old_pitch;
        Ogre::Radian old_yaw;
        Ogre::Radian old_roll;

        Ogre::Matrix3 rotation = getCFrame().getRotation();

        Players* players = (Players*)world->getDatamodel()->getService("Players");

        if(players->getLocalPlayer())
        {
            ModelInstance* character = players->getLocalPlayer()->getCharacter();
            if(character)
            {
                PartInstance* head = dynamic_cast<PartInstance*>(character->findFirstChild("Head"));
                if(head)
                {
                    switch(m_type)
                    {
                    case CAMERA_FOLLOW:
                        m_focus.setPosition(head->getPosition());
                        m_lookMat.ToEulerAnglesYXZ(old_yaw, old_pitch, old_roll);
                        pitch = old_pitch + pitch;
                        yaw = old_yaw + yaw;
                        m_yaw = yaw;
                        m_pitch = pitch;
                        m_lookMat.FromEulerAnglesYXZ(yaw, pitch, Ogre::Radian(0));
                        getCFrame().setPosition(Ogre::Vector3(5,0,0) * m_lookMat);
                        getCFrame().lookAt(getFocus().getPosition());
                        break;
                    }
                }
            }
        }
        else
        {
            getCFrame().getRotation().ToEulerAnglesYXZ(old_yaw, old_pitch, old_roll);

            pitch = old_pitch + pitch;
            yaw = old_yaw - yaw;
            m_yaw = yaw;
            m_pitch = pitch;

            rotation.FromEulerAnglesYXZ(yaw, pitch, Ogre::Radian(0));
            getCFrame().setRotation(rotation);
        }

        if(!movement_disable)
        {
            IInputManager* input = world->getInputManager();

            float speed = 50;

            if(input->isKeyDown(0x01000020)) // Scan code for Shift
                speed = speed / 2;

            Ogre::Vector3 position = getCFrame().getPosition();
            Ogre::Vector3 movement = Ogre::Vector3(0, 0, 0);

            if(input->isKeyDown('W'))
                movement.z = -speed;
            else if(input->isKeyDown('S'))
                movement.z = speed;
            
            if(input->isKeyDown('A'))
                movement.x = -speed;
            else if(input->isKeyDown('D'))
                movement.x = speed;
            
            if(input->isKeyDown('E'))
                movement.y = speed;
            else if(input->isKeyDown('Q'))
                movement.y = -speed;

            movement = rotation * movement;
            position += movement * world->getLastDelta();

            getCFrame().setPosition(position);
        }
    }

    void Camera::addProperties(std::vector<ReflectionProperty>& properties)
    {
        ReflectionProperty _properties[]  = {
            { this, std::string("CFrame"), std::string(""), 
              ACCESS_NONE, OPERATION_READWRITE, PROPERTY_CFRAME,         
              REFLECTION_GETTER(Camera* instance = (Camera*)object; return &instance->m_cframe; ), 
              REFLECTION_SETTER(Camera* instance = (Camera*)object; instance->setCFrame(*(CoordinateFrame*)value); ) },
            { this, std::string("Focus"), std::string(""), 
              ACCESS_NONE, OPERATION_READWRITE, PROPERTY_CFRAME,         
              REFLECTION_GETTER(Camera* instance = (Camera*)object; return &instance->m_focus; ), 
              REFLECTION_SETTER(Camera* instance = (Camera*)object; instance->setFocus(*(CoordinateFrame*)value); ) },
        };

        properties.insert(properties.end(), _properties, _properties+(sizeof(_properties)/sizeof(ReflectionProperty)));
    }
}