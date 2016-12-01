@deferred

@vertex
{
	#define USE_DEFAULT_VS
	#include <AppData.inc>
}

@fragment
{
	#include <DeferredShadingCommon.inc>

	struct VS_OUT
	{
	    vec3 position;      // in world space
	    vec3 normal;        // in world space
	    vec3 tangent;
	    vec2 uv;
	};

	in VS_OUT vs_out;

	layout(location = 0) out vec4 MRT0;
	layout(location = 1) out vec4 MRT1;
	layout(location = 2) out vec4 MRT2;

	uniform float3	BaseColor;
	uniform float   Metallic;
	uniform float   Roughness;
	uniform float   Specular;

#ifdef NO_DEFAULT_MATERIAL
	FGBufferData CalcGBuffer(VS_OUT v2f);
#else
	FGBufferData CalcGBuffer(VS_OUT v2f)
	{
		FGBufferData GBuffer;
		GBuffer.WorldNormal = v2f.normal;

		GBuffer.BaseColor	= BaseColor;
		GBuffer.Metallic 	= Metallic;
		GBuffer.Roughness 	= Roughness;
		GBuffer.Specular 	= Specular;
		return GBuffer;
	}
#endif

	void main()
	{
		FGBufferData GBuffer = CalcGBuffer(vs_out);
		EncodeGBuffer(GBuffer, MRT0, MRT1, MRT2);
	}
}