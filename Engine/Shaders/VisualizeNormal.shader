#pragma GeometryShader On

struct VS_OUT {
    vec3 normal;
};

#ifdef VERTEX_SHADER
	#include "AppData.inc"

	out VS_OUT vs_out;

	void vs_main(AppData appdata)
	{
	    gl_Position = MATRIX_MVP * appdata.position;
	    mat3 normalMatrix = mat3(MATRIX_IT_M);
	    vs_out.normal = normalize(vec3(MATRIX_VP * vec4(normalMatrix*appdata.normal, 0.0)));
	}
#endif /* VERTEX_SHADER */

#ifdef GEOMETRY_SHADER
	layout (triangles) in;
	layout (line_strip, max_vertices = 6) out;

	in VS_OUT gs_in[];

	const float MAGNITUDE = 0.01f;

	void GenerateLine(int index) {
	    gl_Position = gl_in[index].gl_Position;
	    EmitVertex();
	    gl_Position = gl_in[index].gl_Position + vec4(gs_in[index].normal, 0.f) * MAGNITUDE;
	    EmitVertex();
	    EndPrimitive();
	}

	void main()
	{
	    GenerateLine(0);
	    GenerateLine(1);
	    GenerateLine(2);
	}
#endif /* GEOMETRY_SHADER */

#ifdef FRAGMENT_SHADER
	out vec4 color;
	void main()
	{
	    color = vec4(1.0, 0.5, 1.0, 1.0);
	}
#endif