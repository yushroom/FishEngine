#version 410 core
#define PositionIndex 0
#define NormalIndex 1
#define UVIndex 2
#define TangentIndex 3

#define PI 3.141592653589793f
#define INV_PI 0.3183098861837907f

// http://docs.unity3d.com/Manual/SL-UnityShaderVariables.html

// Transformations
uniform mat4 MATRIX_MVP;
//uniform mat4 MATRIX_MV;
uniform mat4 MATRIX_V;
uniform mat4 MATRIX_P;
uniform mat4 MATRIX_VP;
//uniform mat4 MATRIX_T_MV;
uniform mat4 MATRIX_IT_MV;
uniform mat4 MATRIX_IT_M;   // new
uniform mat4 _Object2World; // MATRIX_M
//uniform mat4 _World2Object; // MATRIX_I_M

// Camera and screen
uniform vec3 _WorldSpaceCameraPos;
//uniform vec4 _ProjectionParams;
//uniform vec4 _ScreenParams;
//uniform vec4 _ZBufferParams;
//uniform vec4 OrthoParams;
//uniform mat4 CameraProjection;
//uniform mat4 CameraInvProjection;
//uniform vec4 _CameraWorldClipPlanes[6];

// Time
//uniform vec4 _Time;
//uniform vec4 _SinTime;
//uniform vec4 _CosTime;
//uniform vec4 _DeltaTime;

// Lighting
//	 forward rendering

// http://docs.unity3d.com/Manual/SL-UnityShaderVariables.html

uniform vec4 _LightColor0;
uniform vec4 _WorldSpaceLightPos0;	//Directional lights: (world space direction, 0). Other lights: (world space position, 1).

//	 Deffered shading/lighting
uniform vec4 _LightColor;
uniform mat4 _LightMatrix0; // World-to-light matrix. Used to sample cookie & attenuation textures.

//	 Vertex-lit rendering
uniform vec3 unity_LightPosition; // view space light position

// uniform mat4 UNITY_MATRIX_MVP;  // Current model * view * projection matrix.
// uniform mat4 UNITY_MATRIX_MV;   // Current model * view matrix.
// uniform mat4 UNITY_MATRIX_V;    // Current view matrix.
// uniform mat4 UNITY_MATRIX_P;    // Current projection matrix.
// uniform mat4 UNITY_MATRIX_VP;   // Current view * projection matrix.
// uniform mat4 UNITY_MATRIX_T_MV; // Transpose of model * view matrix.
// uniform mat4 UNITY_MATRIX_IT_MV;// Inverse transpose of model * view matrix.
// uniform mat4 _Object2World;     // Current model matrix.
// uniform mat4 _World2Object;     // Inverse of current world matrix.

#define UNITY_MATRIX_MVP MATRIX_MVP
#define unity_ObjectToWorld _Object2World

// UnituShaderVariables.cginc
#define UNITY_MATRIX_P glstate_matrix_projection
#define UNITY_MATRIX_V unity_MatrixV
#define UNITY_MATRIX_I_V unity_MatrixInvV
#define UNITY_MATRIX_VP unity_MatrixVP
#define UNITY_MATRIX_M unity_ObjectToWorld
