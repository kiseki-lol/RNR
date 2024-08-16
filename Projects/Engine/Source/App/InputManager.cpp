#include <App/InputManager.hpp>
#include <App/V8/DataModel/Camera.hpp>
#include <App/Humanoid/Humanoid.hpp>
#include <stdio.h>

namespace RNR
{
    IInputManager::IInputManager()
    {
        state.mouse_primary = false;
        state.mouse_secondary = false;
        state.mouse_middle = false;
        state.mouse_scroll = 0;    }

    void IInputManager::keyDown(int scancode)
    {
        auto it = std::find(scancodes_down.begin(), scancodes_down.end(), scancode);
        if(it == scancodes_down.end())
            scancodes_down.push_back(scancode);
    }

    void IInputManager::keyUp(int scancode)
    {
        auto it = std::find(scancodes_down.begin(), scancodes_down.end(), scancode);
        if(it != scancodes_down.end())
            scancodes_down.erase(it);
    }

    bool IInputManager::isKeyDown(int scancode)
    {
        auto it = std::find(scancodes_down.begin(), scancodes_down.end(), scancode);
        if(it != scancodes_down.end())
            return true;
        return false;
    }

    void IInputManager::mouseMoveAbsolute(float x, float y)
    {

    }

    void IInputManager::mouseMoveRelative(float x, float y)
    {
        if(m_world)
        {
            m_mouseDX = x * m_world->getLastDelta();
            m_mouseDY = y * m_world->getLastDelta();

            Workspace* workspace = m_world->getWorkspace();
            Camera* camera = workspace->getCurrentCamera();
            if(camera && state.mouse_secondary)
            {
                camera->cameraFrame(m_mouseDX, m_mouseDY);

                resetMouse();
            }
        }
    }

    void IInputManager::frame()
    {
        Players* players = (Players*)m_world->getDatamodel()->getService("Players");
        Player* localPlayer = players->getLocalPlayer();
        if(localPlayer)
        {
            ModelInstance* character = localPlayer->getCharacter();
            if(character)
            {
                Humanoid* humanoid = (Humanoid*)character->findFirstChildOfType("Humanoid");
                if(humanoid)
                {
                    humanoid->inputFrame(m_mouseDX, m_mouseDY);
                }
            }
        }
        else
        {
            Workspace* workspace = m_world->getWorkspace();
            Camera* camera = workspace->getCurrentCamera();
            if(camera) 
                camera->cameraFrame(0, 0, false); // update camera position
        }
        m_mouseDX = 0;
        m_mouseDY = 0;
    }

    void IInputManager::mousePrimaryState(bool down)
    {
        state.mouse_primary = down;
    }

    void IInputManager::mouseSecondaryState(bool down)
    {
        if(down)
            grab();
        else if(!down)
            ungrab();
        state.mouse_secondary = down;
    }

    void IInputManager::mouseMiddleState(bool down)
    {
        state.mouse_middle = down;
    }

    void IInputManager::mouseScrollState(float scroll)
    {
        state.mouse_scroll = scroll;
    }
}