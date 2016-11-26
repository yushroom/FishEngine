@vertex
{
	#include <ShaderVariables.inc>
	layout (location = PositionIndex) 	in vec3 InputPositon;

	#ifdef _SKINNED
		layout (location = BoneIndexIndex) 	in ivec4 boneIndex;
		layout (location = BoneWeightIndex) in vec4 boneWeight;
	#endif

	uniform mat4 MATRIX_LIGHT_MVP;

	void main()
	{
	#ifdef _SKINNED
		mat4 boneTransformation = BoneTransformations[boneIndex[0]] * boneWeight[0];
		boneTransformation += BoneTransformations[boneIndex[1]] * boneWeight[1];
		boneTransformation += BoneTransformations[boneIndex[2]] * boneWeight[2];
		boneTransformation += BoneTransformations[boneIndex[3]] * boneWeight[3];
		vec4 position = boneTransformation * vec4(InputPositon, 1);
	#else // !_SKINNED
		vec4 position = vec4(InputPositon, 1);
	#endif // _SKINNED

		gl_Position = MATRIX_LIGHT_MVP * position;
	}
}

@fragment
{
	void main()
	{
	    
	}
}
