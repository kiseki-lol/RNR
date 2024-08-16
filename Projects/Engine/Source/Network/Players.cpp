#include <Network/Players.hpp>

namespace RNR
{
    Players::Players()
    {
        setName("Players");
        m_localPlayer = 0;
    }
        
    Player* Players::createLocalPlayer(int userId)
    {
        if(m_localPlayer)
        {
            printf("Players::createLocalPlayer: attempt to create another local player %i\n", userId);
            return 0;
        }
        printf("Players::createLocalPlayer: created player %i\n", userId);
        m_localPlayer = new Player();
        m_localPlayer->setParent(this);
        m_localPlayer->initLocalPlayer();
        return m_localPlayer;
    }

    void Players::addProperties(std::vector<ReflectionProperty>& properties)
    {
        ReflectionProperty _properties[]  = {
            { this, std::string("LocalPlayer"), std::string(""), 
              ACCESS_NONE, OPERATION_READ, PROPERTY_INSTANCE,         
              REFLECTION_GETTER(Players* instance = (Players*)object; return instance->m_localPlayer; ), 
              REFLECTION_SETTER(  ) },
        };

        properties.insert(properties.end(), _properties, _properties+(sizeof(_properties)/sizeof(ReflectionProperty)));
    }
}