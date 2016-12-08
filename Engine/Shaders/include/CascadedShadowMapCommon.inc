#ifndef CascadedShadowMapCommon_inc
#define CascadedShadowMapCommon_inc

const float ExpC = 120.0;
const float bias = 0.002f;

uniform sampler2DArray CascadedShadowMap;

vec4 GetCascadeWeights( float Depth )
{
    vec4 near = step( CascadesSplitPlaneNear, vec4(Depth) );
    vec4 far  = step( Depth, CascadesSplitPlaneFar);
    return near * far;
}

float CalcShadowTerm(vec4 WorldPosition, float Depth)
{
    vec4 CascadeWeights = GetCascadeWeights( Depth );
    float fSection = dot(vec4(0, 1, 2, 3), CascadeWeights);
    uint Section = uint(fSection);
    
    vec4 ProjCoords = LightMatrix[Section] * WorldPosition;
    ProjCoords.xyz = ProjCoords.xyz * (0.5 / ProjCoords.w) + 0.5;

    float Z = ProjCoords.z;
    float Occluder = texture( CascadedShadowMap, vec3(ProjCoords.xy, fSection) ).r;

#if 0
    // ESM
    //return clamp( Occluder * exp( -ExpC*Z ), 0.2, 1.0 );
    return clamp( exp( ExpC*(Occluder - Z) ), 0.2, 1.0 );
#elif 0
    // a modified version of ESM
    // http://www.klayge.org/2013/10/07/%E5%88%87%E6%8D%A2%E5%88%B0esm/
    float C = ExpC / (ProjectionParams.z - ProjectionParams.y); //  ExpC / (far - near)
    return clamp( exp( C * (Z - Occluder) ), 0.0, 1.0 );
#else
    float Shadow = 1.0;
    if (Z - bias > Occluder)
        Shadow = 0.2f;
    return Shadow;
#endif
}


float CalcShadowTerm(vec4 WorldPosition)
{
    float Depth = distance(WorldPosition.xyz, WorldSpaceCameraPos.xyz);
    return CalcShadowTerm(WorldPosition, Depth);
}

#endif