@Cull Front
@ZWrite Off

#include <UnitySupport.inc>
#include <ShaderVariables.inc>
#include <CG.inc>

uniform samplerCube _Tex;
uniform vec4        _Tint;
uniform float       _Exposure;
uniform float       _Rotation;

@vertex
{
	layout(location = PositionIndex) in vec3 InputPositon;

	out vec3 uv;

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
		uv = InputPositon.xyz;
	}
}

@fragment
{
	in vec3 uv;

	out vec4 color;

	void main()
	{
		half4 tex = texCUBE (_Tex, uv);
		//half3 c = DecodeHDR (tex, _Tex_HDR);
		half3 c = tex.xyz;
		//c = c * _Tint.rgb * unity_ColorSpaceDouble.rgb;
		c *= _Tint.rgb * _Exposure;
		//c *= _Exposure;
		color = half4(c, 1);
	}
}