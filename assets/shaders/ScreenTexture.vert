#include <CG.inc>

layout (location = PositionIndex) in vec3 position;
layout (location = UVIndex) in vec2 uv;

out VS_OUT {
    vec2 uv;
} vs_out;

void main()
{
    gl_Position = vec4(position.x, position.y, 0.f, 1.f);
    vs_out.uv = uv;
}