layout (location = PositionIndex) in vec3 position;
layout (location = NormalIndex) in vec3 normal;
layout (location = TangentIndex) in vec3 tangent;
layout (location = UVIndex) in vec2 uv;
//uniform sampler2D bumpTex;

out VS_OUT {
    vec3 normal;
    vec3 tangent;
    vec2 uv;
} vs_out;

void main()
{
    vs_out.normal = mat3(MATRIX_IT_M) * normal;
    vs_out.tangent = mat3(_Object2World) * tangent;
    vs_out.uv = uv;
    gl_Position = MATRIX_MVP * vec4(position, 1.0);
}
