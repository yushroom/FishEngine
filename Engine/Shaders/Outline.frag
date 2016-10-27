#include "CGSupport.inc"
/// Cull Front
in VS_OUT {
    vec2 uv;
} vs_out;

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