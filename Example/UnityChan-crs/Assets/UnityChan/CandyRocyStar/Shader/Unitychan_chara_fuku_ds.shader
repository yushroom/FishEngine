#include <UnitySupport.inc>
#include <CG.inc>

@Properties
{
	_Color ("Main Color", Color) = (1, 1, 1, 1)
	_ShadowColor ("Shadow Color", Color) = (0.8, 0.8, 1, 1)
	_SpecularPower ("Specular Power", Float) = 20
	_EdgeThickness ("Outline Thickness", Float) = 1
	_MainTex ("Diffuse", 2D) = "white" {}
	_FalloffSampler ("Falloff Control", 2D) = "white" {}
	_RimLightSampler ("RimLight Control", 2D) = "white" {}
	_SpecularReflectionSampler ("Specular / Reflection Mask", 2D) = "white" {}
	_EnvMapSampler ("Environment Map", 2D) = "" {} 
	_NormalMapSampler ("Normal Map", 2D) = "" {} 
}

uniform vec4 _Color;
uniform vec4 _ShadowColor;
uniform float _SpecularPower;
uniform float _EdgeThickness;
uniform sampler2D _MainTex;
uniform sampler2D _FalloffSampler;
uniform sampler2D _RimLightSampler;
uniform sampler2D _SpecularReflectionSampler;
uniform sampler2D _EnvMapSampler;
uniform sampler2D _NormalMapSampler;

// Constants
#define FALLOFF_POWER 0.3

#ifdef ENABLE_CAST_SHADOWS

// Structure from vertex shader to fragment shader
struct v2f
{
	float4 pos;
	LIGHTING_COORDS( 0, 1 );
	float2 uv;
	float3 eyeDir;
	float3 lightDir;
	float3 normal;
#ifdef ENABLE_NORMAL_MAP;
	float3 tangent;
	float3 binormal;
#endif
};

#else

// Structure from vertex shader to fragment shader
struct VS_OUT
{
	float4 pos;
	float2 uv;
	float3 eyeDir;
	float3 lightDir;
	float3 normal;
#ifdef ENABLE_NORMAL_MAP
	float3 tangent;
	float3 binormal;
#endif
};

#endif

// Float types
#define float_t    half
#define float2_t   half2
#define float3_t   half3
#define float4_t   half4
#define float3x3_t half3x3


@vertex
{
	#include <Appdata.inc>
	//#define appdata_tan AppData

	out VS_OUT vs_out; 

	// Vertex shader
	void vs_main( AppData v )
	{
		//v2f o;
		vs_out.pos = mul( UNITY_MATRIX_MVP, v.position );
		//vs_out.uv.xy = TRANSFORM_TEX( v.texcoord.xy, _MainTex );
		vs_out.uv.xy = v.uv.xy;
		vs_out.normal = normalize( mul( unity_ObjectToWorld, float4_t( v.normal, 0 ) ).xyz );

		// Eye direction vector
		half4 worldPos = mul( unity_ObjectToWorld, v.position );
		vs_out.eyeDir.xyz = normalize( _WorldSpaceCameraPos.xyz - worldPos.xyz ).xyz;
		vs_out.lightDir = UnityWorldSpaceLightDir( worldPos.xyz );

	#ifdef ENABLE_NORMAL_MAP
		// Binormal and tangent (for normal map)
		vs_out.tangent = normalize( mul( unity_ObjectToWorld, float4_t( v.tangent.xyz, 0 ) ).xyz );
		vs_out.binormal = normalize( cross( vs_out.normal, vs_out.tangent ) * v.tangent.w );
	#endif

	#ifdef ENABLE_CAST_SHADOWS
		TRANSFER_VERTEX_TO_FRAGMENT( vs_out );
	#endif

		return vs_out;
	}
}

