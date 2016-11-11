#include <UnrealSupport.inc>

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

uniform vec3 BaseColor;
uniform float Metallic;
uniform float Roughness;
uniform float Specular;

uniform samplerCube RadianceMap;
uniform samplerCube IrradianceMap;


float3 Diffuse_Lambert( float3 DiffuseColor )
{
    return DiffuseColor * (1 / PI);
}

// GGX / Trowbridge-Reitz
// [Walter et al. 2007, "Microfacet models for refraction through rough surfaces"]
float D_GGX( float Roughness, float NoH )
{
    float a = Roughness * Roughness;
    float a2 = a * a;
    float d = ( NoH * a2 - NoH ) * NoH + 1; // 2 mad
    return a2 / ( PI*d*d );                 // 4 mul, 1 rcp
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

half3 EnvBRDFApprox( half3 SpecularColor, half Roughness, half NoV )
{
    // [ Lazarov 2013, "Getting More Physical in Call of Duty: Black Ops II" ]
    // Adaptation to fit our G term.
    const half4 c0 = half4( -1, -0.0275, -0.572, 0.022 );
    const half4 c1 = half4( 1, 0.0425, 1.04, -0.04 );
    half4 r = Roughness * c0 + c1;
    half a004 = min( r.x * r.x, exp2( -9.28 * NoV ) ) * r.x + r.y;
    half2 AB = half2( -1.04, 1.04 ) * a004 + r.zw;

#if !(ES2_PROFILE || ES3_1_PROFILE)
    // Anything less than 2% is physically impossible and is instead considered to be shadowing
    // In ES2 this is skipped for performance as the impact can be small
    // Note: this is needed for the 'specular' show flag to work, since it uses a SpecularColor of 0
    AB.y *= saturate( 50.0 * SpecularColor.g );
#endif

    return SpecularColor * AB.x + AB.y;
}


half EnvBRDFApproxNonmetal( half Roughness, half NoV )
{
    // Same as EnvBRDFApprox( 0.04, Roughness, NoV )
    const half2 c0 = half2( -1, -0.0275 );
    const half2 c1 = half2( 1, 0.0425 );
    half2 r = Roughness * c0 + c1;
    return min( r.x * r.x, exp2( -9.28 * NoV ) ) * r.x + r.y;
}


// @param DiffSpecMask .r: diffuse, .g:specular e.g. float2(1,1) for both, float2(1,0) for diffuse only
float3 StandardShading( float3 DiffuseColor, float3 SpecularColor, float3 LobeRoughness, float3 LobeEnergy, float3 L, float3 V, half3 N, float2 DiffSpecMask )
{
	float3 H = normalize(V + L);
	float NoL = saturate( dot(N, L) );
	float NoV = saturate( abs( dot(N, V) ) + 1e-5 );
	float NoH = saturate( dot(N, H) );
	float VoH = saturate( dot(V, H) );
	
	// Generalized microfacet specular
	float D = D_GGX( LobeRoughness[1], NoH ) * LobeEnergy[1];
	float Vis = Vis_SmithJointApprox( LobeRoughness[1], NoV, NoL );
	float3 F = F_Schlick( SpecularColor, VoH );

	float3 Diffuse = Diffuse_Lambert( DiffuseColor );
	//float3 Diffuse = Diffuse_Burley( DiffuseColor, LobeRoughness[1], NoV, NoL, VoH );
	//float3 Diffuse = Diffuse_OrenNayar( DiffuseColor, LobeRoughness[1], NoV, NoL, VoH );
	
	return Diffuse * (LobeEnergy[2] * DiffSpecMask.r) + (D * Vis * DiffSpecMask.g) * F;
}


// http://the-witness.net/news/2012/02/seamless-cube-map-filtering/
vec3 fix_cube_lookup( vec3 v, float cube_size, float lod )
{
    float M = max(max(abs(v.x), abs(v.y)), abs(v.z));
    float scale = 1 - exp2(lod) / cube_size;
    if (abs(v.x) != M) v.x *= scale;
    if (abs(v.y) != M) v.y *= scale;
    if (abs(v.z) != M) v.z *= scale;
    return v;
}

#if 0

void main()
{
    vec3 L = normalize(WorldSpaceLightDir(vs_out.position));
    vec3 V = normalize(WorldSpaceCameraPos - vs_out.position);
    vec3 N = normalize(vs_out.normal);
    vec3 DiffuseColor = BaseColor - BaseColor * Metallic;
    vec3 SpecularColor = mix( vec3(0.08 * Specular), BaseColor, Metallic);
    //color.rgb = StandardShading(DiffuseColor, SpecularColor, vec3(Roughness, Roughness, Roughness), vec3(1), L, V, N, vec2(1));
    float NoL = saturate( dot(N, L) );
    float NoV = saturate( abs( dot(N, V) ) + 1e-5 );
    //color.rgb *= NoL;

    const int numMips   = 6;
    float mip           = numMips - 1 + log2(Roughness);
    vec3 lookup         = -reflect(V, N);
    lookup              = fix_cube_lookup(lookup, 512, mip);
    vec3 radiance       = pow(textureLod(RadianceMap, lookup, mip).rgb, vec3(2.2));
    vec3 irradiance     = pow(texture(IrradianceMap, N).rgb, vec3(2.2));

    vec3 reflectance    = EnvBRDFApprox(SpecularColor, Roughness, NoV);
    vec3 diffuse        = DiffuseColor * irradiance;
    vec3 specular       = radiance * reflectance;
    color.rgb           = diffuse + specular;
    //SpecularColor = EnvBRDFApprox(SpecularColor, Roughness, NoV);
    //vec3 diffuse = Diffuse

    //color.rgb = StandardShading(DiffuseColor, SpecularColor, vec3(Roughness, Roughness, Roughness), vec3(1), L, V, N, vec2(1));
    color.rgb = pow(color.rgb, vec3(1.0 / 2.2));
    color.a = 1.0f;
}

#elif 1

void main()
{
    vec3 L = normalize(WorldSpaceLightDir(vs_out.position));
    vec3 V = normalize(WorldSpaceCameraPos - vs_out.position);
    vec3 N = normalize(vs_out.normal);
    vec3 DiffuseColor = BaseColor - BaseColor * Metallic;
    vec3 SpecularColor = mix( vec3(0.08 * Specular), BaseColor, Metallic);
    float NoL = saturate( dot(N, L) );
    color.rgb = PI * LightColor0.rgb * NoL * StandardShading(DiffuseColor, SpecularColor, vec3(Roughness, Roughness, Roughness), vec3(1), L, V, N, vec2(1));
    // float NoL = saturate( dot(N, L) );
    // float NoV = saturate( abs( dot(N, V) ) + 1e-5 );
    // color.rgb *= NoL;

    // const int numMips   = 6;
    // float mip           = numMips - 1 + log2(Roughness);
    // vec3 lookup         = -reflect(V, N);
    // lookup              = fix_cube_lookup(lookup, 512, mip);
    // vec3 radiance       = pow(textureLod(RadianceMap, lookup, mip).rgb, vec3(2.2));
    // vec3 irradiance     = pow(texture(IrradianceMap, N).rgb, vec3(2.2));

    // vec3 reflectance    = EnvBRDFApprox(SpecularColor, Roughness, NoV);
    // //vec3 diffuse        = DiffuseColor * irradiance;
    // vec3 specular       = radiance * reflectance;
    // color.rgb           += specular;
    // // color.rgb           = diffuse + specular;
    // // //SpecularColor = EnvBRDFApprox(SpecularColor, Roughness, NoV);
    // // //vec3 diffuse = Diffuse

    // //color.rgb = StandardShading(DiffuseColor, SpecularColor, vec3(Roughness, Roughness, Roughness), vec3(1), L, V, N, vec2(1));
    // color.rgb = pow(color.rgb, vec3(1.0 / 2.2));
    color.a = 1.0f;
}

#else

float3 PBR_BlinnPhong( float3 DiffuseColor, float3 F0, float3 LightColor, float3 L, float3 V, float3 N, float Roughness)
{
    float3 H = normalize(L + V);
    float NDotH = saturate(dot(N, H));
    float LDotH = saturate(dot(L, H));
    float NDotL = saturate(dot(N, L));

    float3 F = F0 + (1 - F0)*Pow5(1 - LDotH);
    return (DiffuseColor + (Roughness+2.0f)/8.0f * pow(NDotH, Roughness) * F) * LightColor * NDotL;
}

void main()
{
    vec3 L = normalize(WorldSpaceLightDir(vs_out.position));
    vec3 V = normalize(WorldSpaceCameraPos - vs_out.position);
    vec3 N = normalize(vs_out.normal);
    vec3 DiffuseColor = BaseColor - BaseColor * Metallic;
    vec3 SpecularColor = mix( vec3(0.08 * Specular), BaseColor, Metallic);

    color.rgb = PBR_BlinnPhong(DiffuseColor, SpecularColor, LightColor0.rgb, L, V, N, Roughness);
    color.a = 1.0;
}

#endif