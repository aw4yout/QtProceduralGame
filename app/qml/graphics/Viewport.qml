import QtQuick
import QtQuick3D
import QtQuick3D.Helpers
import QtQuick3D.Physics

View3D {
    id: view3d

    PhysicsWorld {
        id: physicsWorld

        gravity: Qt.vector3d(0, -982, 0)
        scene: view3d.scene
        running: false
    }

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

        position: Qt.vector3d(1000, 1000, 1200)

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

    DynamicRigidBody {
        collisionShapes: BoxShape{}
        position: Qt.vector3d(1000, 1200, 1000)
        Model {
            source: "#Cube"
            materials: [ PrincipledMaterial {
                baseColor: "red"
            } ]
        }
    }

    Repeater3D {
        model: chunks
        delegate: StaticRigidBody {
            id: body

            required property var modelData
            required property int index

            scale: Qt.vector3d(50, 50, 50)
            collisionShapes: TriangleMeshShape {
                geometry: (body.modelData && body.modelData.vertexCount > 0) ? body.modelData : null
            }

            Model {
                id: model

                geometry: body.modelData
                materials: [ PrincipledMaterial {
                    baseColor: "green"
                    roughness: 0.8
                    metalness: 0.0
                } ]
            }
        }

        Component.onCompleted: physicsWorld.running = true
    }
}
