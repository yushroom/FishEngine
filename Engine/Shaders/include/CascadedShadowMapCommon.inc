#ifndef CascadedShadowMapCommon_inc
#define CascadedShadowMapCommon_inc

#include <CGSupport.inc>

//const float ExpC = 120.0;
//const float bias = 0.05;

uniform sampler2DArrayShadow CascadedShadowMap;

// vec4(1.0 / width, 1.0 / height, width, height)
// eg. (0.00048828125, 0.00048828125, 2048, 2048)
uniform vec4 _ShadowMapTexture_TexelSize;

vec4 GetCascadeWeights( float Depth )
{
	vec4 near = step( CascadesSplitPlaneNear, vec4(Depth) );
	vec4 far  = step( Depth, CascadesSplitPlaneFar);
	return near * far;
}

vec4 GetShadowCoord( vec4 WorldPosition, vec4 CascadeWeights )
{
	vec3 sc0 = (LightMatrix[0] * WorldPosition).xyz;
	vec3 sc1 = (LightMatrix[1] * WorldPosition).xyz;
	vec3 sc2 = (LightMatrix[2] * WorldPosition).xyz;
	vec3 sc3 = (LightMatrix[3] * WorldPosition).xyz;
	vec4 shadowMapCoordinate = vec4(sc0*CascadeWeights[0] + sc1*CascadeWeights[1] + sc2*CascadeWeights[2] + sc3*CascadeWeights[3], 1);
	return shadowMapCoordinate;
}


/**
 * PCF shadowmap filtering based on a 5x5 kernel (optimized with 9 taps)
 *
 * Algorithm: http://the-witness.net/news/2013/09/shadow-mapping-summary-part-1/
 * Implementation example: http://mynameismjp.wordpress.com/2013/09/10/shadow-maps/
 */
float SampleCascadeShadowMap_PCF5x5( vec3 Coord, float Section )
{
	const float2 offset = float2(0.5,0.5);
	float2 uv = (Coord.xy * _ShadowMapTexture_TexelSize.zw) + offset;
	float2 base_uv = (floor(uv) - offset) * _ShadowMapTexture_TexelSize.xy;
	float2 st = frac(uv);

	float3 uw = float3( 4-3*st.x, 7, 1+3*st.x );
	float3 u = float3( (3-2*st.x) / uw.x - 2, (3+st.x)/uw.y, st.x/uw.z + 2 );
	u *= _ShadowMapTexture_TexelSize.x;

	float3 vw = float3( 4-3*st.y, 7, 1+3*st.y );
	float3 v = float3( (3-2*st.y) / vw.x - 2, (3+st.y)/vw.y, st.y/vw.z + 2 );
	v *= _ShadowMapTexture_TexelSize.y;

	half shadow;
	half sum = 0.0f;

	//float bias = - unity_LightShadowBias.x * LightMatrix[int(Section)][2][2];
	//float z = Coord.z - bias;
	float z = Coord.z;

	half3 accum = uw * vw.x;
	sum += accum.x * texture( CascadedShadowMap, vec4(base_uv+float2(u.x, v.x), Section, z ));
	sum += accum.y * texture( CascadedShadowMap, vec4(base_uv+float2(u.y, v.x), Section, z ));
	sum += accum.z * texture( CascadedShadowMap, vec4(base_uv+float2(u.z, v.x), Section, z ));

	accum = uw * vw.y;
	sum += accum.x * texture( CascadedShadowMap, vec4(base_uv+float2(u.x, v.y), Section, z ));
	sum += accum.y * texture( CascadedShadowMap, vec4(base_uv+float2(u.y, v.y), Section, z ));
	sum += accum.z * texture( CascadedShadowMap, vec4(base_uv+float2(u.z, v.y), Section, z ));

	accum = uw * vw.z;
	sum += accum.x * texture( CascadedShadowMap, vec4(base_uv+float2(u.x, v.z), Section, z ));
	sum += accum.y * texture( CascadedShadowMap, vec4(base_uv+float2(u.y, v.z), Section, z ));
	sum += accum.z * texture( CascadedShadowMap, vec4(base_uv+float2(u.z, v.z), Section, z ));

	shadow = sum / 144.0f;
	return shadow;
}

half UnityComputeShadowFade(float fadeDist)
{
    return saturate(fadeDist * _LightShadowData.z + _LightShadowData.w);
}

// @param WorldPosition position in world space
// @param Depth depth to camera
float CalcShadowTerm(vec4 WorldPosition, float Depth)
{
	vec4 CascadeWeights = GetCascadeWeights( Depth );
	float fSection = dot(vec4(0, 1, 2, 3), CascadeWeights);

	//uint Section = uint(fSection);
	//vec4 ProjCoords = LightMatrix[Section] * WorldPosition;
	vec4 ProjCoords = GetShadowCoord( WorldPosition, CascadeWeights );
	ProjCoords.xyz = ProjCoords.xyz * (0.5 / ProjCoords.w) + 0.5;
	float Z = ProjCoords.z;
	float Shadow = SampleCascadeShadowMap_PCF5x5(ProjCoords.xyz, fSection);
	//Shadow += saturate(Z * 0.03333 - 2.7); // from unity
	Shadow += UnityComputeShadowFade(Z);
	// return shadow;
	return saturate(Shadow);
}


float CalcShadowTerm(vec4 WorldPosition)
{
	float Depth = distance(WorldPosition.xyz, WorldSpaceCameraPos.xyz);
	return CalcShadowTerm(WorldPosition, Depth);
}

#endif