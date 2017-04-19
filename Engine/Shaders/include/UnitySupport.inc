#ifndef UnitySupport_inc
#define UnitySupport_inc

#include <CGSupport.inc>
#include <ShaderVariables.inc>

#define UNITY_PI	3.14159265359f

// http://docs.unity3d.com/Manual/SL-UnityShaderVariables.html
#define glstate_matrix_mvp MATRIX_MVP

// #define UNITY_MATRIX_P glstate_matrix_projection
// #define UNITY_MATRIX_V unity_MatrixV
// #define UNITY_MATRIX_I_V unity_MatrixInvV
// #define UNITY_MATRIX_VP unity_MatrixVP
// #define UNITY_MATRIX_M unity_ObjectToWorld
// #define UNITY_MATRIX_MVP glstate_matrix_mvp
// #define UNITY_MATRIX_MV glstate_matrix_modelview0
// #define UNITY_MATRIX_T_MV glstate_matrix_transpose_modelview0
// #define UNITY_MATRIX_IT_MV glstate_matrix_invtrans_modelview0
#define UNITY_MATRIX_MVP MATRIX_MVP
#define Object2World MATRIX_M
#define unity_ObjectToWorld Object2World
#define unity_WorldToObject MATRIX_IT_M
#define unity_MatrixVP MATRIX_VP

// UnituShaderVariables.cginc
#define UNITY_MATRIX_P glstate_matrix_projection
#define UNITY_MATRIX_V unity_MatrixV
#define UNITY_MATRIX_I_V unity_MatrixInvV
#define UNITY_MATRIX_VP unity_MatrixVP
#define UNITY_MATRIX_M unity_ObjectToWorld

#define _WorldSpaceCameraPos WorldSpaceCameraPos

#define _WorldSpaceLightPos0 WorldSpaceLightPos
#define _LightColor0 LightColor

#define WorldSpaceLightPos0 WorldSpaceLightPos
#define LightColor0 LightColor


// no keyword inline in glsl
#define inline

// #if defined(UNITY_REVERSED_Z)
// #define UNITY_NEAR_CLIP_VALUE (1.0)
// #elif defined(SHADER_API_D3D9)  || defined(SHADER_API_WIIU) || defined(SHADER_API_D3D11_9X)
// #define UNITY_NEAR_CLIP_VALUE (0.0)
// #else
// #define UNITY_NEAR_CLIP_VALUE (-1.0)
// #endif


#endif // UnitySupport_inc
