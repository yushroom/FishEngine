@shadow off
#include <ShaderVariables.inc>

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

	//uniform mat4 CascadesViewMatrices[4];
	//uniform mat4 CascadesProjMatrices[4];
	//uniform mat4 CascadesWorldToClip[4];	// VP

	out float 	gLayer;
	//out vec3	vsPosition;
	out float	Depth;

	void main()
	{
		for (int i = 0; i < gl_in.length(); ++i)
		{
			gl_Position = LightMatrix[gl_InvocationID] * gl_in[i].gl_Position;
			//vec4 pos = CascadesViewMatrices[gl_InvocationID] * gl_in[i].gl_Position;
			//gl_Position = CascadesProjMatrices[gl_InvocationID] * pos;
			//vsPosition = pos.xyz;
			Depth = gl_Position.z / gl_Position.w;
			//Depth = gl_Position.z;
			gl_Layer = gl_InvocationID;
			gLayer = float(gl_InvocationID);
			EmitVertex();
		}
		EndPrimitive();
	}
}

@fragment
{
	// uniform vec4 CascadesNear;
	// uniform vec4 CascadesFar;
	//uniform float ExpC;
	const float ExpC = 120.0;

	in float gLayer;
	//in vec3 vsPosition;
	in float Depth;
	out float oDepth;

	void main()
	{
		int layer = int(gLayer);
		//float linearDepth = ( Depth - CascadesNear[layer]) / (CascadesFar[layer] - CascadesNear[layer]);
		//oDepth = linearDepth;
		oDepth = Depth;
		//oDepth = 1.0f;
	}
}