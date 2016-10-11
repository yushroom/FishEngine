layout (location = PositionIndex) 	in vec3 position;
layout (location = NormalIndex) 	in vec3 normal;
layout (location = UVIndex) 		in vec2 uv;
layout (location = BoneIndexIndex) 	in ivec4 boneIndex;
layout (location = BoneWeightIndex) in vec4 boneWeight;

#define MAX_BONE_SIZE 200
uniform mat4 BoneTransformations[MAX_BONE_SIZE];

out VS_OUT {
	vec3 position;
    vec3 normal;
    vec2 uv;
} vs_out;

void main() {
	mat4 boneTransformation = BoneTransformations[boneIndex[0]] * boneWeight[0];
	boneTransformation += BoneTransformations[boneIndex[1]] * boneWeight[1];
	boneTransformation += BoneTransformations[boneIndex[2]] * boneWeight[2];
	boneTransformation += BoneTransformations[boneIndex[3]] * boneWeight[3];
 
	vec4 posL = boneTransformation * vec4(position, 1);
    gl_Position = MATRIX_MVP * posL;
    vs_out.position = (MATRIX_M * posL).xyz;
    vs_out.normal = mat3(MATRIX_IT_M) * normal;
    vs_out.uv = uv;
}