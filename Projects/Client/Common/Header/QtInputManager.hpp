#pragma once

#include <App/InputManager.hpp>
#include <QMouseEvent>
#include <QKeyEvent>

namespace RNR
{
    class OgreWidget;

    class QtInputManager : public RNR::IInputManager
    {
    private:
        QPointF last_position;
        QPoint grab_position;
        bool m_grabbed;
        OgreWidget* widget;
        bool reset;
    public:
        QtInputManager(OgreWidget* widget);

        void keyEvent(QKeyEvent* e);
        void mouseEvent(QMouseEvent* e);

        virtual void resetMouse();
        virtual void grab();
        virtual void ungrab();
    };
}
