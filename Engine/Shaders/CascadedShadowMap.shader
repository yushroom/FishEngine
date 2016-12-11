#include <ShaderVariables.inc>
#include <ShadowCommon.inc>

@vertex
{
	layout (location = PositionIndex) 	in vec3 InputPositon;

	uniform mat4 ObjectToWorld;

	void main()
	{
		gl_Position = ObjectToWorld * vec4(InputPositon, 1);
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