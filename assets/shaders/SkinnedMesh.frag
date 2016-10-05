in VS_OUT {
    vec3 normal;
    vec2 uv;
} vs_out;

uniform sampler2D DiffuseMap;

out vec4 color;

void main()
{
	color = texture(DiffuseMap, vs_out.uv);
}