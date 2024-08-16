#include <Network/Player.hpp>
#include <App/Humanoid/Humanoid.hpp>
#include <App/V8/World/World.hpp>

namespace RNR
{
    Player::Player()
    {
        setName("Player");

        m_character = NULL;
    }

    void Player::addProperties(std::vector<ReflectionProperty> &properties)
    {
        ReflectionProperty _properties[] = {
            {this, std::string("Character"), std::string(""),
             ACCESS_NONE, OPERATION_READWRITE, PROPERTY_INSTANCE,
             REFLECTION_GETTER(Player *instance = (Player *)object; return instance->m_character;),
             REFLECTION_SETTER(Player *instance = (Player *)object; instance->setCharacter((ModelInstance *)value);)},
        };

        properties.insert(properties.end(), _properties, _properties + (sizeof(_properties) / sizeof(ReflectionProperty)));
    }

    void Player::initLocalPlayer()
    {
    }

    void Player::loadCharacter()
    {
        m_character = new ModelInstance();
        m_character->setName(getName());

        PartInstance *head = new PartInstance();
        head->setName("Head");
        head->setSize(Ogre::Vector3(2, 1, 1));
        head->setBrickColor(24);
        head->getCFrame().setPosition(Ogre::Vector3(0, 1.5, 0));
        head->updateMatrix();
        head->setParent(m_character);

        PartInstance *torso = new PartInstance();
        torso->setName("Torso");
        torso->setSize(Ogre::Vector3(2, 2, 1));
        torso->setBrickColor(23);
        torso->getCFrame().setPosition(Ogre::Vector3(0, 0, 0));
        torso->updateMatrix();
        torso->setParent(m_character);
        torso->setAnchored(true);

        Humanoid *character_humanoid = new Humanoid();
        character_humanoid->setParent(m_character);
        m_character->makeJoints();
        character_humanoid->buildJoints();

        m_character->setParent(world->getWorkspace());

        Camera *player_camera = world->getWorkspace()->getCurrentCamera();
        player_camera->setCFrame(CoordinateFrame());
    }
}
