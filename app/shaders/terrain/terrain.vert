VARYING vec3 worldPosition;
VARYING vec3 normal;

VARYING flat vec2 material;
VARYING float blendWeight;

void MAIN()
{
    worldPosition = (MODEL_MATRIX * vec4(VERTEX, 1.0)).xyz;
    normal = normalize(NORMAL_MATRIX * NORMAL);
    material = TANGENT.xy; blendWeight = TANGENT.z; TANGENT = vec3(0.0);

    POSITION = MODELVIEWPROJECTION_MATRIX * vec4(VERTEX, 1.0);
}
