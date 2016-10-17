#ifndef CGSupport_inc
#define CGSupport_inc

#include <CG.inc>

#define float2 vec2
#define float3 vec3
#define float4 vec4
#define uint2 uvec2
#define uint3 uvec3
#define uint4 uvec4
#define float2x2 mat2
#define float3x3 mat3
#define float4x4 mat4

// http://http.developer.nvidia.com/Cg/lit.html
float4 lit(float NdotL, float NdotH, float m)
{
    float specular = (NdotL > 0) ? pow(max(0.0, NdotH), m) : 0;
    return float4(1.0, max(0.0, NdotL), specular, 1.0);
}

void sincos(float a, out float s, out float c)
{
	s = sin(a);
	c = cos(a);
}

// http://http.developer.nvidia.com/Cg/lerp.html
#define lerp(x, y, t) mix(x, y, t)

// http://http.developer.nvidia.com/Cg/saturate.html
#define saturate(x) clamp(x, 0.0, 1.0)

//http://http.developer.nvidia.com/Cg/frac.html
#define frac(x) fract(x)

// http://http.developer.nvidia.com/Cg/tex2D.html
#define tex2D texture

#define mul(M, V) ((M) * (V))

#endif //CGSupport_inc
