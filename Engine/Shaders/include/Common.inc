#ifndef Common_inc
#define Common_inc

#include <ShaderVariables.inc>

// (x, y) bottom left corner position (in clip space, (-1, 1))
// (z, w) width and height, in range [0, 1]
// eg. (-1, -1, 1, 1) for full sreen quad
uniform vec4 DrawRectParameters;

// Screen Aligned Quad
vec4 DrawRect(in vec2 InPosition)
{
	return vec4( (InPosition - DrawRectParameters.xy) * DrawRectParameters.zw + DrawRectParameters.xy ,0, 1 );
}

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
float CalcSceneDepth( vec2 ScreenUV )
{
	// (2 * n * f) / (f + n - z * (f - n))
	//return LinearEyeDepth( textureLod( SceneDepthTexture, ScreenUV, 0 ).r );
	float DeviceZ = textureLod( SceneDepthTexture, ScreenUV, 0 ).r;
	return LinearEyeDepth(DeviceZ);
}

#endif
