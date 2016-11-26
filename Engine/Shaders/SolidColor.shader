@vertex
{
	#define USE_DEFAULT_VS
	#include "AppData.inc"
}

@fragment
{
	uniform vec4 Color = vec4(1, 0, 0, 1);

	out vec4 color;

	void main()
	{
		color = Color;
	}
}