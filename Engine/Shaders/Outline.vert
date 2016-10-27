#include "AppDataBase.inc"

uniform float _EdgeThickness = 0.5;
// Outline thickness multiplier
#define INV_EDGE_THICKNESS_DIVISOR 0.00285

out VS_OUT {
    vec2 uv;
} vs_out;

void vs_main(AppData appdata)
{
    vec4 projSpacePos = MATRIX_MVP * appdata.position;
    vec4 projSpaceNormal = normalize( MATRIX_MVP * vec4(appdata.normal, 0));
    vec4 scaledNormal = _EdgeThickness * INV_EDGE_THICKNESS_DIVISOR * projSpaceNormal; // * projSpacePos.w;

    scaledNormal.z += 0.00001;
	gl_Position = projSpacePos + scaledNormal;
    vs_out.uv = appdata.uv;
}

#include "AppDataBaseMain.inc"