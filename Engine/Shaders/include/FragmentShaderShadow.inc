#ifndef FragmentShaderShadow_inc
#define FragmentShaderShadow_inc

#include <CG.inc>
//#include <ShadowCommon.inc>
//#include <CascadedShadowMapCommon.inc>

#ifdef _NORMALMAP
	uniform sampler2D NormalMap;
	const float bumpiness = 0.2;
#endif 

struct VS_OUT
{
	vec3 position;      // in world space
	vec3 normal;        // in world space
	vec3 tangent;
	vec2 uv;
};
in VS_OUT vs_out;

out vec4 color;

vec3 UnpackNormalMap(vec4 TextureSample)
{
#if 1
	return TextureSample.xyz * 2.0 - 1.0;
#else
	vec2 NormalXY = TextureSample.xy;
	NormalXY = NormalXY * 2.0 - 1.0;
	float NormalZ = sqrt( saturate( 1.0 - dot(NormalXY, NormalXY) ) );
	return vec3( NormalXY, NormalZ );
#endif
}


vec3 GetNormal()
{
#ifdef _NORMALMAP
	vec3 N = normalize(vs_out.normal);
	vec3 T = normalize(vs_out.tangent);
	vec3 B = normalize(cross(T, N));    // OpenGL is right-handed
	//mat3 Tangent2World = transpose(mat3(T, B, N));
	vec3 TangentNormal = UnpackNormalMap( texture(NormalMap, vs_out.uv) );
	//return normalize(Tangent2World * TangentNormal);
	return normalize(T * TangentNormal.x + B * TangentNormal.y + N * TangentNormal.z);
#else
	return normalize(vs_out.normal);
#endif
}


struct SurfaceData
{
	vec3 L;
	vec3 V;
	vec3 N;
	vec2 uv;
	float Depth;    // depth to camera
};


vec4 ps_main(SurfaceData surfaceData);


void main()
{
	SurfaceData surfaceData;
	vec3 L = normalize(WorldSpaceLightDir(vs_out.position));
	vec3 V = WorldSpaceCameraPos.xyz - vs_out.position;
	float Depth = length(V);
	V /= Depth;
	//vec3 N = normalize(vs_out.normal);
	vec3 N = GetNormal();
	surfaceData.L = L;
	surfaceData.V = V;
	surfaceData.N = N;
	surfaceData.uv = vs_out.uv;
	surfaceData.Depth = Depth;

	color = ps_main(surfaceData);

// #ifdef _SHADOW
//     vec4 WorldPosition = vec4(vs_out.position, 1.0);
//     color.rgb *= CalcShadowTerm(WorldPosition);
// #endif
	//color.a = 1.0f;
}

#endif /* FragmentShaderShadow_inc */
