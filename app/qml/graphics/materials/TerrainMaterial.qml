import QtQuick
import QtQuick3D

CustomMaterial {
    component BaseTexture: Texture {
        generateMipmaps: true
    }

    property TextureInput dirtColor: TextureInput { texture: BaseTexture { source: "file:app/resources/textures/dirt/dirt_color.jpg" } }
    property TextureInput dirtNormal: TextureInput { texture: BaseTexture { source: "file:app/resources/textures/dirt/dirt_normal.jpg" } }
    property TextureInput dirtRoughness: TextureInput { texture: BaseTexture { source: "file:app/resources/textures/dirt/dirt_roughness.jpg" } }
    property TextureInput dirtAo: TextureInput { texture: BaseTexture { source: "file:app/resources/textures/dirt/dirt_ao.jpg" } }

    property TextureInput grassColor: TextureInput { texture: BaseTexture { source: "file:app/resources/textures/grass/grass_color.jpg" } }
    property TextureInput grassNormal: TextureInput { texture: BaseTexture { source: "file:app/resources/textures/grass/grass_normal.jpg" } }
    property TextureInput grassRoughness: TextureInput { texture: BaseTexture { source: "file:app/resources/textures/grass/grass_roughness.jpg" } }
    property TextureInput grassAo: TextureInput { texture: BaseTexture { source: "file:app/resources/textures/grass/grass_ao.jpg" } }

    property TextureInput sandColor: TextureInput { texture: BaseTexture { source: "file:app/resources/textures/sand/sand_color.jpg" } }
    property TextureInput sandNormal: TextureInput { texture: BaseTexture { source: "file:app/resources/textures/sand/sand_normal.jpg" } }
    property TextureInput sandRoughness: TextureInput { texture: BaseTexture { source: "file:app/resources/textures/sand/sand_roughness.jpg" } }
    property TextureInput sandAo: TextureInput { texture: BaseTexture { source: "file:app/resources/textures/sand/sand_ao.jpg" } }

    property TextureInput snowColor: TextureInput { texture: BaseTexture { source: "file:app/resources/textures/snow/snow_color.jpg" } }
    property TextureInput snowNormal: TextureInput { texture: BaseTexture { source: "file:app/resources/textures/snow/snow_normal.jpg" } }
    property TextureInput snowRoughness: TextureInput { texture: BaseTexture { source: "file:app/resources/textures/snow/snow_roughness.jpg" } }
    property TextureInput snowAo: TextureInput { texture: BaseTexture { source: "file:app/resources/textures/snow/snow_ao.jpg" } }

    property TextureInput stoneColor: TextureInput { texture: BaseTexture { source: "file:app/resources/textures/stone/stone_color.jpg" } }
    property TextureInput stoneNormal: TextureInput { texture: BaseTexture { source: "file:app/resources/textures/stone/stone_normal.jpg" } }
    property TextureInput stoneRoughness: TextureInput { texture: BaseTexture { source: "file:app/resources/textures/stone/stone_roughness.jpg" } }
    property TextureInput stoneAo: TextureInput { texture: BaseTexture { source: "file:app/resources/textures/stone/stone_ao.jpg" } }

    shadingMode: CustomMaterial.Shaded
    vertexShader: "qrc:/app/shaders/terrain/terrain.vert"
    fragmentShader: "qrc:/app/shaders/terrain/terrain.frag"
}
