@zwrite off
@cull off

@vertex
{
	#include <ShaderVariables.inc>

	layout (location = PositionIndex) 	in vec3 InputPosition;
	layout (location = UVIndex) 		in vec2 InputUV;

	out vec2 UV;

	void main()
	{
		gl_Position = vec4(InputPosition.x, InputPosition.y, 0.f, 1.f);
		UV = InputUV * vec2(1, -1);
	}
}


#define KERNEL_3x3_GAUSSIAN 1 
#define KERNEL_7x7_GAUSSIAN 2
#define KERNEL_11x11_GAUSSIAN 3
#define KERNEL_15x15_GAUSSIAN 4

#ifndef KERNEL
	#define KERNEL KERNEL_7x7_GAUSSIAN
#endif

#if KERNEL == KERNEL_3x3_GAUSSIAN
	float offsets[KERNEL] = float[KERNEL]( 0.0174168 );
	float weights[KERNEL] = float[KERNEL]( 0.5 );
#elif KERNEL == KERNEL_7x7_GAUSSIAN
	float offsets[KERNEL] = float[KERNEL]( 0.538049, 2.06278 );
	float weights[KERNEL] = float[KERNEL]( 0.44908, 0.0509202 );
#elif KERNEL == KERNEL_11x11_GAUSSIAN
	float offsets[KERNEL] = float[KERNEL]( 0.621839, 2.2731, 4.14653 );
	float weights[KERNEL] = float[KERNEL]( 0.330228, 0.157012, 0.0127605 );
#elif KERNEL == KERNEL_15x15_GAUSSIAN
	float offsets[KERNEL] = float[KERNEL]( 0.644342, 2.37885, 4.29111, 6.21661 );
	float weights[KERNEL] = float[KERNEL]( 0.249615, 0.192463, 0.0514763, 0.00644572 );
#endif


@fragment
{
	in vec2 UV;

	out vec4 FragColor;
	
	uniform sampler2D _MainTex;
	uniform vec2 Direction;	// (1, 0) * InvWindowSize or (0, 1) * InvWindowSize;

	void main()
	{
		FragColor = vec4(0, 0, 0, 1);
		for (int i = 0; i < KERNEL; ++i)
		{
			vec2 Offset = offsets[i] * Direction;
			vec4 Color = texture( _MainTex, UV + Offset ) + texture( _MainTex, UV - Offset );
			FragColor.rgb += Color.rgb * weights[i];
		}
	}
}