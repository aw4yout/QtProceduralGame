import QtQuick
import QtQuick3D

PerspectiveCamera {
    id: perspectiveCamera

    fieldOfView: 90
    clipFar: 1500.0
    clipNear: 0.1
    frustumCullingEnabled: true
}
