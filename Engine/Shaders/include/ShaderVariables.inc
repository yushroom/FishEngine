#ifndef ShaderVariables_inc
#define ShaderVariables_inc

#define PositionIndex 0
#define NormalIndex 1
#define UVIndex 2
#define TangentIndex 3
#define BoneIndexIndex 4
#define BoneWeightIndex 5

// layout(std140, row_major) uniform PerCamera
// {
// 	// Time (t = time since current level load) values from Unity
// 	//vec4 _Time; // (t/20, t, t*2, t*3)
// 	//vec4 _SinTime; // sin(t/8), sin(t/4), sin(t/2), sin(t)
// 	//vec4 _CosTime; // cos(t/8), cos(t/4), cos(t/2), cos(t)
// 	//vec4 unity_DeltaTime; // dt, 1/dt, smoothdt, 1/smoothdt

// 	// x = 1 or -1 (-1 if projection is flipped)
// 	// y = near plane
// 	// z = far plane
// 	// w = 1/far plane
// 	//vec4 _ProjectionParams;
	
// 	// x = width
// 	// y = height
// 	// z = 1 + 1.0/width
// 	// w = 1 + 1.0/height
// 	//vec4 _ScreenParams;
	
// 	// Values used to linearize the Z buffer (http://www.humus.name/temp/Linearize%20depth.txt)
// 	// x = 1-far/near
// 	// y = far/near
// 	// z = x/far
// 	// w = y/far
// 	//vec4 _ZBufferParams;

// 	// x = orthographic camera's width
// 	// y = orthographic camera's height
// 	// z = unused
// 	// w = 1.0 if camera is ortho, 0.0 if perspective
// 	//vec4 unity_OrthoParams;

// 	vec3 _WorldSpaceCameraPos;
// };


layout(std140, row_major) InternalUniform PerDraw
{
	mat4 MATRIX_MVP;
	mat4 MATRIX_MV;
	mat4 MATRIX_M;
	mat4 MATRIX_IT_MV;
	mat4 MATRIX_IT_M;   // new
};

layout(std140, row_major) InternalUniform PerFrame
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


// #define MAX_BONE_SIZE 100
// //uniform mat4 BoneTransformations[MAX_BONE_SIZE];
// layout(std140, row_major) uniform Bones
// {
//     mat4 BoneTransformations2[MAX_BONE_SIZE];
// };

#define PI 3.141592653589793f
#define INV_PI 0.3183098861837907f

// Camera and screen
//uniform vec3 WorldSpaceCameraPos;

//uniform vec4 LightColor0;
//uniform vec4 WorldSpaceLightPos0;	//Directional lights: (world space direction, 0). Other lights: (world space position, 1).

//	 Deffered shading/lighting
InternalUniform vec4 LightColor;
//uniform mat4 LightMatrix0; // World-to-light matrix. Used to sample cookie & attenuation textures.

//	 Vertex-lit rendering
InternalUniform vec3 unity_LightPosition; // view space light position

#endif // ShaderVariables_inc
