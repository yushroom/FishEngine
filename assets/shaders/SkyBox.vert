#include <CG.inc>

/// Cull Front
/// ZWrite Off
layout(location = PositionIndex) in vec3 InputPositon;
out vec3 uv;

void main()
{
    uv = InputPositon.xyz;
    vec3 p = (MATRIX_M * vec4(InputPositon, 1)).xyz + WorldSpaceCameraPos;
    gl_Position = MATRIX_VP * vec4(p, 1);
    //uv.z = -uv.z;
}