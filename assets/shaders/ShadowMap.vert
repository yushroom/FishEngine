//layout (location = PositionIndex) in vec3 position;

#include "AppDataBase.inc"

uniform mat4 MATRIX_LIGHT_MVP;

void vs_main(AppData appdata)
{
    gl_Position = MATRIX_LIGHT_MVP * appdata.position;
}

#include "AppDataBaseMain.inc"