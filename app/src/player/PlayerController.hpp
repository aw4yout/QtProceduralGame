#pragma once
#include <QObject>
#include <QVector3D>
#include <QtQml/qqmlregistration.h>

namespace app {

class InputHandler;

class PlayerController : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(InputHandler* inputHandler READ inputHandler WRITE setInputHandler)
    Q_PROPERTY(QVector3D movement READ movement NOTIFY inputChanged)
    Q_PROPERTY(QVector3D cameraRotation READ cameraRotation NOTIFY cameraRotationChanged)
    Q_PROPERTY(bool onGround READ onGround WRITE setOnGround)
    Q_PROPERTY(bool moving READ moving NOTIFY movingChanged)
    Q_PROPERTY(bool running READ running NOTIFY runningChanged)
    Q_PROPERTY(bool shooting READ shooting NOTIFY shootingChanged)
public:
    enum InputState
    {
        None = 0x0, Forward = 0x1, Backward = 0x2, Left = 0x4, Right = 0x8,
        Jump = 0x10, Sprint = 0x20, Shoot = 0x40
    };
    Q_DECLARE_FLAGS(InputStates, InputState)

    explicit PlayerController(QObject* parent = nullptr);

    InputHandler* inputHandler() const { return m_inputHandler; }
    void setInputHandler(InputHandler* inputHandler);

    QVector3D movement();
    QVector3D cameraRotation() const { return m_cameraRotation; }

    bool onGround() const { return m_onGround; }
    void setOnGround(const bool onGround)
    {
        m_onGround = onGround;
        emit inputChanged();
    }

    bool moving() const { return m_moving; }
    bool running() const { return m_inputState.testFlag(Sprint); }
    bool shooting() const { return m_inputState.testFlag(Shoot); }

signals:
    void inputChanged();
    void cameraRotationChanged();
    void movingChanged();
    void runningChanged();
    void shootingChanged();

private slots:
    void onKeyPressed(int key);
    void onKeyReleased(int key);
    void onMouseButtonPressed(int button);
    void onMouseButtonReleased(int button);
    void onMouseMoved(qreal dx, qreal dy);

private:
    static InputState keyToFlag(Qt::Key key);
    static InputState buttonToFlag(Qt::MouseButton button);

private:
    InputHandler* m_inputHandler{};
    InputStates m_inputState;

    QVector3D m_cameraRotation;

private:
    float m_speed{ 500 }, m_sprintMultiplier{ 2 };
    float m_jumpForce{ 8000 };

    bool m_onGround{}, m_moving{};
};

Q_DECLARE_OPERATORS_FOR_FLAGS(PlayerController::InputStates)

} // app
