import QtQuick
import QtQuick3D
import QtQuick3D.Helpers

View3D {
    camera: perspectiveCamera
    environment: ExtendedSceneEnvironment {
        backgroundMode: SceneEnvironment.SkyBox
        aoEnabled: true
        aoDistance: 100
        aoDither: true
        aoSampleRate: 4
        aoSoftness: 50
        lightProbe: Texture {
            textureData: ProceduralSkyTextureData {}
        }
    }

    Node {
        id: cameraNode

        PerspectiveCamera {
            id: perspectiveCamera

            fieldOfView: 90
            clipFar: 30000
            frustumCullingEnabled: true
        }
    }

    WasdController {
        controlledObject: cameraNode
        smooth: true
    }

    DirectionalLight {
        eulerRotation.x: -70
        castsShadow: true
        shadowMapFar: 30000
        shadowMapQuality: Light.ShadowMapQualityUltra
        softShadowQuality: Light.PCF16
        use32BitShadowmap: true
        brightness: 1.0
        shadowFactor: 90
    }

    Model {
        source: "#Cube"
        position: Qt.vector3d(0, 0, -200)
        materials: [ PrincipledMaterial {
            baseColor: "red"
        } ]
    }

    Repeater3D {
        model: chunks
        delegate: Model {
            required property var modelData

            geometry: modelData
            scale: Qt.vector3d(50, 50, 50)
            materials: [ PrincipledMaterial {
                baseColor: "green"
                roughness: 0.8
                metalness: 0.0
                //cullMode: Material.NoCulling
            } ]
        }
    }

    // Model {
    //     geometry: terrainGeometry
    //     position: Qt.vector3d(0, -500, -400)
    //     scale: Qt.vector3d(20, 20, 20)
    //     materials: [ PrincipledMaterial {
    //         baseColor: "green"
    //         roughness: 0.8
    //         metalness: 0.0
    //         cullMode: Material.NoCulling
    //     } ]
    // }
}
