#pragma once
#include <App/V8/Tree/Instance.hpp>
#include <App/V8/Tree/ModelInstance.hpp>

namespace RNR
{
    class Player : public Instance
    {
        ModelInstance* m_character;
        int m_userId;
    public:
        Player();

        void initLocalPlayer();
        void loadCharacter();
        
        ModelInstance* getCharacter() { return m_character; }
        void setCharacter(ModelInstance* model) { m_character = model; }
        virtual std::string getClassName() { return "Player"; }
        virtual void addProperties(std::vector<ReflectionProperty>& properties);        
    };
}