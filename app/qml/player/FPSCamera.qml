import QtQuick
import QtQuick3D

PerspectiveCamera {
    id: perspectiveCamera

    fieldOfView: 90
    clipFar: 15000.0
    clipNear: 0.1
    frustumCullingEnabled: true
}
