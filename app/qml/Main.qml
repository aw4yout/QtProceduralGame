import QtQuick
import QtQuick.Controls
import QtQuick3D.Helpers

import "graphics"

ApplicationWindow {
    width: 1920
    height: 1080

    visible: true
    title: qsTr("Game")

    visibility: Window.FullScreen

    Viewport {
        id: viewport
        anchors.fill: parent
    }

    DebugView {
        source: viewport
        resourceDetailsVisible: true
        visible: false
    }
}
