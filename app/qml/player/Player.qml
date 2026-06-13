import QtQuick

import ProceduralGame

PlayerController {
    id: playerController

    required property var mouseArea
    required property var appWindow

    inputHandler: InputHandler {
        viewportSize: Qt.vector2d(playerController.appWindow.width, playerController.appWindow.height)
        mouseSensitivity: 0.05
        mouseGrabbed: true

        Component.onCompleted: {
            addTarget(playerController.mouseArea)
            addTarget(playerController.appWindow)
        }
    }
}
