@Name "Hidden/InternalErrorShader"

#include <ShaderVariables.inc>
#include <CG.inc>

layout (location = PositionIndex) 	in vec3 InputPositon;


@vertex
{
	void main()
	{
		gl_Position = UnityObjectToClipPos(InputPositon);
	}
}

@fragment
{
	out float4 OutColor;

	void main()
	{
		OutColor = float4(1, 0, 1, 1);
	}
}