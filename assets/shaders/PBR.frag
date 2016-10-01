#include "HLSLSupport.inc"
#define half float
#define half2 vec2
#define half3 vec3
#define half4 vec4
in VS_OUT {
    vec3 position;
    vec3 normal;
    vec2 uv;
} vs_out;

out vec4 color;

uniform vec3 albedo;
uniform float metallic;
//uniform float specular;
uniform float roughness;
//uniform float F0; // = pow((ior-1)/(ior+1), 2);
uniform float specular;

uniform samplerCube AmbientCubemap;

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

// Smith term for GGX
// [Smith 1967, "Geometrical shadowing of a random rough surface"]
float Vis_Smith( float Roughness, float NoV, float NoL )
{
    float a = Square( Roughness );
    float a2 = a*a;

    float Vis_SmithV = NoV + sqrt( NoV * (NoV - NoV * a2) + a2 );
    float Vis_SmithL = NoL + sqrt( NoL * (NoL - NoL * a2) + a2 );
    return rcp( Vis_SmithV * Vis_SmithL );
}

// Appoximation of joint Smith term for GGX
// [Heitz 2014, "Understanding the Masking-Shadowing Function in Microfacet-Based BRDFs"]
float Vis_SmithJointApprox( float Roughness, float NoV, float NoL )
{
    float a = Square( Roughness );
    float Vis_SmithV = NoL * ( NoV * ( 1 - a ) + a );
    float Vis_SmithL = NoV * ( NoL * ( 1 - a ) + a );
    return 0.5 * rcp( Vis_SmithV + Vis_SmithL );
}

// [Schlick 1994, "An Inexpensive BRDF Model for Physically-Based Rendering"]
float3 F_Schlick( float3 SpecularColor, float VoH )
{
    float Fc = Pow5( 1 - VoH );                 // 1 sub, 3 mul
    //return Fc + (1 - Fc) * SpecularColor;     // 1 add, 3 mad
    
    // Anything less than 2% is physically impossible and is instead considered to be shadowing
    return saturate( 50.0 * SpecularColor.g ) * Fc + (1 - Fc) * SpecularColor;
    
}

float3 F_Fresnel( float3 SpecularColor, float VoH )
{
    float3 SpecularColorSqrt = sqrt( clamp( float3(0, 0, 0), float3(0.99, 0.99, 0.99), SpecularColor ) );
    float3 n = ( 1 + SpecularColorSqrt ) / ( 1 - SpecularColorSqrt );
    float3 g = sqrt( n*n + VoH*VoH - 1 );
    return 0.5 * Square( (g - VoH) / (g + VoH) ) * ( 1 + Square( ((g+VoH)*VoH - 1) / ((g-VoH)*VoH + 1) ) );
}


