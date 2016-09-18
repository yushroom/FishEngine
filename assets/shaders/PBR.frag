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

float rcp(float x) {
    return 1.0 / x;
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

// l: *normalized* lightDir
// v: *normalized* viewDir
// n: *normalized* normal
// return: D(h)*F(v, h)*G(l, v, h) / (4*dot(n, l)*dot(n, v))
vec3 PRBLighting(vec3 l, vec3 v, vec3 n)
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

const vec3 light_pos = vec3(10, 10, 10);

void main()
{
    vec3 l = normalize(light_pos - vs_out.position);
    vec3 v = normalize(_WorldSpaceCameraPos - vs_out.position);
    vec3 n = normalize(vs_out.normal);
    color.rgb = PRBLighting(l, v, n);
    //color.rgb = SpecularIBL(uint2(311010, 3671), vec3(0.7, 1.0, 0.7), roughness, n, v);
    color.a = 1.0f;
}