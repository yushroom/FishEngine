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
	}
}

@fragment
{
	in V2F v2f;

	out vec4 FragColor;

	uniform sampler2D StencilTexture;	// acturally is depth buffer, arb_stencil_textureing maybe not supported
	uniform sampler2D ColorTexture;
	uniform sampler2D DepthTexture;

	const int Width = 2;	// in pixels
	const float OccludeWeight = 1 - 0.1f;
	const vec4 OutlineColor = vec4(1, 0.65, 0, 1);	// orange
	const int SampleCount = 8; 
	vec2 Samples[SampleCount] = vec2[SampleCount](
		vec2(1, 1), vec2(-1, 1), vec2(1, -1), vec2(-1, -1),
		vec2(1, 0), vec2(-1, 0), vec2(0,  1), vec2(0,  -1)
	);

	void main()
	{
		vec2 InvSize = 1.0 / textureSize(StencilTexture, 0);
		float SceneDepth = texture(DepthTexture, v2f.UV).r;
		vec3 SceneColor = texture(ColorTexture, v2f.UV).rgb;
		FragColor = vec4(SceneColor, 1);
	    float Stencil = texture(StencilTexture, v2f.UV).r;
	    //float factor = 0;

	    if (Stencil == 1.0) // not in selection region
	    {
	    #if 1
	    	for (int i = -Width; i <= Width; ++i)
		    {
		    	for (int j = -Width; j <= Width; ++j)
		    	{
		    		Stencil = texture(StencilTexture, v2f.UV + vec2(i, j)*InvSize).r;
		    		if (Stencil != 1.0)
		    		{
		    			FragColor = OutlineColor;
		    			if (Stencil > SceneDepth + 0.002f)
		    			{
		    				FragColor.rgb = mix(FragColor.rgb, SceneColor, OccludeWeight);
		    			}
		    			return;
		    		}
		    	}
		    }
		#else
			for (int i = 0; i < SampleCount; ++i)
			{
				vec2 uv = v2f.UV + Samples[i] * Width * InvSize;
				Stencil = texture(StencilTexture, uv).r;
				if (Stencil != 1.0)
				{
					FragColor = OutlineColor;
	    			if (Stencil > SceneDepth + 0.002f)
	    			{
	    				FragColor.rgb = mix(FragColor.rgb, SceneColor, OccludeWeight);
	    			}
	    			return;
				}
			}
		#endif
	    }
	    //FragColor.rgb = mix(FragColor.rgb, SceneColor, factor);
	}
}