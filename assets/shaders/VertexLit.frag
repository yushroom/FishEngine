in VS_OUT {
    //vec3 normal;
    vec3 color;
} vs_out;

out vec4 color;

void main()
{
    //float c = dot(vs_out.normal, _WorldSpaceCameraPos);
    //c = clamp(c, 0, 1);
    color = vec4(vs_out.color, 1);
}