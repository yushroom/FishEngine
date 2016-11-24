#pragma once

#include <BRDF.inc>
#include <ShadingModels.inc>
#include <DeferredShadingCommon.inc>
#include <MonteCarlo.inc>
#include <CubemapCommon.inc>
#include <Random.inc>

float3 DiffuseIBL( uint2 Random, float3 DiffuseColor, float Roughness, float3 N, float3 V )
{
    N = normalize( N );
    V = normalize( V );

    float3 DiffuseLighting = float3(0);
    
    float NoV = saturate( dot( N, V ) );

    const uint NumSamples = 512;
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

#if 0
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

    const uint NumSamples = 512;
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


float3 ImageBasedLightingMIS(FGBufferData GBuffer, float3 V, float3 N, uint2 Random)
{
    float3 Lighting = float3(0);

    float Roughness1 = GBuffer.Roughness;
    float Roughness2 = 0.1;

    uint NumSamples[] = uint[](16, 16, 0);

    for (uint Set = 0; Set < 3; Set++)
    {
        for (uint i = 0; i < NumSamples[ Set ]; i++)
        {
            float2 E = Hammersley( i, NumSamples[ Set ], Random );

            float3 L, H;
            if ( Set == 0 )
            {
                L = TangentToWorld( CosineSampleHemisphere( E ).xyz, N );
                H = normalize(V + L);
            }
            else if ( Set == 1)
            {
                H = TangentToWorld( ImportanceSampleGGX( E, Roughness1 ).xyz, N );
                L = 2 * dot( V, H ) * H - V;
            }
            else
            {
                H = TangentToWorld( ImportanceSampleGGX( E, Roughness2 ).xyz, N );
                L = 2 * dot( V, H ) * H - V;
            }

            float NoL = saturate( dot(N, L) );
            float NoH = saturate( dot(N, H) );
            float VoH = saturate( dot(V, H) );

            if (NoL > 0 && VoH > 0)
            {
                float3 SampleColor = textureLod( AmbientCubemap, L, 0 ).rgb;

                float PDF[] = float[]
                (
                    NoL / PI, 
                    D_GGX( Roughness1, NoH ) * NoH / (4 * VoH),
                    D_GGX( Roughness2, NoH ) * NoH / (4 * VoH)
                );

                // MIS balance heuristic
                float InvWeight = 0;
                for (uint j = 0; j < 3; ++j)
                {
                    InvWeight += PDF[j] * NumSamples[j];
                }
                float Weight = rcp( InvWeight );

                float3 Shading = StandardShading( GBuffer.DiffuseColor, GBuffer.SpecularColor, float3(GBuffer.Roughness), float3(1), L, V, N );

                Lighting += SampleColor * Shading * ( NoL * Weight );
            }
        }
    }

    return Lighting;
}


float3 PrefilterEnvMap( uint2 Random, float Roughness, float3 R )
{
    float3 FilteredColor = float3(0);
    float Weight = 0;
        
    const uint NumSamples = 128;
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
