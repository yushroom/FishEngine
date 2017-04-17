@vertex
{
	#include <ShaderVariables.inc>
	#include <CG.inc>

	layout (location = PositionIndex) 	in vec3 InputPositon;
	layout (location = NormalIndex)		in vec3 InputNormal;

	uniform mat4 MATRIX_LIGHT_MVP;

	void main()
	{
		vec4 position = vec4(InputPositon, 1);
		vec3 normal = InputNormal;

		//normal = normalize(mat3(MATRIX_IT_MV) * normal);
		//position = MATRIX_LIGHT_MVP * position;
		position = UnityClipSpaceShadowCasterPos(position, normal);
		position = UnityApplyLinearShadowBias(position);
		gl_Position = position;
	}
}

@fragment
{
	void main()
	{
		
	}
}