@fragment
{
	// Overlay blend
	inline float3_t GetOverlayColor( float3_t inUpper, float3_t inLower )
	{
		float3_t oneMinusLower = float3_t( 1.0, 1.0, 1.0 ) - inLower;
		float3_t valUnit = 2.0 * oneMinusLower;
		float3_t minValue = 2.0 * inLower - float3_t( 1.0, 1.0, 1.0 );
		float3_t greaterResult = inUpper * valUnit + minValue;

		float3_t lowerResult = 2.0 * inLower * inUpper;

		half3 lerpVals = round(inLower);
		return lerp(lowerResult, greaterResult, lerpVals);
	}

	#ifdef ENABLE_NORMAL_MAP

	// Compute normal from normal map
	inline float3_t GetNormalFromMap( v2f input )
	{
		float3_t normalVec = normalize( tex2D( _NormalMapSampler, input.uv ).xyz * 2.0 - 1.0 );
		float3x3_t localToWorldTranspose = float3x3_t(
			input.tangent,
			input.binormal,
			input.normal
		);

		normalVec = normalize( mul( normalVec, localToWorldTranspose ) );
		return normalVec;
	}

	#endif

	in VS_OUT vs_out;
	out float4 OutColor;

	// Fragment shader
	//float4 frag( v2f i )
	void main()
	{
		float4_t diffSamplerColor = tex2D( _MainTex, vs_out.uv.xy );

	#ifdef ENABLE_NORMAL_MAP
		float3_t normalVec = GetNormalFromMap( vs_out );
	#else
		float3_t normalVec = vs_out.normal;
	#endif

		// Falloff. Convert the angle between the normal and the camera direction into a lookup for the gradient
		float_t normalDotEye = dot( normalVec, vs_out.eyeDir.xyz );
		float_t falloffU = clamp( 1.0 - abs( normalDotEye ), 0.02, 0.98 );
		float4_t falloffSamplerColor = FALLOFF_POWER * tex2D( _FalloffSampler, float2( falloffU, 0.25f ) );
		float3_t shadowColor = diffSamplerColor.rgb * diffSamplerColor.rgb;
		float3_t combinedColor = lerp( diffSamplerColor.rgb, shadowColor, falloffSamplerColor.r );
		combinedColor *= ( 1.0 + falloffSamplerColor.rgb * falloffSamplerColor.a );

		// Specular
		// Use the eye vector as the light vector
	#ifdef ENABLE_SPECULAR
		float4_t reflectionMaskColor = tex2D( _SpecularReflectionSampler, vs_out.uv.xy );
		float_t specularDot = dot( normalVec, vs_out.eyeDir.xyz );
		float4_t lighting = lit( normalDotEye, specularDot, _SpecularPower );
		float3_t specularColor = saturate( lighting.z ) * reflectionMaskColor.rgb * diffSamplerColor.rgb;
		combinedColor += specularColor;
	#endif

		// Reflection
	#ifdef ENABLE_REFLECTION
		float3_t reflectVector = reflect( -i.eyeDir.xyz, normalVec ).xzy;
		float2_t sphereMapCoords = 0.5 * ( float2_t( 1.0, 1.0 ) + reflectVector.xy );
		float3_t reflectColor = tex2D( _EnvMapSampler, sphereMapCoords ).rgb;
		reflectColor = GetOverlayColor( reflectColor, combinedColor );

		combinedColor = lerp( combinedColor, reflectColor, reflectionMaskColor.a );
	#endif
		combinedColor *= _Color.rgb * _LightColor0.rgb;
		float opacity = diffSamplerColor.a * _Color.a * _LightColor0.a;

	#ifdef ENABLE_CAST_SHADOWS
		// Cast shadows
		shadowColor = _ShadowColor.rgb * combinedColor;
		float_t attenuation = saturate( 2.0 * LIGHT_ATTENUATION( i ) - 1.0 );
		combinedColor = lerp( shadowColor, combinedColor, attenuation );
	#endif

		// Rimlight
	#ifdef ENABLE_RIMLIGHT
		float_t rimlightDot = saturate( 0.5 * ( dot( normalVec, i.lightDir ) + 1.0 ) );
		falloffU = saturate( rimlightDot * falloffU );
		falloffU = tex2D( _RimLightSampler, float2( falloffU, 0.25f ) ).r;
		float3_t lightColor = diffSamplerColor.rgb; // * 2.0;
		combinedColor += falloffU * lightColor;
	#endif

		OutColor = float4( combinedColor, opacity );
	}
} 

