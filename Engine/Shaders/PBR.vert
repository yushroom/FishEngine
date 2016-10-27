#include "AppDataBase.inc"

out VS_OUT {
    vec3 position;
    vec3 normal;
    vec2 uv;
} vs_out;

void vs_main(AppData appdata)
{
    gl_Position = MATRIX_MVP * appdata.position;
    vs_out.position = (MATRIX_M * appdata.position).xyz;
    vs_out.normal = mat3(MATRIX_IT_M) * appdata.normal;
    vs_out.uv = appdata.uv;
}

#include "AppDataBaseMain.inc"