#ifndef Ambient_inc
#define Ambient_inc

#include <BRDF.inc>
#include <ShadingModels.inc>
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

#endif
