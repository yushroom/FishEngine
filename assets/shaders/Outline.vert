layout (location = PositionIndex) in vec3 position;
layout (location = NormalIndex) in vec3 normal;
layout (location = UVIndex) in vec2 uv;

uniform float _EdgeThickness = 0.5;
// Outline thickness multiplier
#define INV_EDGE_THICKNESS_DIVISOR 0.00285

out VS_OUT {
    vec2 uv;
} vs_out;

void main() {
    vec4 projSpacePos = MATRIX_MVP * vec4(position, 1);
    vec4 projSpaceNormal = normalize( MATRIX_MVP * vec4(normal, 0));
    vec4 scaledNormal = _EdgeThickness * INV_EDGE_THICKNESS_DIVISOR * projSpaceNormal; // * projSpacePos.w;

    scaledNormal.z += 0.00001;
	gl_Position = projSpacePos + scaledNormal;
    vs_out.uv = uv;
}