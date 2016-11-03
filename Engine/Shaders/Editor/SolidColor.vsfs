uniform mat4 MATRIX_MVP;
uniform vec4 _Color = vec4(1, 0, 0, 1);

#ifdef VERTEX_SHADER
	layout(location = 0)	in vec3 InputPositon;
	void main()
	{
	    gl_Position = MATRIX_MVP * vec4(InputPositon, 1);
	}
#endif

#ifdef FRAGMENT_SHADER
	out vec4 fragColor;
	void main()
	{
		fragColor = _Color;
	}
#endif