/*
Shader "UnityChan/Clothing - Double-sided"
{
	Properties
	{
		_Color ("Main Color", Color) = (1, 1, 1, 1)
		_ShadowColor ("Shadow Color", Color) = (0.8, 0.8, 1, 1)
		_SpecularPower ("Specular Power", Float) = 20
		_EdgeThickness ("Outline Thickness", Float) = 1
				
		_MainTex ("Diffuse", 2D) = "white" {}
		_FalloffSampler ("Falloff Control", 2D) = "white" {}
		_RimLightSampler ("RimLight Control", 2D) = "white" {}
		_SpecularReflectionSampler ("Specular / Reflection Mask", 2D) = "white" {}
		_EnvMapSampler ("Environment Map", 2D) = "" {} 
		_NormalMapSampler ("Normal Map", 2D) = "" {} 
	}

CGINCLUDE
#include "UnityCG.cginc"
#include "AutoLight.cginc"
ENDCG

	SubShader
	{
		Tags
		{
			"RenderType"="OpaqueDoubleSided"
			"Queue"="Geometry"
			"LightMode"="ForwardBase"
		}		

        LOD 450

		Pass
		{
			Cull Off
			ZTest LEqual
CGPROGRAM
#pragma multi_compile_fwdbase
#pragma target 3.0
#pragma vertex vert
#pragma fragment frag
#define ENABLE_CAST_SHADOWS
#define ENABLE_NORMAL_MAP
#define ENABLE_SPECULAR
#define ENABLE_REFLECTION
#define ENABLE_RIMLIGHT
#include "CharaMain.cg"
ENDCG
		}

		Pass
		{
			Cull Front
			ZTest Less
CGPROGRAM
#pragma target 3.0
#pragma vertex vert
#pragma fragment frag
#include "UnityCG.cginc"
#include "CharaOutline.cg"
ENDCG
		}

	}

	SubShader
	{
		Tags
		{
			"RenderType"="OpaqueDoubleSided"
			"Queue"="Geometry"
			"LightMode"="ForwardBase"
		}

        LOD 400

		Pass
		{
			Cull Off
			ZTest LEqual
CGPROGRAM
#pragma multi_compile_fwdbase
#pragma target 3.0
#pragma vertex vert
#pragma fragment frag
#define ENABLE_CAST_SHADOWS
#define ENABLE_SPECULAR
#define ENABLE_RIMLIGHT
#include "CharaMain.cg"
ENDCG
		}

		Pass
		{
			Cull Front
			ZTest Less
CGPROGRAM
#pragma target 3.0
#pragma vertex vert
#pragma fragment frag
#include "UnityCG.cginc"
#include "CharaOutline.cg"
ENDCG
		}

    }


	SubShader
	{
		Tags
		{
			"RenderType"="OpaqueDoubleSided"
			"Queue"="Geometry"
			"LightMode"="ForwardBase"
		}

        LOD 300

		Pass
		{
			Cull Off
			ZTest LEqual
CGPROGRAM
#pragma multi_compile_fwdbase
#pragma target 3.0
#pragma vertex vert
#pragma fragment frag
#define ENABLE_CAST_SHADOWS
#define ENABLE_SPECULAR
#define ENABLE_RIMLIGHT
#include "CharaMain.cg"
ENDCG
		}

    }

	SubShader
	{
		Tags
		{
			"RenderType"="OpaqueDoubleSided"
			"Queue"="Geometry"
			"LightMode"="ForwardBase"
		}

        LOD 250

		Pass
		{
			Cull Off
			ZTest LEqual
CGPROGRAM
#pragma multi_compile_fwdbase
#pragma target 3.0
#pragma vertex vert
#pragma fragment frag
#define ENABLE_CAST_SHADOWS
#define ENABLE_RIMLIGHT
#include "CharaMain.cg"
ENDCG
		}

	}

	SubShader
	{
		Tags
		{
			"RenderType"="OpaqueDoubleSided"
			"Queue"="Geometry"
		}

        LOD 200

		Pass
		{
			Cull Off
			ZTest LEqual
CGPROGRAM
#pragma vertex vert
#pragma fragment frag
#include "Unlit.cg"
ENDCG
		}

	}

	FallBack "Diffuse"
}
*/

