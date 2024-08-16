#pragma once

#include <App/V8/Tree/Instance.hpp>
#include <Network/Player.hpp>

namespace RNR
{
    class Players : public Instance
    {
    private:
        Player* m_localPlayer;
    public:
        Players();

        virtual std::string getClassName() { return "Players"; }

        Player* getLocalPlayer() { return m_localPlayer; };
        Player* createLocalPlayer(int userId);
        virtual void addProperties(std::vector<ReflectionProperty>& properties);
    };
}