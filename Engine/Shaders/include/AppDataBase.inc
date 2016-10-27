#ifndef AppDataBase_inc
#define AppDataBase_inc

#ifdef AppDataTan_inc
#error You can inclue only *one* header from AppDataBase.h and AppDataTan.h
#endif

#include "CG.inc"

layout (location = PositionIndex) 	in vec3 InputPositon;
layout (location = NormalIndex) 	in vec3 InputNormal;
layout (location = UVIndex) 		in vec2 InputUV;
#ifdef SKINNED
	layout (location = BoneIndexIndex) 	in ivec4 boneIndex;
	layout (location = BoneWeightIndex) in vec4 boneWeight;
	
	#define MAX_BONE_SIZE 100
	uniform mat4 BoneTransformations[MAX_BONE_SIZE];
#endif

struct AppData {
    vec4 position;
    vec3 normal;
    vec2 uv;
};

// void vs_main(AppData appdata);

// void main()
// {

//     AppData appdata;
// #ifdef SKINNED
// 	mat4 boneTransformation = BoneTransformations[boneIndex[0]] * boneWeight[0];
// 	boneTransformation += BoneTransformations[boneIndex[1]] * boneWeight[1];
// 	boneTransformation += BoneTransformations[boneIndex[2]] * boneWeight[2];
// 	boneTransformation += BoneTransformations[boneIndex[3]] * boneWeight[3];
//     appdata.position	= boneTransformation * vec4(InputPositon, 1);
//     appdata.normal      = mat3(boneTransformation) * InputNormal;
// #else
// 	appdata.position	= vec4(InputPositon, 1);
//     appdata.normal      = InputNormal;
// #endif
//     appdata.uv			= InputUV;
//     vs_main(appdata);
// }

#endif //AppDataBase_inc

// your implementation of vs_main