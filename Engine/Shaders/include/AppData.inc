#ifndef AppData_inc
#define AppData_inc

#include <CG.inc>

layout (location = PositionIndex) 	in vec3 InputPositon;
layout (location = NormalIndex) 	in vec3 InputNormal;
layout (location = TangentIndex)    in vec3 InputTangent;
layout (location = UVIndex) 		in vec2 InputUV;
#ifdef _SKINNED
	layout (location = BoneIndexIndex) 	in ivec4 boneIndex;
	layout (location = BoneWeightIndex) in vec4 boneWeight;
#endif

struct AppData
{
    vec4 position;
    vec3 normal;
    vec3 tangent;
    vec2 uv;
};


#ifdef USE_DEFAULT_VS

    out VS_OUT
    {
    	vec3 position;		// in world space
    	vec3 normal;		// in world space
        vec3 tangent;
    	vec2 uv;
    	//vec4 positionInLightSpace; // light space
    } vs_out;

    void vs_main(AppData appdata)
    {
        gl_Position     = MATRIX_MVP * appdata.position;
        vs_out.position = (MATRIX_M * appdata.position).xyz;
        vs_out.normal   = normalize(mat3(MATRIX_IT_M) * appdata.normal);
        vs_out.tangent  = normalize(mat3(MATRIX_IT_M) * appdata.tangent);
        vs_out.uv       = appdata.uv;
    }
#else   // !USE_DEFAULT_VS
    void vs_main(AppData appdata);
#endif  // USE_DEFAULT_VS

void main()
{
    AppData appdata;
#ifdef _SKINNED
	mat4 boneTransformation = BoneTransformations[boneIndex[0]] * boneWeight[0];
	boneTransformation += BoneTransformations[boneIndex[1]] * boneWeight[1];
	boneTransformation += BoneTransformations[boneIndex[2]] * boneWeight[2];
	boneTransformation += BoneTransformations[boneIndex[3]] * boneWeight[3];
    appdata.position	= boneTransformation * vec4(InputPositon, 1);
    appdata.normal      = mat3(boneTransformation) * InputNormal;
    appdata.tangent     = mat3(boneTransformation) * InputTangent;
#else // !_SKINNED
	appdata.position	= vec4(InputPositon, 1);
    appdata.normal      = InputNormal;
    appdata.tangent     = InputTangent;
#endif // _SKINNED
    appdata.uv			= InputUV;

    vs_main(appdata);
}

#endif /* AppData_inc */

/* your implementation of vs_main */
