layout (location = PositionIndex) in vec3 position;
layout (location = NormalIndex) in vec3 normal;
layout (location = UVIndex) in vec2 uv;

out VS_OUT {
    vec3 position;
    vec3 normal;
    vec2 uv;
    vec4 positionInLightSpace;
} vs_out;

void main() {
    vec4 posH = vec4(position, 1);
    gl_Position = MATRIX_MVP * posH;
    vs_out.position = (Object2World * posH).xyz;
    vs_out.normal = mat3(MATRIX_IT_M) * normal;
    vs_out.uv = uv;
    vs_out.positionInLightSpace = LightMatrix0 * vec4(vs_out.position, 1.0f);
}