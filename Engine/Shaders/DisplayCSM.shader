@shadow off
@cull off
@zwrite off

#include <Common.inc>

struct V2F
{
	vec2 UV;
};

@vertex
{
	#include <CG.inc>

	layout (location = PositionIndex) 	in vec3 InputPosition;
	layout (location = UVIndex) 		in vec2 InputUV;

	//out V2F v2f;
	out vec2 UV;

	void main()
	{
	    gl_Position = DrawRect(InputPosition.xy);
	    UV = InputUV * vec2(1, -1);
	}
}

@fragment
{
	in vec2 UV;

	out vec4 FragColor;

	uniform float Section;
	uniform sampler2DArray _MainTex;

	void main()
	{
	    float d = texture(_MainTex, vec3(UV, Section)).r;
		FragColor = vec4(vec3(d), 1);
	}
}