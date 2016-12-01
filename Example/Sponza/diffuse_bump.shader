@shadow off

#define NO_DEFAULT_MATERIAL
#include "Standard.shader"

@fragment
{
	uniform sampler2D DiffuseTex;

	FGBufferData CalcGBuffer(VS_OUT v2f)
	{
		FGBufferData GBuffer;
		GBuffer.WorldNormal = v2f.normal;

		GBuffer.Depth		= distance(v2f.position, WorldSpaceCameraPos.xyz) / ProjectionParams.z;
		GBuffer.BaseColor	= texture(DiffuseTex, v2f.uv).rgb;
		GBuffer.Metallic 	= 0;
		GBuffer.Roughness 	= Roughness;
		GBuffer.Specular 	= Specular;
		return GBuffer;
	}
}
