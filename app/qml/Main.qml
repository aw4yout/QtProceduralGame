import QtQuick
import QtQuick.Controls
import QtQuick3D.Helpers

import "graphics"
import "player"

ApplicationWindow {
    id: appWindow

    width: 1920
    height: 1080

    visible: true
    title: qsTr("Game")

    Scene {
        id: scene
        anchors.fill: parent

        player: player
    }

    Player {
        id: player

        mouseArea: mouseArea
        appWindow: appWindow
        onGround: scene.character.onGround
    }

    MouseArea {
        id: mouseArea

        anchors.fill: parent

        hoverEnabled: true
        propagateComposedEvents: true
    }

    DebugView {
        source: scene
        resourceDetailsVisible: true
        visible: false
    }

    Text {
        text: `position: ${scene.character.position}`
        font.pixelSize: 24
        color: "white"
    }
}
