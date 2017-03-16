#ifndef DeferredShadingCommon_inc
#define DeferredShadingCommon_inc

#include <UnrealSupport.inc>

float3 EncodeNormal( float3 N )
{
	return N * 0.5 + 0.5;
	//return Pack1212To888( UnitVectorToOctahedron( N ) * 0.5 + 0.5 );
}

float3 DecodeNormal( float3 N )
{
	return N * 2 - 1;
	//return OctahedronToUnitVector( Pack888To1212( N ) * 2 - 1 );
}

struct FGBufferData
{
	float3  BaseColor;
	float3  WorldNormal;
	float   Metallic;
	float   Roughness;
	float   Specular;
	//float   AO;
	//uint    MaterialID;
	float   Depth;
};

void EncodeGBuffer(
	FGBufferData GBuffer,
	out float4 RT0,
	out float4 RT1,
	out float4 RT2)
{
	RT0 = float4( GBuffer.BaseColor, 1 );
	RT1 = float4( EncodeNormal( GBuffer.WorldNormal ), 1 );
	RT2 = float4( GBuffer.Metallic, GBuffer.Roughness, GBuffer.Specular, 1 );
}

FGBufferData DecodeGBuffer(
	float4 DBufferA,
	float4 DBufferB,
	float4 DBufferC)
{
	FGBufferData ret;
	ret.BaseColor       = DBufferA.rgb;
	ret.WorldNormal     = DecodeNormal( DBufferB.xyz );
	ret.Metallic        = DBufferC.x;
	ret.Roughness       = DBufferC.y;
	ret.Specular        = DBufferC.z;
	return ret;
}

#endif
