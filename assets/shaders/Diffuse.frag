in VS_OUT {
    vec3 position;
    vec3 normal;
    vec2 uv;
} vs_out;

uniform sampler2D diffuseMap;

out vec4 color;

void main()
{
	vec3 l = _WorldSpaceLightPos0.xyz;
    if (_WorldSpaceLightPos0.w > 0.5f) {
        l = normalize(_WorldSpaceLightPos0.xyz - vs_out.position);
    }
    float nDotL = dot(normalize(vs_out.normal), l);
    nDotL = clamp(nDotL, 0.0f, 1.0f);
	color = nDotL * texture(diffuseMap, vs_out.uv);
	color.a = 1.0f;
}