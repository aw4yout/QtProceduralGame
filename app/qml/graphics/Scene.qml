import QtQuick
import QtQuick3D
import QtQuick3D.Physics

import "materials"
import "../player"

View3D {
    id: view3d

    required property var player
    property alias character: character

    PhysicsWorld {
        id: physicsWorld

        gravity: Qt.vector3d(0, -982, 0)
        scene: view3d.scene
        running: false
        minimumTimestep: 16.67
    }

    camera: fpsCamera
    environment: Environment{}

    DirectionalLight {
        id: light

        eulerRotation {
            x: -30
            y: 90
        }
        castsShadow: true
        shadowMapFar: 30000
        shadowMapQuality: Light.ShadowMapQualityUltra
        softShadowQuality: Light.PCF16
        use32BitShadowmap: true
        brightness: 0.1
        shadowFactor: 100
        color: "#ffefa1"
    }

    FPSCharacterController {
        id: character

        position: Qt.vector3d(1000, 1500, 1200)
        eulerRotation.y: fpsCamera.eulerRotation.y

        gravity: physicsWorld.gravity
        movement: view3d.player.movement

        FPSCamera {
            id: fpsCamera

            eulerRotation: Qt.vector3d(
                view3d.player.cameraRotation.x,
                view3d.player.cameraRotation.y,
                0.0
            )
        }
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
                materials: [ TerrainMaterial{} ]
            }
        }

        Component.onCompleted: physicsWorld.running = true
    }
}
