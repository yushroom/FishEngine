#pragma once

#include <BRDF.inc>

uniform samplerCube AmbientCubemap;

const uint GDiffuseConvolveMipLevel = 4;

// MipCount = FMath::Log2(CubemapWidth) + 1.0f;
const uint MipCount = 9;
// in PostProcessAmbient.cpp
// AmbientCubemapMipAdjustValue.X =  1.0f - GDiffuseConvolveMipLevel / MipCount;
// AmbientCubemapMipAdjustValue.Y = (MipCount - 1.0f) * AmbientCubemapMipAdjustValue.X;
// AmbientCubemapMipAdjustValue.Z = MipCount - GDiffuseConvolveMipLevel;
// AmbientCubemapMipAdjustValue.W = MipCount;
const half4 AmbientCubemapMipAdjust = half4(0.5555555555555556, 4.444444444444445, 5.0, 9.0);

// @param MipCount e.g. 10 for x 512x512
half ComputeCubemapMipFromRoughness( half Roughness, half MipCount )
{
    // Level starting from 1x1 mip
    half Level = 3 - 1.15 * log2( Roughness );
    return MipCount - 1 - Level;
}


float4 ImportanceSampleGGX( float2 E, float Roughness )
{
    float m = Roughness * Roughness;
    float m2 = m * m;

    float Phi = 2 * PI * E.x;
    float CosTheta = sqrt( (1 - E.y) / ( 1 + (m2 - 1) * E.y ) );
    float SinTheta = sqrt( 1 - CosTheta * CosTheta );

    float3 H;
    H.x = SinTheta * cos( Phi );
    H.y = SinTheta * sin( Phi );
    H.z = CosTheta;
    
    float d = ( CosTheta * m2 - CosTheta ) * CosTheta + 1;
    float D = m2 / ( PI*d*d );
    float PDF = D * CosTheta;

    return float4( H, PDF );
}

float3 PrefilterEnvMap( uint2 Random, float Roughness, float3 R )
{
    float3 FilteredColor = float3(0);
    float Weight = 0;
        
    const uint NumSamples = 64;
    for( uint i = 0; i < NumSamples; i++ )
    {
        float2 E = Hammersley( i, NumSamples, Random );
        float3 H = TangentToWorld( ImportanceSampleGGX( E, Roughness ).xyz, R );
        float3 L = 2 * dot( R, H ) * H - R;

        float NoL = saturate( dot( R, L ) );
        if( NoL > 0 )
        {
            //FilteredColor += AmbientCubemap.SampleLevel( AmbientCubemapSampler, L, 0 ).rgb * NoL;
            FilteredColor += textureLod(AmbientCubemap, L, 0).rgb * NoL;
            Weight += NoL;
        }
    }

    return FilteredColor / max( Weight, 0.001 );
}
//#endif

float3 IntegrateBRDF( uint2 Random, float Roughness, float NoV )
{
    float3 V;
    V.x = sqrt( 1.0f - NoV * NoV ); // sin
    V.y = 0;
    V.z = NoV;                      // cos

    float A = 0;
    float B = 0;
    float C = 0;

    const uint NumSamples = 64;
    for( uint i = 0; i < NumSamples; i++ )
    {
        float2 E = Hammersley( i, NumSamples, Random );

        {
            float3 H = ImportanceSampleGGX( E, Roughness ).xyz;
            float3 L = 2 * dot( V, H ) * H - V;

            float NoL = saturate( L.z );
            float NoH = saturate( H.z );
            float VoH = saturate( dot( V, H ) );

            if( NoL > 0 )
            {
                float Vis = Vis_SmithJointApprox( Roughness, NoV, NoL );

                float a = Square( Roughness );
                float a2 = a*a;
                float Vis_SmithV = NoL * sqrt( NoV * (NoV - NoV * a2) + a2 );
                float Vis_SmithL = NoV * sqrt( NoL * (NoL - NoL * a2) + a2 );
                //float Vis = 0.5 * rcp( Vis_SmithV + Vis_SmithL );

                // Incident light = NoL
                // pdf = D * NoH / (4 * VoH)
                // NoL * Vis / pdf
                float NoL_Vis_PDF = NoL * Vis * (4 * VoH / NoH);

                float Fc = pow( 1 - VoH, 5 );
                A += (1 - Fc) * NoL_Vis_PDF;
                B += Fc * NoL_Vis_PDF;
            }
        }

        {
            float3 L = CosineSampleHemisphere( E ).xyz;
            float3 H = normalize(V + L);

            float NoL = saturate( L.z );
            float NoH = saturate( H.z );
            float VoH = saturate( dot( V, H ) );

            float FD90 = ( 0.5 + 2 * VoH * VoH ) * Roughness;
            float FdV = 1 + (FD90 - 1) * pow( 1 - NoV, 5 );
            float FdL = 1 + (FD90 - 1) * pow( 1 - NoL, 5 );
            C += FdV * FdL * ( 1 - 0.3333 * Roughness );
        }
    }

    return float3( A, B, C ) / NumSamples;
}


