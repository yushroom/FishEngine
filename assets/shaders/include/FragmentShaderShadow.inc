#ifndef FragmentShaderShadow_inc
#define FragmentShaderShadow_inc

#include "CG.inc"

uniform sampler2D shadowMap;
const float bias = 0.0002f;

in VS_OUT {
    vec3 position;      // in world space
    vec3 normal;        // in world space
    vec2 uv;
    vec4 positionInLightSpace; // light space
} vs_out;

out vec4 color;

// http://learnopengl.com/#!Advanced-Lighting/Shadows/Shadow-Mapping
float ShadowCalculation(vec4 fragPosLightSpace, vec3 lightDir, vec3 normal)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // Transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // Get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // Get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // Calculate bias (based on depth map resolution and slope)
    vec3 N = normalize(normal);
    //vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    float bias = max(0.0002 * (1.0 - dot(N, lightDir)), bias);
    // Check whether current frag pos is in shadow
    //float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    
    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    
    // Keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;
        
    return shadow;
}

struct SurfaceData
{
    vec3 L;
    vec3 V;
    vec3 N;
    vec2 uv;
};

void ps_main(SurfaceData surfaceData);

void main()
{
    SurfaceData surfaceData;
    vec3 L = normalize(WorldSpaceLightDir(vs_out.position));
    vec3 V = normalize(WorldSpaceCameraPos - vs_out.position);
    vec3 N = normalize(vs_out.normal);
    surfaceData.L = L;
    surfaceData.V = V;
    surfaceData.N = N;
    surfaceData.uv = vs_out.uv;

    ps_main(surfaceData);

    float shadowFactor = ShadowCalculation(vs_out.positionInLightSpace, L, N);
    color.rgb *= (1.f-shadowFactor);
    //color.a = 1.0f;
}

#endif // FragmentShaderShadow_inc