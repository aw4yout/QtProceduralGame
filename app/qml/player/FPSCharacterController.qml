import QtQuick
import QtQuick3D.Physics

CharacterController {
    id: character

    readonly property bool onGround: (collisions & CharacterController.Down)

    collisionShapes: CapsuleShape {
        diameter: 50
        height: 130
    }

    Behavior on movement {
        PropertyAnimation { duration: 200 }
    }
}
