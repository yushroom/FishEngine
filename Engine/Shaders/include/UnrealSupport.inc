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

/*
 * Pseudo random number generator, based on "TEA, a tiny Encrytion Algorithm"
 * http://citeseer.ist.psu.edu/viewdoc/download?doi=10.1.1.45.281&rep=rep1&type=pdf
 * http://www.umbc.edu/~olano/papers/index.html#GPUTEA
 * @param v - old seed (full 32bit range)
 * @param IterationCount - >=1, bigger numbers cost more performance but improve quality
 * @return new seed
 */
uint2 ScrambleTEA(uint2 v, uint IterationCount)
{
    // Start with some random data (numbers can be arbitrary but those have been used by others and seem to work well)
    uint k[4] = uint[4]( 0xA341316Cu , 0xC8013EA4u , 0xAD90777Du , 0x7E95761Eu );
    
    uint y = v[0];
    uint z = v[1];
    uint sum = 0;
    
    for(uint i = 0; i < IterationCount; ++i)
    {
        sum += 0x9e3779b9;
        y += ((z << 4u) + k[0]) ^ (z + sum) ^ ((z >> 5u) + k[1]);
        z += ((y << 4u) + k[2]) ^ (y + sum) ^ ((y >> 5u) + k[3]);
    }

    return uint2(y, z);
}

uint2 ScrambleTEA(uint2 v)
{
    return ScrambleTEA(v, 3);
}


#endif // UnrealSupport_inc
