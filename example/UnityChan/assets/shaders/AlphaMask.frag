#include "FragmentShaderShadow.inc"

/// Blend On

uniform sampler2D _MainTex;
uniform sampler2D _AlphaMask;

void ps_main(SurfaceData surfaceData)
{
    color.rgb = texture(_MainTex, vs_out.uv*5).rgb;
    color.a = texture(_AlphaMask, vs_out.uv).a;
}