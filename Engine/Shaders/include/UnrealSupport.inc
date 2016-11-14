#ifndef UnrealSupport_inc
#define UnrealSupport_inc

#include <CGSupport.inc>

#define ReverseBits32(x) bitfieldReverse(x)

float3 TangentToWorld( float3 Vec, float3 TangentZ )
{
    float3 UpVector = abs(TangentZ.z) < 0.999 ? float3(0,0,1) : float3(1,0,0);
    float3 TangentX = normalize( cross( UpVector, TangentZ ) );
    float3 TangentY = cross( TangentZ, TangentX );
    return TangentX * Vec.x + TangentY * Vec.y + TangentZ * Vec.z;
}

float2 Hammersley( uint Index, uint NumSamples, uint2 Random )
{
    float E1 = frac( float(Index) / NumSamples + float( Random.x & uint(0xffff) ) / (1<<16) );
    float E2 = float( ReverseBits32(Index) ^ Random.y ) * 2.3283064365386963e-10;
    return float2( E1, E2 );
}

float4 CosineSampleHemisphere( float2 E )
{
    float Phi = 2 * PI * E.x;
    float CosTheta = sqrt( E.y );
    float SinTheta = sqrt( 1 - CosTheta * CosTheta );

    float3 H;
    H.x = SinTheta * cos( Phi );
    H.y = SinTheta * sin( Phi );
    H.z = CosTheta;

    float PDF = CosTheta / PI;

    return float4( H, PDF );
}

float Square( float x )
{
    return x*x;
}

float2 Square( float2 x )
{
    return x*x;
}

float3 Square( float3 x )
{
    return x*x;
}

float4 Square( float4 x )
{
    return x*x;
}

float Pow5(float x)
{
    float x2 = x*x;
    return x2*x2*x;
}

float rcp(float x) {
    return 1.0f / x;
}

#endif // UnrealSupport_inc
