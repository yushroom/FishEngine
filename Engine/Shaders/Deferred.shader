@zwrite off
@deferred on

#include <DeferredShadingCommon.inc>
#include <ShadingModels.inc>
//#include <ShadowCommon.inc>

struct V2F
{
	vec2 UV;
	vec3 ScreenVector;
};

@vertex
{
	#include <CG.inc>

	layout (location = PositionIndex) in vec3 InputPosition;
	layout (location = UVIndex) in vec2 InputUV;

	out V2F v2f;

	void main()
	{
	    gl_Position = vec4(InputPosition.x, InputPosition.y, 0.f, 1.f);
	    //gl_Position = ObjectToClipPos(InputPosition);
	    // Screen to world
	    v2f.UV = InputUV * vec2(1, -1);	// OpenGL
	    
		vec4 WorldPosition = inverse(MATRIX_VP) * vec4(InputPosition.x, InputPosition.y, 1.0f, 1.0f);
		// in world space
		v2f.ScreenVector = normalize(WorldPosition.xyz / WorldPosition.w - WorldSpaceCameraPos.xyz);
	}
}

@fragment
{
	#include <Common.inc>

	uniform sampler2D DBufferATexture;
	uniform sampler2D DBufferBTexture;
	uniform sampler2D DBufferCTexture;

	vec4 GetDynamicLighting(vec3 WorldPosition, vec3 CameraVector, FGBufferData GBuffer)
	{
		vec4 Color = vec4(0, 0, 0, 1);
		vec3 L = normalize(WorldSpaceLightDir(WorldPosition));
    	vec3 V = -CameraVector;
    	vec3 N = normalize(GBuffer.WorldNormal);

    	vec3 DiffuseColor = GBuffer.BaseColor - GBuffer.BaseColor * GBuffer.Metallic;
	    vec3 SpecularColor = mix( vec3(0.08 * GBuffer.Specular), GBuffer.BaseColor, GBuffer.Metallic);
	    float NoL = saturate( dot(N, L) );
	    //float NoV = max( dot(N, V), 1e-5 );
	    float NoV = saturate( dot(N, V) );
	    // Point lobe in off-specular peak direction

	    Color.rgb = PI * LightColor.rgb * NoL * StandardShading(DiffuseColor, SpecularColor, vec3(GBuffer.Roughness), vec3(1), L, V, N);

	    return Color;
	}

	in V2F v2f;

	out vec4 FragColor;

	void main()
	{
		vec3 CameraVector = normalize(v2f.ScreenVector);	// View

		float4 DBufferA = texture(DBufferATexture, v2f.UV);
		float4 DBufferB = texture(DBufferBTexture, v2f.UV);
		float4 DBufferC = texture(DBufferCTexture, v2f.UV);

		FGBufferData GBuffer = DecodeGBuffer(DBufferA, DBufferB, DBufferC);
		float SceneDepth = CalcSceneDepth(v2f.UV);
		float z = dot(CameraVector, WorldSpaceCameraDir.xyz);
		vec3 WorldPosition = CameraVector * (SceneDepth / z) + WorldSpaceCameraPos.xyz;

		FragColor = GetDynamicLighting(WorldPosition, CameraVector, GBuffer);
		// #ifdef _SHADOW
		// 	//float ShadowFactor = CalcShadowTerm(vec4(WorldPosition, 1), SceneDepth);
		// 	float ShadowFactor = GetScreenShadow(v2f.UV);
		//     FragColor.rgb *= ShadowFactor;
		// #endif
		//FragColor = vec4(1, 1, 0, 1);
	}
}