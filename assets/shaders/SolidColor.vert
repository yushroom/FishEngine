#include "AppDataBase.inc"

void vs_main(AppData appdata)
{
    gl_Position = MATRIX_MVP * appdata.position;
}