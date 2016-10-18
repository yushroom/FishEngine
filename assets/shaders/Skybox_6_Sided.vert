#include <UnitySupport.inc>

/// Cull Front
/// ZWrite Off

uniform vec4        _Tint;
uniform float       _Exposure;
uniform float       _Rotation;
uniform sampler2D   _FrontTex;
uniform sampler2D   _BackTex;
uniform sampler2D   _LeftTex;
uniform sampler2D   _RightTex;
uniform sampler2D   _UpTex;
uniform sampler2D   _DownTex;

#ifdef VERTEX_SHADER

layout(location = PositionIndex)	in vec3 InputPositon;
layout(location = UVIndex)			in vec2 InputUV;

out vec2 texcoord;

float3 RotateAroundYInDegrees (float3 vertex, float degrees)
{
	float alpha = degrees * UNITY_PI / 180.0;
	float sina, cosa;
	sincos(alpha, sina, cosa);
	float2x2 m = float2x2(cosa, -sina, sina, cosa);
	return float3(mul(m, vertex.xz), vertex.y).xzy;
}

void main()
{
	float3 rotated = RotateAroundYInDegrees(InputPositon, _Rotation);
	gl_Position = UnityObjectToClipPos(rotated);
	texcoord = InputUV;
}

#endif

#ifdef FRAGMENT_SHADER



#endif

