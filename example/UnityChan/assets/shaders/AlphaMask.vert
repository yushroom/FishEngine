#include "UnitySupport.inc"
#include "AppDataBase.inc"

out VS_OUT {
    vec2 uv;
    vec3 normal;
} vs_out;

void vs_main(AppData appdata)
{
    gl_Position = MATRIX_MVP * appdata.position;
    vs_out.uv = appdata.uv;
    vs_out.normal = normalize((Object2World * vec4(appdata.normal, 0)).xyz);
}

#include "AppDataBaseMain.inc"