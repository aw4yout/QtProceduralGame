import QtQuick
import QtQuick3D
import QtQuick3D.Helpers

ExtendedSceneEnvironment {
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
