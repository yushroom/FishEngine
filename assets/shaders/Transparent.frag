/// Blend On
in VS_OUT {
    vec3 position;
    vec3 normal;
    vec2 uv;
} vs_out;

uniform sampler2D DiffuseMap;

out float4 color;

void main()
{
	// vec3 L = -_WorldSpaceLightPos0.xyz; // directional light
 //    if (_WorldSpaceLightPos0.w > 0.5f) { // other type
 //        L = normalize(_WorldSpaceLightPos0.xyz - vs_out.position);
 //    }
 //    vec3 N = normalize(vs_out.normal);
 //    float NDotL = saturate(dot(L, N));
	// color.rgb = texture(DiffuseMap, vs_out.uv).rgb * NDotL;
	// color.a = 1.0f;
	color = texture(DiffuseMap, vs_out.uv);
}