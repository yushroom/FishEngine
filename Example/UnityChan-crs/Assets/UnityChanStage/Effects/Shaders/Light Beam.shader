@name "Custom/Light Beam"
@ZWrite Off
@Blend SrcAlpha OneMinusSrcAlpha
@Cull Off

#include <UnitySupport.inc>

@Properties
{
	_Color      ("Base Color", Color)    = (1, 1, 1, 1)
	_MainTex    ("Gradient Texture", 2D) = ""{}
	_NoiseTex1  ("Noise Texture 1", 2D)  = ""{}
	_NoiseTex2  ("Noise Texture 2", 2D)  = ""{}
	_NoiseScale ("Noise Scale", Vector)  = (1, 1, 1, 1)
	_NoiseSpeed ("Noise Speed", Vector)  = (0.1, 0.1, 0.1, 0.1)
}

struct v2f
{
	float2 uv0;
	float2 uv1;
	float2 uv2;
	float4 world_position;
	float3 normal;
};

uniform float4 _Color;
uniform sampler2D _MainTex;
uniform float4 _MainTex_ST;
uniform sampler2D _NoiseTex1;
uniform sampler2D _NoiseTex2;
uniform float4 _NoiseScale;
uniform float4 _NoiseSpeed;

@vertex
{
	#include <AppData.inc>

	layout(location = 0) out v2f vs_out;

	void vs_main(AppData v)
	{
		gl_Position = UnityObjectToClipPos(v.position);
		vs_out.uv0 = v.uv;

		float4 wp = mul(unity_ObjectToWorld, v.position);
		vs_out.uv1 = wp.xy * _NoiseScale.xy + _NoiseSpeed.xy * Time.y;
		vs_out.uv2 = wp.xy * _NoiseScale.zw + _NoiseSpeed.zw * Time.y;
		vs_out.world_position = v.position;
		vs_out.normal = normalize(mul(mat3(unity_ObjectToWorld), v.normal));
	}
}

@fragment
{
	layout(location = 0) in v2f vs_out;
	layout(location = 0) out float4 OutColor;

	void main()
	{
		float3 normal = vs_out.normal;
		float3 camDir = normalize(vs_out.world_position.xyz - _WorldSpaceCameraPos.xyz);
		float falloff = max(abs(dot(camDir, normal))-0.4, 0.0);
		falloff = falloff * falloff * 5.0;

		float4 c = _Color;
		float n1 = texture(_NoiseTex1, vs_out.uv1).r;
		float n2 = texture(_NoiseTex2, vs_out.uv2).r;

		c.a *= texture(_MainTex, vs_out.uv0).r * n1 * n2 * falloff;

		OutColor = c;
	}
}