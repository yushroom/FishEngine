#include <ShaderVariables.inc>
#include <ShadowCommon.inc>

@vertex
{
	layout (location = PositionIndex) 	in vec3 InputPositon;
	#ifdef _SKINNED
		layout (location = BoneIndexIndex) 	in ivec4 boneIndex;
		layout (location = BoneWeightIndex) in vec4 boneWeight;
	#endif

	uniform mat4 ObjectToWorld;

	void main()
	{
		vec4 ObjectPosition = vec4(InputPositon, 1);
		#ifdef _SKINNED
			mat4 boneTransformation = BoneTransformations[boneIndex[0]] * boneWeight[0];
			boneTransformation += BoneTransformations[boneIndex[1]] * boneWeight[1];
			boneTransformation += BoneTransformations[boneIndex[2]] * boneWeight[2];
			boneTransformation += BoneTransformations[boneIndex[3]] * boneWeight[3];
		    ObjectPosition	= boneTransformation * ObjectPosition;
		#endif // _SKINNED
		gl_Position = ObjectToWorld * ObjectPosition;
	}
}

@geometry
{
	layout(triangles, invocations = 4) in;
	layout(triangle_strip, max_vertices = 3) out;
	
	void main()
	{
		for (int i = 0; i < gl_in.length(); ++i)
		{
			gl_Position = LightMatrix[gl_InvocationID] * gl_in[i].gl_Position;
			gl_Layer = gl_InvocationID;
			EmitVertex();
		}
		EndPrimitive();
	}
}

@fragment
{
	void main()
	{
	}
}