struct V2F
{
	vec2 UV;
};

@vertex
{
	#include <CG.inc>

	layout (location = PositionIndex) 	in vec3 InputPosition;
	layout (location = UVIndex) 		in vec2 InputUV;

	out V2F v2f;

	void main()
	{
	    gl_Position = vec4(InputPosition.x, InputPosition.y, 0.f, 1.f);
	    v2f.UV = InputUV * vec2(1, -1);
	    //gl_Position = ObjectToClipPos(InputPosition);
	    //v2f.UV = InputUV;
	}
}

@fragment
{
	in V2F v2f;

	out vec4 color;

	uniform sampler2D _MainTex;

	void main()
	{
	    color = texture(_MainTex, v2f.UV);
	}
}