@Cull Front

struct VS_OUT
{
	vec2 uv;
};

@vertex
{
	#include "AppData.inc"

	uniform float _EdgeThickness = 0.5;
	// Outline thickness multiplier
	#define INV_EDGE_THICKNESS_DIVISOR 0.00285

	out VS_OUT vs_out;

	void vs_main(AppData appdata)
	{
	    vec4 projSpacePos = MATRIX_MVP * appdata.position;
	    vec4 projSpaceNormal = normalize( MATRIX_MVP * vec4(appdata.normal, 0));
	    vec4 scaledNormal = _EdgeThickness * INV_EDGE_THICKNESS_DIVISOR * projSpaceNormal; // * projSpacePos.w;

	    scaledNormal.z += 0.00001;
		gl_Position = projSpacePos + scaledNormal;
	    vs_out.uv = appdata.uv;
	}
}

@fragment
{
	#include "CGSupport.inc"

	in VS_OUT vs_out;

	out vec4 color;

	uniform sampler2D _MainTex;
	uniform vec4 _Color;

	// Outline color parameters
	#define SATURATION_FACTOR 0.6
	#define BRIGHTNESS_FACTOR 0.8

	void main()
	{
		vec4 diffuseMapColor = texture(_MainTex, vs_out.uv);
		float maxChan = max( max( diffuseMapColor.r, diffuseMapColor.g ), diffuseMapColor.b );
		float4 newMapColor = diffuseMapColor;

		maxChan -= ( 1.0 / 255.0 );
		float3 lerpVals = saturate( ( newMapColor.rgb - float3( maxChan, maxChan, maxChan ) ) * 255.0 );
		newMapColor.rgb = lerp( SATURATION_FACTOR * newMapColor.rgb, newMapColor.rgb, lerpVals );
		
		color = float4( BRIGHTNESS_FACTOR * newMapColor.rgb * diffuseMapColor.rgb, diffuseMapColor.a ) * _Color * LightColor0; 
	}
}