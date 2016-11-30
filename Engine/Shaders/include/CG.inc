#ifndef CG_inc
#define CG_inc

#include <ShaderVariables.inc>

vec4 ObjectToViewPos( in vec3 pos )
{
	return MATRIX_MV * vec4(pos, 1.0);
}

vec4 ObjectToViewPos( in vec4 pos )
{
	return ObjectToViewPos( pos.xyz );
}

vec4 ObjectToClipPos( in vec3 pos )
{
	return MATRIX_MVP * vec4(pos, 1.0);
}

vec4 ObjectToClipPos( vec4 pos )
{
	return ObjectToClipPos(pos.xyz);
}

vec4 WorldToClipPos( in vec3 pos)
{
	return MATRIX_VP * vec4(pos, 1.0);
}

vec4 ViewToClipPos( in vec3 pos )
{
	return MATRIX_P * vec4(pos, 1.0);
}

vec3 WorldSpaceLightDir( in vec3 worldPos )
{
	return WorldSpaceLightPos.xyz - worldPos * WorldSpaceLightPos.w;
}

vec3 WorldSpaceViewDir( in vec3 worldPos )
{
    return WorldSpaceCameraPos.xyz - worldPos;
}

#endif /* CG_inc */

