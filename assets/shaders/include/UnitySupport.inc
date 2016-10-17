#ifndef UnitySupport_inc
#define UnitySupport_inc

#include <CGSupport.inc>

#define UNITY_PI	3.14159265359f

// http://docs.unity3d.com/Manual/SL-UnityShaderVariables.html
#define glstate_matrix_mvp MATRIX_MVP

// uniform mat4 UNITY_MATRIX_MVP;  // Current model * view * projection matrix.
// uniform mat4 UNITY_MATRIX_MV;   // Current model * view matrix.
// uniform mat4 UNITY_MATRIX_V;    // Current view matrix.
// uniform mat4 UNITY_MATRIX_P;    // Current projection matrix.
// uniform mat4 UNITY_MATRIX_VP;   // Current view * projection matrix.
// uniform mat4 UNITY_MATRIX_T_MV; // Transpose of model * view matrix.
// uniform mat4 UNITY_MATRIX_IT_MV;// Inverse transpose of model * view matrix.
// uniform mat4 _Object2World;     // Current model matrix.
// uniform mat4 _World2Object;     // Inverse of current world matrix.


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

// UnituShaderVariables.cginc
#define UNITY_MATRIX_P glstate_matrix_projection
#define UNITY_MATRIX_V unity_MatrixV
#define UNITY_MATRIX_I_V unity_MatrixInvV
#define UNITY_MATRIX_VP unity_MatrixVP
#define UNITY_MATRIX_M unity_ObjectToWorld


// Tranforms position from object to homogenous space
float4 UnityObjectToClipPos( in float3 pos )
{
	return mul(UNITY_MATRIX_MVP, float4(pos, 1.0));
}

float4 UnityObjectToClipPos(float4 pos) // overload for float4; avoids "implicit truncation" warning for existing shaders
{
	return UnityObjectToClipPos(pos.xyz);
}


#endif // UnitySupport_inc
