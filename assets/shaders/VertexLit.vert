layout (location = PositionIndex) in vec3 position;
layout (location = NormalIndex) in vec3 normal;

uniform vec3 _Color = vec3(1, 1, 1);
uniform vec3 unity_LightPosition; // view space light position

const vec3 dir = normalize(vec3(0, 0, 1));

out VS_OUT {
    vec3 color;
} vs_out;

void main()
{
    gl_Position = MATRIX_MVP * vec4(position, 1);
    float c = dot(mat3(MATRIX_IT_MV) * normal, normalize(unity_LightPosition));
    c = clamp(c, 0, 1);
    vs_out.color = _Color * c;
}