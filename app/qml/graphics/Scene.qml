import QtQuick
import QtQuick3D
import QtQuick3D.Physics

import ProceduralGame

import "materials"
import "../player"

View3D {
    id: view3d

    required property var player
    property alias character: character

    function clamp(num, lower, upper) { return Math.min(Math.max(num, lower), upper); }

    PhysicsWorld {
        id: physicsWorld

        gravity: Qt.vector3d(0, -9.82, 0)
        scene: view3d.scene
        running: false
        minimumTimestep: 16.67
        // forceDebugDraw: true
    }

    camera: fpsCamera
    environment: Environment { id: environment }

    DirectionalLight {
        id: light

        eulerRotation {
            x: -30
            y: 90
        }
        castsShadow: true
        shadowMapFar: 3000
        shadowMapQuality: Light.ShadowMapQualityUltra
        softShadowQuality: Light.PCF16
        use32BitShadowmap: true
        brightness: 0.1
        shadowFactor: 100
        color: "#ffefa1"
    }

    Timer {
        interval: 500
        running: view3d.player.shooting
        repeat: true
        onTriggered: {
            const pick = view3d.pick(view3d.width / 2, view3d.height / 2)

            if (pick.objectHit && pick.distance < 100.0) {
                const worldPos = pick.scenePosition
                const voxelPos = Qt.vector3d(
                    Math.floor(worldPos.x / ChunkManager.worldScale),
                    Math.floor(worldPos.y / ChunkManager.worldScale),
                    Math.floor(worldPos.z / ChunkManager.worldScale)
                )

                ChunkManager.setVoxel(voxelPos, false)
            }
        }
    }

    FPSCharacterController {
        id: character

        position: Qt.vector3d(
            ChunkManager.chunkSize * ChunkManager.worldScale / 2.0,
            40 * ChunkManager.worldScale,
            ChunkManager.chunkSize * ChunkManager.worldScale / 2.0)
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

    Timer {
        interval: 50
        running: true
        repeat: true
        triggeredOnStart: true
        onTriggered: {
            ChunkManager.playerPosition = character.position
            ChunkManager.update(64)
        }
    }

    Component {
        id: meshShapeComponent
        TriangleMeshShape {}
    }

    Repeater3D {
        model: ChunkManager.activeChunks
        onObjectAdded: {
            physicsWorld.running = true
        }
        delegate: StaticRigidBody {
            id: body

            required property var modelData
            required property int index

            property var collisionShape: null

            property bool isPlayerNearby:
                (modelData.chunkPosition.x === Math.floor(character.position.x / ChunkManager.chunkSize)
                && modelData.chunkPosition.y === Math.floor(character.position.y / ChunkManager.chunkSize)
                && modelData.chunkPosition.z === Math.floor(character.position.z / ChunkManager.chunkSize))

            scale: Qt.vector3d(1, 1, 1).times(ChunkManager.worldScale)

            onIsPlayerNearbyChanged: updatePhysics()
            Component.onCompleted: updatePhysics()

            function updatePhysics() {
                if (isPlayerNearby) {
                    if (!collisionShape && body.modelData && body.modelData.vertexCount > 0) {
                        collisionShape = meshShapeComponent.createObject(body)
                        body.collisionShapes = [collisionShape]
                        collisionShape.geometry = body.modelData
                    }
                } else {
                    if (collisionShape) {
                        collisionShape.destroy()
                    }
                }
            }

            Model {
                id: model

                pickable: true
                geometry: body.modelData
                materials: [ TerrainMaterial{} ]
            }
        }
    }
}
