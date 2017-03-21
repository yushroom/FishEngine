@shadow off
@cull off

#define NO_DEFAULT_MATERIAL
#include "Standard.shader"

@fragment
{
	uniform sampler2D DiffuseTex;
	uniform sampler2D MaskTex;

	FGBufferData CalcGBuffer(VS_OUT v2f)
	{
		float mask = texture(MaskTex, v2f.uv).r;
    	if (mask < 0.5)
    		discard;
		FGBufferData GBuffer;
		GBuffer.WorldNormal = v2f.normal;

		GBuffer.BaseColor	= texture(DiffuseTex, v2f.uv).rgb;
		GBuffer.Metallic 	= 0;
		GBuffer.Roughness 	= Roughness;
		GBuffer.Specular 	= Specular;
		return GBuffer;
	}
}
