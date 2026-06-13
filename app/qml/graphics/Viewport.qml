import QtQuick
import QtQuick3D
import QtQuick3D.Helpers
import QtQuick3D.Physics

import "materials"

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
        lensFlareEnabled: true
        lensFlareBloomBias: 7.9
        lensFlareBloomScale: 1
        lensFlareApplyDirtTexture: true
        lensFlareApplyStarburstTexture: true
        lensFlareBlurAmount: 20
        lensFlareDistortion: 4
        ditheringEnabled: true
        vignetteEnabled: true
        vignetteRadius: 0.1
        specularAAEnabled: true
        fxaaEnabled: true
        glowEnabled: true
        glowQualityHigh: true
        glowUseBicubicUpscale: true
        glowStrength: 1.0
        glowIntensity: 0.1
        glowBloom: 1.0
        glowHDRMinimumValue: 4.0
        glowHDRScale: 4.0
        glowBlendMode: ExtendedSceneEnvironment.GlowBlendMode.Screen
        glowLevel: (ExtendedSceneEnvironment.GlowLevel.One
            | ExtendedSceneEnvironment.GlowLevel.Two
            | ExtendedSceneEnvironment.GlowLevel.Three
            | ExtendedSceneEnvironment.GlowLevel.Four
            | ExtendedSceneEnvironment.GlowLevel.Five
            | ExtendedSceneEnvironment.GlowLevel.Six
            | ExtendedSceneEnvironment.GlowLevel.Seven)
        exposure: 1.0
        lightProbe: Texture {
            id: skyTexture
            source: "file:app/resources/textures/sky/sky.hdr"
        }
        probeExposure: 0.1
        probeHorizon: 0.5
    }

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

    Node {
        id: cameraNode

        position: Qt.vector3d(1000, 1000, 1200)

        PerspectiveCamera {
            id: perspectiveCamera

            fieldOfView: 90
            clipFar: 30000
            frustumCullingEnabled: true

            PointLight {
                brightness: 1.0
                constantFade: 0.3
            }
        }
    }

    WasdController {
        controlledObject: cameraNode
        smooth: true
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
