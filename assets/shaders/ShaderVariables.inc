#version 410 core
#define PositionIndex 0
#define NormalIndex 1
#define UVIndex 2
#define TangentIndex 3

#define PI 3.141592653589793f
#define INV_PI 0.3183098861837907f

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
uniform vec4 _LightColor0;
uniform vec4 _WorldSpaceLightPos0;	//Directional lights: (world space direction, 0). Other lights: (world space position, 1).

//	 Deffered shading/lighting
uniform vec4 _LightColor;
uniform mat4 _LightMatrix0; // World-to-light matrix. Used to sample cookie & attenuation textures.

//	 Vertex-lit rendering
uniform vec3 unity_LightPosition; // view space light position

// HLSL
#define float2 vec2
#define float3 vec3
#define float4 vec4
#define uint2 uvec2
#define uint3 uvec3
#define uint4 uvec4
#define lerp(x, y, t) mix(x, y, t)
#define frac(x) fract(x)
#define saturate(x) clamp(x, 0.0, 1.0)

