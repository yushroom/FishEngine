@zwrite off

@vertex
{
	#include <CG.inc>

	layout (location = PositionIndex) 	in vec3 InputPosition;
	layout (location = UVIndex) 		in vec2 InputUV;

	out vec2 UV;

	void main()
	{
	    gl_Position = vec4(InputPosition.x, InputPosition.y, 0.f, 1.f);
	    UV = InputUV * vec2(1, -1);
	}
}

@fragment
{
	in vec2 UV;

	out vec4 FragColor;

	uniform sampler2D _MainTex;
	uniform sampler2D ScreenShadow;

	void main()
	{
		float Shadow = texture(ScreenShadow, UV).r;
	    FragColor = texture(_MainTex, UV);
	    FragColor.rgb *= Shadow;
	    FragColor.a = 1.0;
	}
}