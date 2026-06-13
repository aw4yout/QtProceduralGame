#pragma once
#include <QObject>
#include <QtQml/qqmlregistration.h>
#include <QVector2D>

namespace app {

class InputHandler : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(bool mouseGrabbed READ mouseGrabbed WRITE setMouseGrabbed NOTIFY mouseGrabbedChanged)
    Q_PROPERTY(qreal mouseSensitivity READ mouseSensitivity WRITE setMouseSensitivity NOTIFY mouseSensitivityChanged)
    Q_PROPERTY(QVector2D viewportSize READ viewportSize WRITE setTargetCenter)
public:
    explicit InputHandler(QObject* parent = nullptr) : QObject(parent) {}
    ~InputHandler() override;

    Q_INVOKABLE void addTarget(QObject* target);

    bool mouseGrabbed() const { return m_mouseGrabbed; }
    void setMouseGrabbed(bool grabbed);

    qreal mouseSensitivity() const { return m_mouseSensitivity; }
    void setMouseSensitivity(qreal mouseSensitivity);

    QVector2D viewportSize() const { return m_viewportSize; }
    void setTargetCenter(const QVector2D& viewportSize)
    {
        m_viewportSize = viewportSize;
        m_aspectRatio = viewportSize.x() / viewportSize.y();
    }

    qreal aspectRatio() const { return m_aspectRatio; }

    bool eventFilter(QObject* watched, QEvent* event) override;

signals:
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseButtonPressed(int button);
    void mouseButtonReleased(int button);
    void mouseMoved(qreal dx, qreal dy);
    void mouseGrabbedChanged();
    void mouseSensitivityChanged();

private:
    QList<QObject*> m_targets;
    QVector2D m_viewportSize{};
    QPointF m_previousMousePosition{};
    bool m_mouseGrabbed{};
    qreal m_mouseSensitivity{ 1.0 };
    qreal m_aspectRatio{};
};

} // app
