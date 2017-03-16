#ifndef ShadowCommon_inc
#define ShadowCommon_inc

#include <ShaderVariables.inc>

uniform sampler2D ScreenShadowMap;

float GetScreenShadow(vec2 UV)
{
	return texture(ScreenShadowMap, UV).r;
}

#if 0

uniform sampler2DArray ShadowMap;
const float bias = 0.002f;
const float ExpC = 300.0;

vec4 GetCascadeWeights( float Depth )
{
	vec4 near = step( CascadesSplitPlaneNear, vec4(Depth) );
	vec4 far  = step( Depth, CascadesSplitPlaneFar);
	return near * far;
}

// WorldPosition
// Depth: the distance from WorldPosition to WorldPositionCameraPos
float CalcShadowTerm(vec4 WorldPosition, float Depth)
{
	vec4 CascadeWeights = GetCascadeWeights( Depth );
	float fSection = dot(vec4(0, 1, 2, 3), CascadeWeights);
	uint Section = uint(fSection);
	
	vec4 ProjCoords = LightMatrix[Section] * WorldPosition;
	ProjCoords.xyz = ProjCoords.xyz * (0.5 / ProjCoords.w) + 0.5;

	float Z = ProjCoords.z;
	float Occluder = texture( ShadowMap, vec3(ProjCoords.xy, fSection) ).r;

#if 1
	// ESM
	//return clamp( Occluder * exp( -ExpC*Z ), 0.2, 1.0 );
	return clamp( exp( ExpC*(Occluder - Z) ), 0.2, 1.0 );
#elif 0
	// a modified version of ESM
	// http://www.klayge.org/2013/10/07/%E5%88%87%E6%8D%A2%E5%88%B0esm/
	float C = ExpC / (ProjectionParams.z - ProjectionParams.y); //  ExpC / (far - near)
	return clamp( exp( C * (Z - Occluder) ), 0.0, 1.0 );
#else
	float Shadow = 1.0;
	if (Z - bias > Occluder)
		Shadow = 0.2f;
	return Shadow;
#endif
}

float CalcShadowTerm(vec4 WorldPosition)
{
	float Depth = distance(WorldPosition.xyz, WorldSpaceCameraPos.xyz);
	return CalcShadowTerm(WorldPosition, Depth);
}

#endif

#if 0

// http://learnopengl.com/#!Advanced-Lighting/Shadows/Shadow-Mapping
float ShadowCalculation(vec4 WorldPosition, vec3 lightDir, vec3 normal)
{
#ifdef _SHADOW
	vec4 fragPosLightSpace = LightMatrix[0] * WorldPosition;
	// perform perspective divide
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	// Transform to [0,1] range
	projCoords = projCoords * 0.5 + 0.5;
	// Get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
	float closestDepth = texture( ShadowMap, vec3(projCoords.xy, 0) ).r; 
	// Get depth of current fragment from light's perspective
	float currentDepth = projCoords.z;
	// Calculate bias (based on depth map resolution and slope)
	vec3 N = normalize(normal);
	//vec3 lightDir = normalize(lightPos - fs_in.FragPos);
	float bias = max(0.0002 * (1.0 - dot(N, lightDir)), bias);
	// Check whether current frag pos is in shadow
	//float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;

	// PCF
	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(ShadowMap, 0).xy;
	for(int x = -1; x <= 1; ++x)
	{
		for(int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(ShadowMap, vec3(projCoords.xy + vec2(x, y) * texelSize, 0)).r; 
			shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
		}    
	}
	shadow /= 9.0;
	
	// Keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
	if(projCoords.z > 1.0)
		shadow = 0.0;
		
	return shadow;
#else
	return 1;
#endif
}

#endif

#endif