float3 SpecularIBL( uint2 Random, float3 SpecularColor, float Roughness, float3 N, float3 V )
{
    float3 SpecularLighting = float3(0);

    const uint NumSamples = 32;
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
            float3 SampleColor = textureLod( AmbientCubemap, L, 0 ).rgb;
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
            FilteredColor += textureLod( AmbientCubemap, L, 0 ).rgb * NoL;
            //FilteredColor += AmbientCubemap.SampleLevel( AmbientCubemapSampler, L, 0 ).rgb * NoL;
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

// half3 EnvBRDFApprox( half3 SpecularColor, half Roughness, half NoV )
// {
//     // [ Lazarov 2013, "Getting More Physical in Call of Duty: Black Ops II" ]
//     // Adaptation to fit our G term.
//     const half4 c0 = { -1, -0.0275, -0.572, 0.022 };
//     const half4 c1 = { 1, 0.0425, 1.04, -0.04 };
//     half4 r = Roughness * c0 + c1;
//     half a004 = min( r.x * r.x, exp2( -9.28 * NoV ) ) * r.x + r.y;
//     half2 AB = half2( -1.04, 1.04 ) * a004 + r.zw;

// //#if !(ES2_PROFILE || ES3_1_PROFILE)
//     // Anything less than 2% is physically impossible and is instead considered to be shadowing
//     // In ES2 this is skipped for performance as the impact can be small
//     // Note: this is needed for the 'specular' show flag to work, since it uses a SpecularColor of 0
//     AB.y *= saturate( 50.0 * SpecularColor.g );
// //#endif

//     return SpecularColor * AB.x + AB.y;
// }

// l: *normalized* lightDir
// v: *normalized* viewDir
// n: *normalized* normal
// return: D(h)*F(v, h)*G(l, v, h) / (4*dot(n, l)*dot(n, v))
vec3 PRBShading(vec3 l, vec3 v, vec3 n)
{

    float ior = specular * 0.8f + 1.0f;
    float F0 = Square((ior - 1)/(ior + 1));
    
    vec3 h = normalize(l + v);
    float nDoth = clamp(dot(n, h), 0.0, 1.0);
    float lDoth = clamp(dot(l, h), 0.0, 1.0);
    float nDotv = clamp(dot(n, v), 0.0, 1.0);
    float nDotl = clamp(dot(n, l), 0.0, 1.0);
    
    // GGX D
    float alpha = roughness*roughness;
    float alphaSqure = alpha*alpha;
    float k = (nDoth*nDoth*(alphaSqure-1)+1);
    float D = alphaSqure / (PI*k*k);
    
    // F: Fresnel (Schlic)
    k = 1 - lDoth;
    float k2 = k*k;
    float F = F0 + (1-F0)*k2*k2*k;
    
    // Schlick G
    //k = alpha / 2.0;
    k = (roughness+1)*(roughness+1)/8;  // UE4
    float G = 1.0 / ((nDotl*(1-k)+k)*(nDotv*(1-k)+k));
    
    //return D*F*G / (4*nDotl*nDotv);
    vec3 specular = vec3(D*F*G/4);
    float ambient = 0.01f;
    vec3 diffuse = albedo*INV_PI;
    
    return ambient + mix(diffuse, specular, metallic) * nDotl;
}

float3 PBR_BlinnPhong( float3 DiffuseColor, float3 Cspec, float3 LightColor, float3 L, float3 V, float3 N, float Roughness)
{
    float3 H = normalize(L + V);
    float NDotH = saturate(dot(N, H));
    float LDotH = saturate(dot(L, H));
    float NDotL = saturate(dot(N, L));

    float3 F = Cspec + (1 - Cspec)*Pow5(1 - LDotH);
    return (DiffuseColor + (Roughness+2.0f)/8.0f * pow(NDotH, Roughness) * F) * LightColor * NDotL;
}


void main()
{
    vec3 L = -WorldSpaceLightPos0.xyz; // directional light
    if (WorldSpaceLightPos0.w > 0.5f) { // other type
        L = normalize(WorldSpaceLightPos0.xyz - vs_out.position);
    }
    vec3 V = normalize(WorldSpaceCameraPos - vs_out.position);
    vec3 N = normalize(vs_out.normal);
    color.rgb = PRBShading(L, V, N);
    //float3 specularIBL = ApproximateSpecularIBL(uint2(311010, 3671), vec3(0.7, 0.7, 1), roughness, N, V);
    //color.rgb += specularIBL;
    //float3 specularColor = vec3(0.7, 0.7, 1);
    //color.rgb = StandardShading(albedo, specularColor, vec3(0, roughness, 0), vec3(1, 1, 1), L, V, N, vec2(1, 1));
    //color.rgb = SpecularIBL(uint2(311010, 3671), vec3(0.7, 1.0, 0.7), roughness, n, v);
    //float3 SpecularColor = ApproximateSpecularIBL(uint2(311010, 3671), vec3(0.7, 1.0, 0.7), roughness, n, v);
    //color.rgb = SimpleShading(float3(1, 0.5f, 0.5f), SpecularColor, roughness, l, v, n);
    color.a = 1.0f;
}
