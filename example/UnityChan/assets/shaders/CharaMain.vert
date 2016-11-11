#include "UnitySupport.inc"
#include "AppDataTan.inc"

// Material parameters
// float4 _Color;
// float4 _ShadowColor;
// float4 _LightColor0;
// float _SpecularPower;
// float4 _MainTex_ST;

// Transforms 2D UV by scale/bias property
#define TRANSFORM_TEX(tex,name) (tex.xy * name##_ST.xy + name##_ST.zw)

// Computes world space light direction, from world space position
float3 UnityWorldSpaceLightDir( in float3 worldPos )
{
    #ifndef USING_LIGHT_MULTI_COMPILE
        return WorldSpaceLightPos0.xyz - worldPos * WorldSpaceLightPos0.w;
    #else
        #ifndef USING_DIRECTIONAL_LIGHT
        return WorldSpaceLightPos0.xyz - worldPos;
        #else
        return WorldSpaceLightPos0.xyz;
        #endif
    #endif
}

// Computes world space light direction, from object space position
// *Legacy* Please use UnityWorldSpaceLightDir instead
float3 WorldSpaceLightDir( in float4 localPos )
{
    float3 worldPos = mul(unity_ObjectToWorld, localPos).xyz;
    return UnityWorldSpaceLightDir(worldPos);
}



// Constants
#define FALLOFF_POWER 0.3

//#ifdef ENABLE_CAST_SHADOWS

out VS_OUT {
    //LIGHTING_COORDS( 0, 1 )
    float2 uv;
    float3 eyeDir;
    float3 normal;
    float3 tangent;
    float3 binormal;
    float3 lightDir;
} vs_out;

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

    vs_out.lightDir = WorldSpaceLightDir(appdata.position);
}