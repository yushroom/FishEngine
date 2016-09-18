in vec3 uv;
layout(location = PositionIndex) out vec4 color;
uniform float intensity;
uniform samplerCube skyTex;
void main()
{
    color = texture(skyTex, normalize(uv));
    //color.rgb = normalize(uv);
    color.a = 1.0;
}