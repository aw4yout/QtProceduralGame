#include "InputHandler.hpp"

#include <QQuickWindow>

namespace app {

InputHandler::~InputHandler()
{
    for (auto* target : m_targets) {
        target->removeEventFilter(this);
    }
}

void InputHandler::addTarget(QObject* target)
{
    target->installEventFilter(this);
    m_targets.append(target);
}

void InputHandler::setMouseGrabbed(const bool grabbed)
{
    if (m_mouseGrabbed != grabbed) {
        m_mouseGrabbed = grabbed;
        QGuiApplication::setOverrideCursor({ m_mouseGrabbed ? Qt::BlankCursor : Qt::ArrowCursor });
        emit mouseGrabbedChanged();
    }
}

void InputHandler::setMouseSensitivity(const qreal mouseSensitivity)
{
    if (m_mouseSensitivity != mouseSensitivity) {
        m_mouseSensitivity = mouseSensitivity;
        emit mouseSensitivityChanged();
    }
}

bool InputHandler::eventFilter(QObject* watched, QEvent* event)
{
    if (m_targets.contains(watched)) {
        if (event->type() == QEvent::KeyPress) {
            emit keyPressed(dynamic_cast<QKeyEvent*>(event)->key());
        } else if (event->type() == QEvent::KeyRelease) {
            emit keyReleased(dynamic_cast<QKeyEvent*>(event)->key());
        } else if (event->type() == QEvent::MouseButtonPress) {
            emit mouseButtonPressed(dynamic_cast<QMouseEvent*>(event)->button());
        } else if (event->type() == QEvent::MouseButtonRelease) {
            emit mouseButtonReleased(dynamic_cast<QMouseEvent*>(event)->button());
        } else if (event->type() == QEvent::MouseMove && m_mouseGrabbed) {
            const auto* mouseEvent = dynamic_cast<QMouseEvent*>(event);

            const auto dx = (mouseEvent->scenePosition().x() - m_previousMousePosition.x()) * m_mouseSensitivity;
            const auto dy = (mouseEvent->scenePosition().y() - m_previousMousePosition.y()) * m_mouseSensitivity * m_aspectRatio;

            QCursor::setPos(
                static_cast<int>(m_viewportSize.x() / 2),
                static_cast<int>(m_viewportSize.y() / 2)
            );

            m_previousMousePosition = { m_viewportSize.x() / 2, m_viewportSize.y() / 2 };

            emit mouseMoved(dx, dy);
        } else {
            return false;
        }
        return true;
    }
    return false;
}

} // app
