layout (location = PositionIndex) in vec3 position;
layout (location = NormalIndex) in vec3 normal;
out VS_OUT {
    vec3 normal;
} vs_out;
out vec3 fragNormal;

void main() {
    gl_Position = MATRIX_MVP * vec4(position, 1.0f);
    mat3 normalMatrix = mat3(MATRIX_IT_M);
    vs_out.normal = normalize(vec3(MATRIX_VP * vec4(normalMatrix*normal, 1.0)));
}