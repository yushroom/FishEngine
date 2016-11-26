struct VS_OUT
{
	vec2 uv;
};

@vertex
{
	#include <CG.inc>

	layout (location = PositionIndex) in vec3 position;
	layout (location = UVIndex) in vec2 uv;

	out VS_OUT vs_out;

	void main()
	{
	    gl_Position = vec4(position.x, position.y, 0.f, 1.f);
	    vs_out.uv = uv;
	}
}

@fragment
{
	in VS_OUT vs_out;

	out vec4 color;

	uniform sampler2D screenTexture;

	void main()
	{
	    color = texture(screenTexture, vs_out.uv);
	}
}