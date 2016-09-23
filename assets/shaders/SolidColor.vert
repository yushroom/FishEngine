layout (location = PositionIndex) in vec3 position;

void main() {
    vec4 posH = vec4(position, 1);
    gl_Position = MATRIX_MVP * posH;
}