@vertex
{
	#include <ShaderVariables.inc>

	layout (location = PositionIndex)	in vec3 InputPositon;
	layout (location = NormalIndex)		in vec3 InputNormal;
	layout (location = TangentIndex)	in vec3 InputTangent;
	layout (location = BoneIndexIndex)	in ivec4 boneIndex;
	layout (location = BoneWeightIndex)	in vec4 boneWeight;

	// #define MAX_BONE_SIZE 128
	// // column_major: macOS bug
	// layout(std140, column_major) uniform Bones
	// {
	// 	mat4 BoneTransformations[MAX_BONE_SIZE];
	// };

	out vec3 OutputPosition;
	out vec3 OutputNormal;
	out vec3 OutputTangent;

	void main()
	{
		mat4 boneTransformation = BoneTransformations[boneIndex[0]] * boneWeight[0];
		boneTransformation += BoneTransformations[boneIndex[1]] * boneWeight[1];
		boneTransformation += BoneTransformations[boneIndex[2]] * boneWeight[2];
		boneTransformation += BoneTransformations[boneIndex[3]] * boneWeight[3];
		OutputPosition = (boneTransformation * vec4(InputPositon, 1)).xyz;
		OutputNormal = mat3(boneTransformation) * InputNormal;
		OutputTangent = mat3(boneTransformation) * InputTangent;
	}
}

@fragment
{
	void main()
	{
		
	}
} 