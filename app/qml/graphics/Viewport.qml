import QtQuick
import QtQuick3D
import QtQuick3D.Helpers

View3D {
    camera: perspectiveCamera
    environment: ExtendedSceneEnvironment {
        backgroundMode: SceneEnvironment.SkyBox
        lightProbe: Texture {
            textureData: ProceduralSkyTextureData {}
        }
    }

    Node {
        id: cameraNode

        PerspectiveCamera {
            id: perspectiveCamera

            fieldOfView: 90
        }
    }

    WasdController {
        controlledObject: cameraNode
        smooth: true
    }

    Model {
        source: "#Cube"
        position: Qt.vector3d(0, 0, -200)
        materials: [ PrincipledMaterial {
            baseColor: "red"
        } ]
    }

    Model {
        geometry: terrainGeometry
        position: Qt.vector3d(0, 0, -400)
        materials: [ PrincipledMaterial {
            baseColor: "green"
            roughness: 1.0
            metalness: 0.0
            cullMode: Material.NoCulling
        } ]
    }
}
