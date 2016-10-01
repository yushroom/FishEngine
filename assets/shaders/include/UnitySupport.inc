#ifndef UnitySupport_inc
#define UnitySupport_inc

#include <CGSupport.inc>

#define float2 vec2
#define float3 vec3
#define float4 vec4
#define uint2 uvec2
#define uint3 uvec3
#define uint4 uvec4
#define lerp(x, y, t) mix(x, y, t)
#define frac(x) fract(x)
#define saturate(x) clamp(x, 0.0, 1.0)

#endif // UnitySupport_inc