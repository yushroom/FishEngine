#include "AppData.inc"

void vs_main(AppData appdata)
{
    gl_Position = MATRIX_MVP * appdata.position;
    vs_out.position = (MATRIX_M * appdata.position).xyz;
    vs_out.normal = normalize(mat3(MATRIX_IT_M) * appdata.normal);
    vs_out.uv = appdata.uv;
}