float3 ApproximateSpecularIBL( uint2 Random, float3 SpecularColor, float Roughness, float3 N, float3 V )
{
    // Function replaced with prefiltered environment map sample
    float3 R = 2 * dot( V, N ) * N - V;
    float3 PrefilteredColor = PrefilterEnvMap( Random, Roughness, R );
    //float3 PrefilteredColor = FilterEnvMap( Random, Roughness, N, V );

    // Function replaced with 2D texture sample
    float NoV = saturate( dot( N, V ) );
    float2 AB = IntegrateBRDF( Random, Roughness, NoV ).xy;

    return PrefilteredColor * ( SpecularColor * AB.x + AB.y );
}

float3 DiffuseIBL( uint2 Random, float3 DiffuseColor, float Roughness, float3 N, float3 V )
{
    N = normalize( N );
    V = normalize( V );

    float3 DiffuseLighting = float3(0);
    
    float NoV = saturate( dot( N, V ) );

    const uint NumSamples = 32;
    for( uint i = 0; i < NumSamples; i++ )
    {
        float2 E = Hammersley( i, NumSamples, Random );
        float3 L = TangentToWorld( CosineSampleHemisphere( E ).xyz, N );
        float3 H = normalize(V + L);

        float NoL = saturate( dot( N, L ) );
        float NoH = saturate( dot( N, H ) );
        float VoH = saturate( dot( V, H ) );

        if( NoL > 0 )
        {
            //float3 SampleColor = AmbientCubemap.SampleLevel( AmbientCubemapSampler, L, 0 ).rgb;
            float3 SampleColor = textureLod(AmbientCubemap, L, 0).rgb;

            float FD90 = ( 0.5 + 2 * VoH * VoH ) * Roughness;
            //float FD90 = 0.5 + 2 * VoH * VoH * Roughness;
            float FdV = 1 + (FD90 - 1) * pow( 1 - NoV, 5 );
            float FdL = 1 + (FD90 - 1) * pow( 1 - NoL, 5 );

#if 1
            // lambert = DiffuseColor * NoL / PI
            // pdf = NoL / PI
            DiffuseLighting += SampleColor * DiffuseColor * FdV * FdL * ( 1 - 0.3333 * Roughness );
#else
            DiffuseLighting += SampleColor * DiffuseColor;
#endif
        }
    }

    return DiffuseLighting / NumSamples;
}


float3 SpecularIBL( uint2 Random, float3 SpecularColor, float Roughness, float3 N, float3 V )
{
    float3 SpecularLighting = vec3(0);

    const uint NumSamples = 1024;
    for( uint i = 0; i < NumSamples; i++ )
    {
        float2 E = Hammersley( i, NumSamples, Random );
        float3 H = TangentToWorld( ImportanceSampleGGX( E, Roughness ).xyz, N );
        float3 L = 2 * dot( V, H ) * H - V;

        float NoV = saturate( dot( N, V ) );
        float NoL = saturate( dot( N, L ) );
        float NoH = saturate( dot( N, H ) );
        float VoH = saturate( dot( V, H ) );
        
        if( NoL > 0 )
        {
            //float3 SampleColor = AmbientCubemap.SampleLevel( AmbientCubemapSampler, L, 0 ).rgb;
            float3 SampleColor = textureLod(AmbientCubemap, L, 0).rgb;

            float Vis = Vis_SmithJointApprox( Roughness, NoV, NoL );
            float Fc = pow( 1 - VoH, 5 );
            float3 F = (1 - Fc) * SpecularColor + Fc;

            // Incident light = SampleColor * NoL
            // Microfacet specular = D*G*F / (4*NoL*NoV) = D*Vis*F
            // pdf = D * NoH / (4 * VoH)
            SpecularLighting += SampleColor * F * ( NoL * Vis * (4 * VoH / NoH) );
        }
    }

    return SpecularLighting / NumSamples;
}
