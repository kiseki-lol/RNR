#include <QtInputManager.hpp>
#include <OgreWidget.hpp>

namespace RNR
{
    QtInputManager::QtInputManager(OgreWidget* widget)
    {
        last_position = QPointF();
        this->widget = widget;
        m_grabbed = false;
    }

    void QtInputManager::keyEvent(QKeyEvent* e)
    {
        if(e->isAutoRepeat())
            return;
        if(e->type() == QEvent::KeyPress)
            keyDown(e->key());
        else if(e->type() == QEvent::KeyRelease)
            keyUp(e->key());
    }

    void QtInputManager::mouseEvent(QMouseEvent* e)
    {
        QPointF position = e->position();

        mouseMoveAbsolute(position.x(), position.y());
        mouseMoveRelative(position.x() - last_position.x(), last_position.y() - position.y());

        if(!reset)
        {
            last_position = position;
            reset = false;
        }

        if(e->type() == QEvent::MouseButtonPress || e->type() == QEvent::MouseButtonRelease)
        {
            bool down = (e->type() == QEvent::MouseButtonPress);
            Qt::MouseButton button = e->button();
            switch(button)
            {
                case Qt::LeftButton:
                    mousePrimaryState(down);
                    break;

                case Qt::RightButton:
                    mouseSecondaryState(down);
                    break;

                case Qt::MiddleButton:
                    mouseMiddleState(down);
                    break;
            }
        }
    }

    void QtInputManager::resetMouse()
    {
        widget->clearFocus();
        if(!m_grabbed)
        {
            QPoint glob = widget->mapToGlobal(QPoint(widget->width()/2,widget->height()/2));
            QCursor::setPos(glob);
            last_position = QPoint(widget->width()/2,widget->height()/2);
        }
        else
        {
            QCursor::setPos(widget->mapToGlobal(grab_position));
            last_position = grab_position;
        }
        reset = true;
        widget->setFocus();
    }

    void QtInputManager::grab()
    {
        grab_position = widget->mapFromGlobal(QCursor::pos());
        last_position = grab_position;
        m_grabbed = true;
    }

    void QtInputManager::ungrab()
    {
        m_grabbed = false;
    }
}