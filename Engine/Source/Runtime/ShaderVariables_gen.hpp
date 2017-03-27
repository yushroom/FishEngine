#pragma once
#define mat4 FishEngine::Matrix4x4
#define vec3 FishEngine::Vector3
#define vec4 FishEngine::Vector4

constexpr int PositionIndex = 0;
constexpr int NormalIndex = 1;
constexpr int UVIndex = 2;
constexpr int TangentIndex = 3;
constexpr int BoneIndexIndex = 4;
constexpr int BoneWeightIndex = 5;

struct PerCameraUniforms
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

	vec4 WorldSpaceCameraPos;		// .w is not used
	vec4 WorldSpaceCameraDir;		// forward direction of the camera in world space
};


struct PerDrawUniforms
{
	mat4 MATRIX_MVP;
	mat4 MATRIX_MV;
	mat4 MATRIX_IT_MV;
	mat4 MATRIX_M;		// ObjectToWorld
	mat4 MATRIX_IT_M;   // WorldToObject
};

// layout(std140, row_major) uniform PerFrameUniforms
// {
// };

struct LightingUniforms
{
	vec4 LightColor;
	vec4 WorldSpaceLightPos;

	vec4 CascadesNear;
	vec4 CascadesFar;
	vec4 CascadesSplitPlaneNear;
	vec4 CascadesSplitPlaneFar;
	vec4 fish_LightShadowBias;
	//mat4 LightMatrix; // World-to-light matrix. Used to sample cookie & attenuation textures.
	mat4 LightMatrix[4];
};


#define MAX_BONE_SIZE 128
struct Bones
{
	mat4 BoneTransformations[MAX_BONE_SIZE];
};

#undef mat4
#undef vec3
#undef vec4
