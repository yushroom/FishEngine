#ifndef Shadertoy_inc
#define Shadertoy_inc

#include <ShaderVariables.inc>

const vec2 iResolution = vec2(1024, 1024);
#define iGlobalTime (_Time.y)
const vec2 iMouse = vec2(0, 0);

layout (location = PositionIndex) in vec3 position;
layout (location = UVIndex) in vec2 uv;


#ifdef VERTEX_SHADER
	out VS_OUT {
		vec2 uv;
	} vs_out;

	void main()
	{
		gl_Position = vec4(position.x, position.y, 0.f, 1.f);
		vs_out.uv = uv;
	}
#endif

#ifdef FRAGMENT_SHADER
	in VS_OUT {
		vec2 uv;
	} vs_out;

	out vec4 color;

	uniform sampler2D screenTexture;

	void mainImage( out vec4 fragColor, in vec2 fragCoord );

	void main()
	{
		//color = texture(screenTexture, vs_out.uv);
		vec2 uv = vs_out.uv;
		uv.y = 1 - uv.y;
		uv *= iResolution;
		mainImage(color, uv);
	}
#endif

#undef PI

#endif