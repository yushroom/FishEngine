in VS_OUT {
    vec3 position;
    vec3 normal;
    vec2 uv;
    vec4 positionInLightSpace;
} vs_out;

uniform sampler2D diffuseMap;
uniform sampler2D shadowMap;
const float bias = 0.005f;

out vec4 color;

// http://learnopengl.com/#!Advanced-Lighting/Shadows/Shadow-Mapping
float ShadowCalculation(vec4 fragPosLightSpace, vec3 lightDir)
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
    vec3 normal = normalize(vs_out.normal);
    //vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), bias);
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

void main()
{
	vec3 L = -WorldSpaceLightPos0.xyz;
    if (WorldSpaceLightPos0.w > 0.5f) {
        L = normalize(WorldSpaceLightPos0.xyz - vs_out.position);
    }
    float nDotL = dot(normalize(vs_out.normal), L);
    nDotL = clamp(nDotL, 0.0f, 1.0f);

    float shadowFactor = ShadowCalculation(vs_out.positionInLightSpace, L);
	color = (1.f-shadowFactor) * nDotL * texture(diffuseMap, vs_out.uv);
	color.a = 1.0f;
}
