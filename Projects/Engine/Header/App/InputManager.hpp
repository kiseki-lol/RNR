#pragma once
#include <App/V8/World/World.hpp>

namespace RNR
{
    struct MouseState
    {
        bool mouse_primary;
        bool mouse_secondary;
        bool mouse_middle;
        float mouse_scroll;
    };

    class IInputManager
    {
        World* m_world;
        std::vector<int> scancodes_down;
        float m_mouseDX;
        float m_mouseDY;
    protected:
        MouseState state;
    public:
        IInputManager();

        virtual void resetMouse() {};
        virtual void grab() {};
        virtual void ungrab() {};

        void setWorld(World* world) { m_world = world; world->setInputManager(this); }

        void frame();

        bool isKeyDown(int scancode);
        void keyDown(int scancode);
        void keyUp(int scancode);

        void mouseMoveRelative(float x, float y);
        void mouseMoveAbsolute(float x, float y);

        void mousePrimaryState(bool down);
        void mouseSecondaryState(bool down);
        void mouseMiddleState(bool down);
        void mouseScrollState(float scroll);
    };
}