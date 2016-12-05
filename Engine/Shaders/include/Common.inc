#ifndef Common_inc
#define Common_inc

// (x, y) bottom left corner position (in clip space, (-1, 1))
// (z, w) width and height, in range [0, 1]
// eg. (-1, -1, 1, 1) for full sreen quad
uniform vec4 DrawRectParameters;

// Screen Aligned Quad
vec4 DrawRect(in vec2 InPosition)
{
	return vec4( (InPosition - DrawRectParameters.xy) * DrawRectParameters.zw + DrawRectParameters.xy ,0, 1 );
}

#endif
