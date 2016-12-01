@zwrite off

#include <DeferredShadingCommon.inc>
#include <ShadingModels.inc>
#include <ShadowCommon.inc>

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
	#if 1
		vec4 WorldPosition = inverse(MATRIX_VP) * vec4(InputPosition.x, InputPosition.y, 1.0f, 1.0f);
		// in world space
		v2f.ScreenVector = normalize(WorldPosition.xyz / WorldPosition.w - WorldSpaceCameraPos.xyz);	// in view space
	#else
		vec4 ViewSpacePosition = inverse(MATRIX_P) * vec4(InputPosition.x, InputPosition.y, 1.0f, 1.0f);
		// in view space
		v2f.ScreenVector = normalize(ViewSpacePosition.xyz / ViewSpacePosition.w);
	#endif
	}
}

@fragment
{
	uniform sampler2D DBufferATexture;
	uniform sampler2D DBufferBTexture;
	uniform sampler2D DBufferCTexture;
	uniform sampler2D SceneDepthTexture;

	// also see ConvertToDeviceZ()
	// @param DeviceZ value that is stored in the depth buffer (Z/W)
	// @return SceneDepth (linear in world units, W)
	// float ConvertFromDeviceZ(float DeviceZ)
	// {
	// 	// Supports ortho and perspective, see CreateInvDeviceZToWorldZTransform()
	// 	return DeviceZ * View.InvDeviceZToWorldZTransform[0] + View.InvDeviceZToWorldZTransform[1] + 1.0f / (DeviceZ * View.InvDeviceZToWorldZTransform[2] - View.InvDeviceZToWorldZTransform[3]);
	// }


	// Z buffer to linear 0..1 depth (0 at eye, 1 at far plane)
	float Linear01Depth( float z )
	{
		return 1.0 / (ZBufferParams.x * z + ZBufferParams.y);
	}
	// Z buffer to linear depth
	float LinearEyeDepth( float z )
	{
		//(n * f) / (f - z * (f - n))
		return 1.0 / (ZBufferParams.z * z + ZBufferParams.w);
	}

	// Returns clip space W, which is world space distance along the View Z axis.
	float CalcSceneDepth(float2 ScreenUV)
	{
		// (2 * n * f) / (f + n - z * (f - n))
		//return LinearEyeDepth( textureLod( SceneDepthTexture, ScreenUV, 0 ).r );
		float DeviceZ = textureLod( SceneDepthTexture, ScreenUV, 0 ).r;
		return LinearEyeDepth(DeviceZ);
		// float ClipSpaceZ = DeviceZ * 2 - 1;
		// float near = ProjectionParams.y;
		// float far = ProjectionParams.z;
		// return 2*near*far / (far+near - (far-near)*ClipSpaceZ);
	}


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

	#ifdef _SHADOW
	    vec4 positionInLightSpace = LightMatrix * vec4(WorldPosition, 1);
	    float ShadowFactor = ShadowCalculation(positionInLightSpace, L, N);
	    Color.rgb *= (1.f - ShadowFactor*0.5);
	#endif
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

	#if 1
		//vec4 WorldSpaceCameraDir = MATRIX_I_V * vec4(0, 0, 1, 0);
		float z = dot(CameraVector, WorldSpaceCameraDir.xyz);
		vec3 WorldPosition = CameraVector * (SceneDepth / z) + WorldSpaceCameraPos.xyz;
	#else
		vec3 ViewSpacePosition = CameraVector * (SceneDepth / CameraVector.z);
		vec3 WorldPosition = (MATRIX_I_V * vec4(ViewSpacePosition, 1)).xyz;
		CameraVector = (MATRIX_I_V * vec4(CameraVector, 0)).xyz;	// view -> world
	#endif

		FragColor = GetDynamicLighting(WorldPosition, CameraVector, GBuffer);
		//FragColor = vec4(1, 1, 0, 1);
	}
}