#include "AppDataBase.inc"

out VS_OUT {
    vec3 normal;
} vs_out;

void vs_main(AppData appdata)
{
    gl_Position = MATRIX_MVP * appdata.position;
    mat3 normalMatrix = mat3(MATRIX_IT_M);
    vs_out.normal = normalize(vec3(MATRIX_VP * vec4(normalMatrix*appdata.normal, 0.0)));
}