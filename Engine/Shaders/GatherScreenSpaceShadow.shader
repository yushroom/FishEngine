@shadow off
@cull off
@zwrite off

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
	#include <CascadedShadowMapCommon.inc>
	//#include <ShadowCommon.inc>

	in V2F v2f;
	out float OutShadow;


	void main()
	{
		vec3 CameraVector = normalize(v2f.ScreenVector);
		float SceneDepth = CalcSceneDepth(v2f.UV);
		float z = dot(CameraVector, WorldSpaceCameraDir.xyz);
		vec3 WorldPosition = CameraVector * (SceneDepth / z) + WorldSpaceCameraPos.xyz;
		OutShadow = CalcShadowTerm(vec4(WorldPosition, 1), SceneDepth);
	}
}