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

struct PerDraw
{
	mat4 MATRIX_MVP;
	mat4 MATRIX_MV;
	mat4 MATRIX_M;
	mat4 MATRIX_IT_MV;
	mat4 MATRIX_IT_M;   // new
};

struct PerFrame
{
	mat4 MATRIX_P;
	mat4 MATRIX_V;
	mat4 MATRIX_I_V;
	mat4 MATRIX_VP;

	vec3 WorldSpaceCameraPos;
	float padding0;
	vec4 WorldSpaceLightPos0;
	vec4 LightColor0;
	//vec4 LightColor;
	mat4 LightMatrix0; // World-to-light matrix. Used to sample cookie & attenuation textures.

	//	 Vertex-lit rendering
	//vec3 unity_LightPosition; // view space light position
 	
 	// Time (t = time since current level load) values from Unity
 	vec4 _Time; // (t/20, t, t*2, t*3)
};

#define MAX_BONE_SIZE 128
struct Bones
{
    mat4 BoneTransformations[MAX_BONE_SIZE];
};

#undef mat4
#undef vec3
#undef vec4