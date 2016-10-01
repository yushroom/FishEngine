/// Cull Front
/// ZWrite Off
layout(location = 0) in vec3 position;
out vec3 uv;

void main()
{
    uv = position.xyz;
    vec3 p = (Object2World * vec4(position, 1)).xyz + WorldSpaceCameraPos;
    gl_Position = MATRIX_VP * vec4(p, 1);
    //uv.z = -uv.z;
}