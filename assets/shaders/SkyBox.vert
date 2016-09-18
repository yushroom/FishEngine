/// Cull Front
/// ZWrite Off
layout(location = 0) in vec3 position;
out vec3 uv;

void main()
{
    uv = position.xyz;
    vec3 p = (_Object2World * vec4(position, 1)).xyz + _WorldSpaceCameraPos;
    gl_Position = MATRIX_VP * vec4(p, 1);
    //uv.z = -uv.z;
}