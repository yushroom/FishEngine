#ifndef SkinnedPass_inc
#define SkinnedPass_inc

#include <CG.inc>

layout (location = PositionIndex) 	in vec3 InputPositon;
//layout (location = NormalIndex) 	in vec3 InputNormal;
layout (location = BoneIndexIndex) 	in ivec4 boneIndex;
layout (location = BoneWeightIndex) in vec4 boneWeight;

#define MAX_BONE_SIZE 100
uniform mat4 BoneTransformations[MAX_BONE_SIZE];

out vec3 position;


void main()
{
	mat4 boneTransformation = BoneTransformations[boneIndex[0]] * boneWeight[0];
	boneTransformation += BoneTransformations[boneIndex[1]] * boneWeight[1];
	boneTransformation += BoneTransformations[boneIndex[2]] * boneWeight[2];
	boneTransformation += BoneTransformations[boneIndex[3]] * boneWeight[3];
    position	       = (boneTransformation * vec4(InputPositon, 1)).xyz;
    //normal      = mat3(boneTransformation) * InputNormal;
}

#endif //SkinnedPass_inc