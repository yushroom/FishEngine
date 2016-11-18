struct VS_OUT
{
    vec3 normal;
    vec3 tangent;
    vec2 uv;
};

#ifdef VERTEX_SHADER
	#include "AppData.inc"
	out VS_OUT vs_out;

	void vs_main(AppData appdata)
	{
	    vs_out.normal = mat3(MATRIX_IT_M) * appdata.normal;
	    vs_out.tangent = mat3(MATRIX_M) * appdata.tangent;
	    vs_out.uv = appdata.uv;
	    gl_Position = MATRIX_MVP * appdata.position;
	}

#endif // VERTEX_SHADER

#ifdef FRAGMENT_SHADER
	const vec3 lightDir = normalize(vec3(1, 1, 1));
	const float bumpiness = 0.2;
	uniform sampler2D diffuseMap;
	uniform sampler2D normalMap;
	in VS_OUT vs_out;
	out vec4 color;
	void main()
	{
	    vec3 N = normalize(vs_out.normal);
	    vec3 T = normalize(vs_out.tangent);
	    vec3 B = normalize(cross(T, N));
	    mat3 TBN = mat3(T, B, N);
	    vec3 bump_normal;
	    bump_normal.xy = 2.0 * texture(normalMap, vs_out.uv).gr - 1.0;
	    bump_normal.z = sqrt(1.0 - dot(bump_normal.xy, bump_normal.xy));
	    vec3 tangent_normal = mix(vec3(0, 0, 1), bump_normal, bumpiness);
	    vec3 normal = TBN * tangent_normal;
	    
	    float c = clamp(dot(normal, lightDir), 0.0, 1.0);
	    
	    color.rgb = c * texture(diffuseMap, vs_out.uv).rgb;
	    //color.rgb = vec3(c,c,c);
	    color.a = 1.0f;
	}
#endif // FRAGMENT_SHADER
