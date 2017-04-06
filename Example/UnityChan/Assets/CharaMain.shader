#include <UnitySupport.inc>

struct VS_OUT
{
	//LIGHTING_COORDS( 0, 1 )
	float2 uv;
	float3 eyeDir;
	float3 normal;
	float3 tangent;
	float3 binormal;
	float3 lightDir;
};

@vertex
{
	#include <AppData.inc>

	// Transforms 2D UV by scale/bias property
	#define TRANSFORM_TEX(tex,name) (tex.xy * name##_ST.xy + name##_ST.zw)


	// Constants
	#define FALLOFF_POWER 0.3

	//#ifdef ENABLE_CAST_SHADOWS

	out VS_OUT vs_out;

	void vs_main(AppData appdata)
	{
		gl_Position = MATRIX_MVP * appdata.position;
		vs_out.uv = appdata.uv;
		vs_out.normal = normalize((Object2World * float4(appdata.normal, 0)).xyz);

		float4 worldPos = Object2World * appdata.position;
		vs_out.eyeDir = normalize(WorldSpaceCameraPos.xyz - worldPos.xyz);

		vs_out.tangent = normalize(appdata.tangent);
		//vs_out.binormal = cross(normal, tangent) * tangent.w;
		vs_out.binormal = normalize(cross(appdata.normal, appdata.tangent));

		vs_out.lightDir = WorldSpaceLightDir(appdata.position.xyz);
	}
}

@fragment
{
	#include <CGSupport.inc>
	#include <ShaderVariables.inc>
	// Material parameters
	uniform float4 _Color;
	uniform float4 _ShadowColor;
	//uniform float4 _LightColor0;
	uniform float _SpecularPower;
	uniform float4 _MainTex_ST;

	// Textures
	uniform sampler2D _MainTex;
	uniform sampler2D _FalloffSampler;
	uniform sampler2D _RimLightSampler;
	uniform sampler2D _SpecularReflectionSampler;
	uniform sampler2D _EnvMapSampler;
	uniform sampler2D _NormalMapSampler;

	// Constants
	#define FALLOFF_POWER 0.3

	//#ifdef ENABLE_CAST_SHADOWS

	in VS_OUT vs_out;

	out float4 color;

	#define float_t float
	#define float2_t float2
	#define float3_t float3
	#define float4_t float4

	// Overlay blend
	float3_t GetOverlayColor( float3_t inUpper, float3_t inLower )
	{
		float3_t oneMinusLower = float3_t( 1.0, 1.0, 1.0 ) - inLower;
		float3_t valUnit = 2.0 * oneMinusLower;
		float3_t minValue = 2.0 * inLower - float3_t( 1.0, 1.0, 1.0 );
		float3_t greaterResult = inUpper * valUnit + minValue;

		float3_t lowerResult = 2.0 * inLower * inUpper;

		half3 lerpVals = round(inLower);
		return lerp(lowerResult, greaterResult, lerpVals);
	}

	// Compute normal from normal map
	float3_t GetNormalFromMap()
	{
		float3_t normalVec = normalize( tex2D( _NormalMapSampler, vs_out.uv ).xyz * 2.0 - 1.0 );
		normalVec = vs_out.tangent * normalVec.x + vs_out.binormal * normalVec.y + vs_out.normal * normalVec.z;
		return normalVec;
	}

	void main()
	{
		float4_t diffSamplerColor = tex2D( _MainTex, vs_out.uv.xy );
		//color = diffSamplerColor;
		//return;

		//float3_t normalVec = vs_out.normal;
		float3_t normalVec = GetNormalFromMap();
		
		// Falloff. Convert the angle between the normal and the camera direction into a lookup for the gradient
		float_t normalDotEye = dot( normalVec, vs_out.eyeDir.xyz );
		float_t falloffU = clamp( 1.0 - abs( normalDotEye ), 0.02, 0.98 );
		float4_t falloffSamplerColor = FALLOFF_POWER * tex2D( _FalloffSampler, float2( falloffU, 0.25f ) );
		float3_t shadowColor = diffSamplerColor.rgb * diffSamplerColor.rgb;
		float3_t combinedColor = lerp( diffSamplerColor.rgb, shadowColor, falloffSamplerColor.r );
		combinedColor *= ( 1.0 + falloffSamplerColor.rgb * falloffSamplerColor.a );


		// Use the eye vector as the light vector
		float4_t reflectionMaskColor = tex2D( _SpecularReflectionSampler, vs_out.uv.xy );
		float_t specularDot = dot( normalVec, vs_out.eyeDir.xyz );
		float4_t lighting = lit( normalDotEye, specularDot, _SpecularPower );
		float3_t specularColor = saturate( lighting.z ) * reflectionMaskColor.rgb * diffSamplerColor.rgb;
		combinedColor += specularColor;
		
		// Reflection
		float3_t reflectVector = reflect( -vs_out.eyeDir.xyz, normalVec ).xzy;
		float2_t sphereMapCoords = 0.5 * ( float2_t( 1.0, 1.0 ) + reflectVector.xy );
		float3_t reflectColor = tex2D( _EnvMapSampler, sphereMapCoords ).rgb;
		reflectColor = GetOverlayColor( reflectColor, combinedColor );

		combinedColor = lerp( combinedColor, reflectColor, reflectionMaskColor.a );
		combinedColor *= _Color.rgb * LightColor0.rgb;
		float opacity = diffSamplerColor.a * _Color.a * LightColor0.a;

	#ifdef ENABLE_CAST_SHADOWS
		// Cast shadows
		shadowColor = _ShadowColor.rgb * combinedColor;
		float_t attenuation = saturate( 2.0 * LIGHT_ATTENUATION( i ) - 1.0 );
		combinedColor = lerp( shadowColor, combinedColor, attenuation );
	#endif

		// Rimlight
		float_t rimlightDot = saturate( 0.5 * ( dot( normalVec, vs_out.lightDir ) + 1.0 ) );
		falloffU = saturate( rimlightDot * falloffU );
		falloffU = tex2D( _RimLightSampler, float2( falloffU, 0.25f ) ).r;
		float3_t lightColor = diffSamplerColor.rgb; // * 2.0;
		combinedColor += falloffU * lightColor;

		color = float4( combinedColor, opacity );
	}
}