#include "AppDataTan.inc"

out VS_OUT {
    vec3 normal;
    vec3 tangent;
    vec2 uv;
} vs_out;

void vs_main(AppData appdata)
{
    vs_out.normal = mat3(MATRIX_IT_M) * appdata.normal;
    vs_out.tangent = mat3(MATRIX_M) * appdata.tangent;
    vs_out.uv = appdata.uv;
    gl_Position = MATRIX_MVP * appdata.position;
}

#include "AppDataTanMain.inc"