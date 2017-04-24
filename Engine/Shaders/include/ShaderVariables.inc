#ifndef ShaderVariables_inc
#define ShaderVariables_inc

#define PositionIndex 0
#define NormalIndex 1
#define TangentIndex 2
#define UVIndex 3
#define BoneIndexIndex 4
#define BoneWeightIndex 5

#define CBUFFER_START(name) layout(std140, row_major) uniform name {
#define CBUFFER_END };

layout(std140, row_major) uniform PerCameraUniforms
{
	mat4 MATRIX_P;			// CameraProjection;
	mat4 MATRIX_V;			// WorldToCamera;
	mat4 MATRIX_I_V;		// CameraToWorld;
	mat4 MATRIX_VP;			// CameraProjection * WorldToCamera;

	// Time (t = time since current level load) values from Unity
	vec4 Time; 	// (t/20, t, t*2, t*3)
	//vec4 _SinTime; // sin(t/8), sin(t/4), sin(t/2), sin(t)
	//vec4 _CosTime; // cos(t/8), cos(t/4), cos(t/2), cos(t)
	//vec4 unity_DeltaTime; // dt, 1/dt, smoothdt, 1/smoothdt

	// x = 1 or -1 (-1 if projection is flipped)
	// y = near plane
	// z = far plane
	// w = 1/far plane
	vec4 ProjectionParams;
	
	// x = width
	// y = height
	// z = 1 + 1.0/width
	// w = 1 + 1.0/height
	vec4 ScreenParams;
	
	// Values used to linearize the Z buffer (http://www.humus.name/temp/Linearize%20depth.txt)
	// x = 1-far/near
	// y = far/near
	// z = x/far
	// w = y/far
	vec4 ZBufferParams;

	// x = orthographic camera's width
	// y = orthographic camera's height
	// z = unused
	// w = 1.0 if camera is ortho, 0.0 if perspective
	//vec4 unity_OrthoParams;
	
	vec4 WorldSpaceCameraPos;		// .w = 1, not used
	vec4 WorldSpaceCameraDir;		// .w = 0, not used, forward direction of the camera in world space
};


layout(std140, row_major) uniform PerDrawUniforms
{
	mat4 MATRIX_MVP;
	mat4 MATRIX_MV;
	mat4 MATRIX_IT_MV;
	mat4 MATRIX_M;		// ObjectToWorld
	mat4 MATRIX_IT_M;	// WorldToObject
};

// layout(std140, row_major) uniform PerFrameUniforms
// {
// };

layout(std140, row_major) uniform LightingUniforms
{
	vec4 LightColor;
	vec4 WorldSpaceLightPos;

	vec4 CascadesNear;
	vec4 CascadesFar;
	vec4 CascadesSplitPlaneNear;
	vec4 CascadesSplitPlaneFar;
	vec4 _LightShadowData;
	vec4 unity_LightShadowBias;
	//mat4 LightMatrix; // World-to-light matrix. Used to sample cookie & attenuation textures.
	// macOS bug
	layout(column_major) mat4 LightMatrix[4]; // world -> clip (VP)
};

// CBUFFER_START(UnityShadows)
// 	//float4 unity_ShadowSplitSpheres[4];
// 	//float4 unity_ShadowSplitSqRadii;
// 	float4 unity_LightShadowBias;
// 	float4 _LightSplitsNear;
// 	float4 _LightSplitsFar;
// 	float4x4 unity_WorldToShadow[4];
// 	half4 _LightShadowData;
// 	float4 unity_ShadowFadeCenterAndType;
// CBUFFER_END



#define MAX_BONE_SIZE 128
// column_major: macOS bug
layout(std140, column_major) uniform Bones
{
	mat4 BoneTransformations[MAX_BONE_SIZE];
};


#endif // ShaderVariables_inc
