//#include <ShaderVariables.inc>

#ifdef VERTEX_SHADER
	layout (location = 0) 	in vec3 InputPositon;

	uniform mat4 MATRIX_LIGHT_MVP;

	void main()
	{
	    gl_Position = MATRIX_LIGHT_MVP * vec4(InputPositon, 1);
	}
#endif

#ifdef FRAGMENT_SHADER
	void main()
	{
	    
	}
#endif