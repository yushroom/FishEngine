#ifndef HLSLSupport_inc
#define HLSLSupport_inc

#define float2 vec2
#define float3 vec3
#define float4 vec4
#define uint2 uvec2
#define uint3 uvec3
#define uint4 uvec4
#define half float
#define half2 vec2
#define half3 vec3
#define half4 vec4
#define lerp(x, y, t) mix((x), (y), (t))
#define frac(x) fract(x)
#define saturate(x) clamp((x), 0.0, 1.0)

#endif // HLSLSupport_inc
