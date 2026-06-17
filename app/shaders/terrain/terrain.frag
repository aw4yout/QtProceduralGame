VARYING vec3 worldPosition;
VARYING vec3 normal;

VARYING flat vec2 material;
VARYING float blendWeight;

const float texScale = 0.5;

vec4 sampleTexture(sampler2D tex, vec3 pos, vec3 norm) {
    vec3 blendWeights = pow(abs(norm), vec3(4.0));
    blendWeights = blendWeights / (blendWeights.x + blendWeights.y + blendWeights.z);

    vec4 colX = texture(tex, pos.yz * texScale);
    vec4 colY = texture(tex, pos.xz * texScale);
    vec4 colZ = texture(tex, pos.xy * texScale);

    return colX * blendWeights.x + colY * blendWeights.y + colZ * blendWeights.z;
}

vec3 sampleNormal(sampler2D normalMap, vec3 pos, vec3 norm) {
    vec3 blendWeights = pow(abs(norm), vec3(4.0));
    blendWeights = blendWeights / (blendWeights.x + blendWeights.y + blendWeights.z);

    vec3 axisSign = vec3(
        norm.x < 0.0 ? -1.0 : 1.0,
        norm.y < 0.0 ? -1.0 : 1.0,
        norm.z < 0.0 ? -1.0 : 1.0
    );

    vec2 uvX = pos.yz * texScale;
    vec2 uvY = pos.xz * texScale;
    vec2 uvZ = pos.xy * texScale;

    uvX.x *= axisSign.x;
    uvY.x *= axisSign.y;
    uvZ.x *= -axisSign.z;

    vec3 tnormalX = texture(normalMap, uvX).xyz * 2.0 - 1.0;
    vec3 tnormalY = texture(normalMap, uvY).xyz * 2.0 - 1.0;
    vec3 tnormalZ = texture(normalMap, uvZ).xyz * 2.0 - 1.0;

    tnormalX.x *= axisSign.x;
    tnormalY.x *= axisSign.y;
    tnormalZ.x *= -axisSign.z;

    tnormalX = vec3(tnormalX.xy + norm.zy, tnormalX.z * norm.x);
    tnormalY = vec3(tnormalY.xy + norm.xz, tnormalY.z * norm.y);
    tnormalZ = vec3(tnormalZ.xy + norm.xy, tnormalZ.z * norm.z);

    vec3 blendedNormal = normalize(
        tnormalX.zyx * blendWeights.x +
        tnormalY.xzy * blendWeights.y +
        tnormalZ.xyz * blendWeights.z
    );

    return blendedNormal;
}

vec4 getMaterialColor(float id, vec3 pos, vec3 norm) {
    float matId = round(id);
    if (matId == 1.0) return sampleTexture(grassColor, pos, norm);
    if (matId == 2.0) return sampleTexture(dirtColor, pos, norm);
    if (matId == 3.0) return sampleTexture(stoneColor, pos, norm);
    if (matId == 4.0) return sampleTexture(sandColor, pos, norm);
    if (matId == 5.0) return sampleTexture(snowColor, pos, norm);
    return vec4(1.0, 0.0, 1.0, 1.0);
}

vec3 getMaterialNormal(float id, vec3 pos, vec3 norm) {
    float matId = round(id);
    if (matId == 1.0) return sampleNormal(grassNormal, pos, norm);
    if (matId == 2.0) return sampleNormal(dirtNormal, pos, norm);
    if (matId == 3.0) return sampleNormal(stoneNormal, pos, norm);
    if (matId == 4.0) return sampleNormal(sandNormal, pos, norm);
    if (matId == 5.0) return sampleNormal(snowNormal, pos, norm);
    return vec3(0.0, 0.0, 1.0);
}

float getMaterialRoughness(float id, vec3 pos, vec3 norm) {
    float matId = round(id);
    if (matId == 1.0) return sampleTexture(grassRoughness, pos, norm).r;
    if (matId == 2.0) return sampleTexture(dirtRoughness, pos, norm).r;
    if (matId == 3.0) return sampleTexture(stoneRoughness, pos, norm).r;
    if (matId == 4.0) return sampleTexture(sandRoughness, pos, norm).r;
    if (matId == 5.0) return sampleTexture(snowRoughness, pos, norm).r;
    return 1.0;
}

float getMaterialAo(float id, vec3 pos, vec3 norm) {
    float matId = round(id);
    if (matId == 1.0) return sampleTexture(grassAo, pos, norm).r;
    if (matId == 2.0) return sampleTexture(dirtAo, pos, norm).r;
    if (matId == 3.0) return sampleTexture(stoneAo, pos, norm).r;
    if (matId == 4.0) return sampleTexture(sandAo, pos, norm).r;
    if (matId == 5.0) return sampleTexture(snowAo, pos, norm).r;
    return 0.0;
}

void MAIN() {
//    material interpolation, WIP!!!
//    vec4 colorPrimary = getMaterialColor(material.x);
//    vec4 colorSecondary = getMaterialColor(material.y);
//
//    float roughnessPrimary = getMaterialRoughness(material.x);
//    float roughnessSecondary = getMaterialRoughness(material.y);
//
//    float metalnessPrimary = getMaterialMetalness(material.x);
//    float metalnessSecondary = getMaterialMetalness(material.y);
//
//    BASE_COLOR = mix(colorPrimary, colorSecondary, blendWeight);
//    ROUGHNESS = mix(roughnessPrimary, roughnessSecondary, blendWeight);

    BASE_COLOR = getMaterialColor(material.x, worldPosition, normal);
    NORMAL = getMaterialNormal(material.x, worldPosition, normal);
    ROUGHNESS = getMaterialRoughness(material.x, worldPosition, normal);
    OCCLUSION_AMOUNT = getMaterialAo(material.x, worldPosition, normal);
    METALNESS = 0.0;
}
