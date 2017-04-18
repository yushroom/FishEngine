// Unity built-in shader source. Copyright (c) 2016 Unity Technologies. MIT license (see license.txt)

#ifndef CG_inc
#define CG_inc

#include <ShaderVariables.inc>
#include <UnitySupport.inc>


// Tranforms position from object to homogenous space
float4 UnityObjectToClipPos( in float3 pos )
{
	return mul(UNITY_MATRIX_MVP, float4(pos, 1.0));
}

float4 UnityObjectToClipPos(float4 pos) // overload for float4; avoids "implicit truncation" warning for existing shaders
{
	return UnityObjectToClipPos(pos.xyz);
}

vec4 ObjectToViewPos( in vec3 pos )
{
	return MATRIX_MV * vec4(pos, 1.0);
}

vec4 ObjectToViewPos( in vec4 pos )
{
	return ObjectToViewPos( pos.xyz );
}

vec4 ObjectToClipPos( in vec3 pos )
{
	return MATRIX_MVP * vec4(pos, 1.0);
}

vec4 ObjectToClipPos( vec4 pos )
{
	return ObjectToClipPos(pos.xyz);
}

vec4 WorldToClipPos( in vec3 pos)
{
	return MATRIX_VP * vec4(pos, 1.0);
}

vec4 ViewToClipPos( in vec3 pos )
{
	return MATRIX_P * vec4(pos, 1.0);
}

vec3 WorldSpaceLightDir( in vec3 worldPos )
{
	return WorldSpaceLightPos.xyz - worldPos * WorldSpaceLightPos.w;
}

vec3 WorldSpaceViewDir( in vec3 worldPos )
{
	return WorldSpaceCameraPos.xyz - worldPos;
}


// Transforms normal from object to world space
inline float3 UnityObjectToWorldNormal( in float3 norm )
{
#ifdef UNITY_ASSUME_UNIFORM_SCALING
	return UnityObjectToWorldDir(norm);
#else
	// mul(IT_M, norm) => mul(norm, I_M) => {dot(norm, I_M.col0), dot(norm, I_M.col1), dot(norm, I_M.col2)}
	return normalize(mul(norm, float3x3(unity_WorldToObject)));
#endif
}

// Computes world space light direction, from world space position
inline float3 UnityWorldSpaceLightDir( in float3 worldPos )
{
    #ifndef USING_LIGHT_MULTI_COMPILE
        return _WorldSpaceLightPos0.xyz - worldPos * _WorldSpaceLightPos0.w;
    #else
        #ifndef USING_DIRECTIONAL_LIGHT
        return _WorldSpaceLightPos0.xyz - worldPos;
        #else
        return _WorldSpaceLightPos0.xyz;
        #endif
    #endif
}


/************************************************************************/
/* Shadow                                                               */
/************************************************************************/

float4 UnityClipSpaceShadowCasterPos(float4 vertex, float3 normal)
{
	float4 wPos = mul(unity_ObjectToWorld, vertex);

	if (unity_LightShadowBias.z != 0.0)
	{
		float3 wNormal = UnityObjectToWorldNormal(normal);
		float3 wLight = normalize(UnityWorldSpaceLightDir(wPos.xyz));

		// apply normal offset bias (inset position along the normal)
		// bias needs to be scaled by sine between normal and light direction
		// (http://the-witness.net/news/2013/09/shadow-mapping-summary-part-1/)
		//
		// unity_LightShadowBias.z contains user-specified normal offset amount
		// scaled by world space texel size.

		float shadowCos = dot(wNormal, wLight);
		float shadowSine = sqrt(1-shadowCos*shadowCos);
		float normalBias = unity_LightShadowBias.z * shadowSine;

		wPos.xyz -= wNormal * normalBias;
	}

	return mul(UNITY_MATRIX_VP, wPos);
}

#define UNITY_NEAR_CLIP_VALUE -1.0

float4 UnityApplyLinearShadowBias(float4 clipPos)
{
#if defined(UNITY_REVERSED_Z)
	// We use max/min instead of clamp to ensure proper handling of the rare case
	// where both numerator and denominator are zero and the fraction becomes NaN.
	clipPos.z += max(-1, min(unity_LightShadowBias.x / clipPos.w, 0));
	float clamped = min(clipPos.z, clipPos.w*UNITY_NEAR_CLIP_VALUE);
#else
	clipPos.z += saturate(unity_LightShadowBias.x/clipPos.w);
	float clamped = max(clipPos.z, clipPos.w*UNITY_NEAR_CLIP_VALUE);
#endif
	clipPos.z = lerp(clipPos.z, clamped, unity_LightShadowBias.y);
	return clipPos;
}


#ifdef SHADOWS_CUBE
	// Rendering into point light (cubemap) shadows
	#define V2F_SHADOW_CASTER_NOPOS float3 vec : TEXCOORD0;
	#define TRANSFER_SHADOW_CASTER_NOPOS_LEGACY(o,opos) o.vec = mul(unity_ObjectToWorld, v.vertex).xyz - _LightPositionRange.xyz; opos = UnityObjectToClipPos(v.vertex);
	#define TRANSFER_SHADOW_CASTER_NOPOS(o,opos) o.vec = mul(unity_ObjectToWorld, v.vertex).xyz - _LightPositionRange.xyz; opos = UnityObjectToClipPos(v.vertex);
	#define SHADOW_CASTER_FRAGMENT(i) return UnityEncodeCubeShadowDepth ((length(i.vec) + unity_LightShadowBias.x) * _LightPositionRange.w);
#else
	// Rendering into directional or spot light shadows
	#define V2F_SHADOW_CASTER_NOPOS
	// Let embedding code know that V2F_SHADOW_CASTER_NOPOS is empty; so that it can workaround
	// empty structs that could possibly be produced.
	#define V2F_SHADOW_CASTER_NOPOS_IS_EMPTY
	#define TRANSFER_SHADOW_CASTER_NOPOS_LEGACY(o,opos) \
		opos = UnityObjectToClipPos(v.vertex.xyz); \
		opos = UnityApplyLinearShadowBias(opos);
	#define TRANSFER_SHADOW_CASTER_NOPOS(o,opos) \
		opos = UnityClipSpaceShadowCasterPos(v.vertex, v.normal); \
		opos = UnityApplyLinearShadowBias(opos);
	#define SHADOW_CASTER_FRAGMENT(i) return 0;
#endif

// Declare all data needed for shadow caster pass output (any shadow directions/depths/distances as needed),
// plus clip space position.
#define V2F_SHADOW_CASTER V2F_SHADOW_CASTER_NOPOS float4 pos : SV_POSITION

// Vertex shader part, with support for normal offset shadows. Requires
// position and normal to be present in the vertex input.
#define TRANSFER_SHADOW_CASTER_NORMALOFFSET(o) TRANSFER_SHADOW_CASTER_NOPOS(o,o.pos)


#endif /* CG_inc */

