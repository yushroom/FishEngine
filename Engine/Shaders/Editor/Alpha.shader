//@Blend SrcAlpha OneMinusSrcAlpha
layout(location = 0)	in vec3 InputPositon;
layout(location = 2)	in vec2 InputUV;

uniform mat4 MATRIX_MVP;
uniform sampler2D _MainTex;

@vertex
{
	out vec2 uv;
	void main()
	{
		gl_Position = MATRIX_MVP * vec4(InputPositon, 1);
		uv = InputUV;
	}
}

@fragment
{
	in vec2 uv;
	out vec4 fragColor;
	void main()
	{
		fragColor = texture(_MainTex, uv);
		if (fragColor.a == 0.0)
			discard;
	}
}
