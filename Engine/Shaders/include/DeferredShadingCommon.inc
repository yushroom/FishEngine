#ifndef DeferredShadingCommon_inc
#define DeferredShadingCommon_inc

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
    float3  DiffuseColor;
    float3  WorldNormal;
    float   Metallic;
    float   Roughness;
    float   Specular;
    //float   AO;
    uint    MaterialID;
};

void EncodeGBuffer(
    FGBufferData GBuffer,
    out float4 RT0,
    out float4 RT1,
    out float4 RT2)
{
    RT0 = float4(GBuffer.DiffuseColor, GBuffer.MaterialID);
    RT1 = float4(GBuffer.WorldNormal, 1);
    RT2 = float4(GBuffer.Metallic, GBuffer.Roughness, GBuffer.Specular, 1);
}

#endif
