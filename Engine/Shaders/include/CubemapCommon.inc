#ifndef CubemapCommon_inc
#define CubemapCommon_inc

uniform samplerCube AmbientCubemap;

const uint GDiffuseConvolveMipLevel = 4;

// MipCount = FMath::Log2(CubemapWidth) + 1.0f;
const uint MipCount = 8;	// for 128*128 envmap
// in PostProcessAmbient.cpp
// AmbientCubemapMipAdjustValue.X =  1.0f - GDiffuseConvolveMipLevel / MipCount;
// AmbientCubemapMipAdjustValue.Y = (MipCount - 1.0f) * AmbientCubemapMipAdjustValue.X;
// AmbientCubemapMipAdjustValue.Z = MipCount - GDiffuseConvolveMipLevel;
// AmbientCubemapMipAdjustValue.W = MipCount;
const half4 AmbientCubemapMipAdjust = half4(0.5, 3.5, 4.0, 8.0);

// @param MipCount e.g. 10 for x 512x512
half ComputeCubemapMipFromRoughness( half Roughness, half MipCount )
{
	// Level starting from 1x1 mip
	half Level = 3 - 1.15 * log2( Roughness );
	return MipCount - 1 - Level;
}

#endif

