#ifndef AppData_inc
#define AppData_inc

#include <CG.inc>

layout (location = PositionIndex)	in vec3 InputPositon;
layout (location = NormalIndex)		in vec3 InputNormal;
layout (location = TangentIndex)	in vec3 InputTangent;
layout (location = UVIndex)			in vec2 InputUV;

// struct appdata_base
// {
// 	vec4 vertex;
// 	vec3 normal;
// 	vec2 texcoord;
// };

// struct appdata_tan
// {
// 	vec4 vertex;
// 	vec4 tangent;
// 	vec3 normal;
// 	vec2 texcoord;
// }

struct AppData
{
	vec4 position;
	vec3 normal;
	vec3 tangent;
	vec2 uv;
};


#ifdef USE_DEFAULT_VS
struct VS_OUT
{
	vec3 position;      // in world space
	vec3 normal;        // in world space
	vec3 tangent;
	vec2 uv;
};

out VS_OUT vs_out;

void vs_main(AppData appdata)
{
	gl_Position     = MATRIX_MVP * appdata.position;
	vs_out.position = (MATRIX_M * appdata.position).xyz;
	vs_out.normal   = normalize(mat3(MATRIX_IT_M) * appdata.normal);
	vs_out.tangent  = normalize(mat3(MATRIX_IT_M) * appdata.tangent);
	vs_out.uv       = appdata.uv;
}
#else   // !USE_DEFAULT_VS
void vs_main(AppData appdata);
#endif  // USE_DEFAULT_VS

void main()
{
	AppData appdata;
	appdata.position	= vec4(InputPositon, 1);
	appdata.normal      = InputNormal;
	appdata.tangent     = InputTangent;
	appdata.uv			= InputUV;

	vs_main(appdata);
}

#endif /* AppData_inc */

/* your implementation of vs_main */
