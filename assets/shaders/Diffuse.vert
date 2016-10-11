#include "AppDataBase.inc"

out VS_OUT {
    vec3 position;
    vec3 normal;
    vec2 uv;
    vec4 positionInLightSpace;
} vs_out;

void vs_main(AppData appdata)
{
    gl_Position = MATRIX_MVP * appdata.position;
    vs_out.position = (MATRIX_M * appdata.position).xyz;
    vs_out.normal = mat3(MATRIX_IT_M) * appdata.normal;
    vs_out.uv = appdata.uv;
    vs_out.positionInLightSpace = LightMatrix0 * vec4(vs_out.position, 1.0f);
}