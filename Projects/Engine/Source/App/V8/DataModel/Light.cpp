#include <App/V8/DataModel/Light.hpp>
#include <App/V8/DataModel/PartInstance.hpp>
#include <App/V8/World/World.hpp>

namespace RNR
{
    Light::Light()
    {
        m_brightness = 1.f;
        m_enabled = true;
        m_shadows = true;
        m_color = Ogre::Vector3(75.f/255.f,151.f/255.f,75.f/255.f);       

        setNode(world->getOgreSceneManager()->getRootSceneNode()->createChildSceneNode());
        setObject(world->getOgreSceneManager()->createLight(Ogre::Light::LT_POINT));
        getNode()->attachObject(getObject());

        Ogre::Light* light_object = (Ogre::Light*)getObject();
    }

    void Light::onSetParent(Instance* newParent)
    {
        Ogre::Light* light_object = (Ogre::Light*)getObject();

        if(!newParent)
        {
            light_object->setVisible(false);
            return;
        }

        PartInstance* part = dynamic_cast<PartInstance*>(newParent);
        if(!part)
        {
            light_object->setVisible(false);
            return;
        }

        getNode()->setPosition(part->getPosition());
        setupLight(light_object);
    }

    void Light::setupLight(Ogre::Light* light)
    {
        light->setVisible(m_enabled);
        light->setCastShadows(m_shadows);
        light->setDiffuseColour(Ogre::ColourValue(m_color.x, m_color.y, m_color.z));
        //light->setAttenuation(100, 1.0, 0.045, 0.0075);
        light->setRenderingDistance(500.f);
        light->setShadowFarDistance(100);
    }
}