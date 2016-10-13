#include "FragmentShaderShadow.inc"

/// Blend On
in VS_OUT {
    vec2 uv;
    vec3 normal;
} vs_out;


uniform sampler2D _MainTex;
uniform sampler2D _AlphaMask;

void ps_main()
{
    color.rgb = texture(_MainTex, vs_out.uv*5).rgb;
    color.a = texture(_AlphaMask, vs_out.uv).a;
}