#include "FragmentShaderShadow.inc"

uniform sampler2D _MainTex;

void ps_main(SurfaceData surfaceData)
{
    float nDotL = dot(surfaceData.N, surfaceData.L);
    nDotL = clamp(nDotL, 0.0f, 1.0f);
    vec4 diffuse = texture(_MainTex, surfaceData.uv);
    color = vec4(diffuse.rgb * nDotL, 1);
}
