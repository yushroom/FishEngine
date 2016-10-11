#include "CG.inc"
in VS_OUT {
	vec3 position;
    vec3 normal;
    vec2 uv;
} vs_out;

//uniform sampler2D DiffuseMap;

out vec4 color;

void main()
{
	//color = texture(DiffuseMap, vs_out.uv);
	vec3 L = normalize(WorldSpaceLightDir(vs_out.position));
    float nDotL = dot(normalize(vs_out.normal), L);
    nDotL = clamp(nDotL, 0.0f, 1.0f);
    color = vec4(nDotL, nDotL, nDotL, 1);
}