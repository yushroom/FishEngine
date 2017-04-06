#ifndef CGSupport_inc
#define CGSupport_inc

#define PI 3.141592653589793
#define INV_PI 0.3183098861837907

#define float2 vec2
#define float3 vec3
#define float4 vec4
#define uint2 uvec2
#define uint3 uvec3
#define uint4 uvec4
#define float2x2 mat2
#define float3x3 mat3
#define float4x4 mat4

#define half float
#define half2 vec2
#define half3 vec3
#define half4 vec4

#define samplerCUBE samplerCube

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
//#define lerp(x, y, t) mix(x, y, t)
float lerp(float a, float b, float w)
{
	return a + w*(b-a);
}

float2 lerp(float2 a, float2 b, float2 w)
{
	return a + w*(b-a);
}

float3 lerp(float3 a, float3 b, float3 w)
{
	return a + w*(b-a);
}

float4 lerp(float4 a, float4 b, float4 w)
{
	return a + w*(b-a);
}


float2 lerp(float2 a, float2 b, float w)
{
	return a + w*(b-a);
}

float3 lerp(float3 a, float3 b, float w)
{
	return a + w*(b-a);
}

float4 lerp(float4 a, float4 b, float w)
{
	return a + w*(b-a);
}

// http://http.developer.nvidia.com/Cg/saturate.html
#define saturate(x) clamp((x), 0.0, 1.0)

//http://http.developer.nvidia.com/Cg/frac.html
#define frac(x) fract(x)

// http://http.developer.nvidia.com/Cg/tex2D.html
#define tex2D texture
#define texCUBE texture

#define mul(M, V) ((M) * (V))

#endif //CGSupport_inc
