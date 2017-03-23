#pragma once

#include "GLEnvironment.hpp"

namespace FishEngine
{
	/************************************************************************/
	/* Texture                                                              */
	/************************************************************************/

	// Filtering mode for textures.
	enum class FilterMode
	{
		Point = 0,    // Point filtering - texture pixels become blocky up close.
		Bilinear = 1,    // Bilinear filtering - texture samples are averaged.
		Trilinear = 2,    // Trilinear filtering - texture samples are averaged and also blended between mipmap levels.
	};

	// Wrap mode for textures.
	enum class TextureWrapMode
	{
		Repeat = 0,    // Tiles the texture, creating a repeating pattern.
		Clamp = 1,    // Clamps the texture to the last pixel at the border.
	};

	enum class TextureDimension
	{
		Unknown,    // Texture type is not initialized or unknown.
		None,       // No texture is assigned.
		Tex2D,      // 2D texture (Texture2D).
		Tex3D,      // 3D volume texture (Texture3D).
		Cube,       // Cubemap texture.
		Tex2DArray, // 2D array texture (Texture2DArray).
		Any,        // Any texture type.
	};

	enum class TextureFormat
	{
		Alpha8 = 1,
		ARGB4444,
		RGB24,
		RGBA32,
		ARGB32,
		RGB565 = 7,
		R16 = 9,
		DXT1,
		DXT5 = 12,
		RGBA4444,
		BGRA32,
		RHalf,
		RGHalf,
		RGBAHalf,
		RFloat,
		RGFloat,
		RGBAFloat,
		YUY2,
		BC4 = 26,
		BC5,
		BC6H = 24,
		BC7,
		DXT1Crunched = 28,
		DXT5Crunched,
		PVRTC_RGB2,
		PVRTC_RGBA2,
		PVRTC_RGB4,
		PVRTC_RGBA4,
		ETC_RGB4,
		ATC_RGB4,
		ATC_RGBA8,
		EAC_R = 41,
		EAC_R_SIGNED,
		EAC_RG,
		EAC_RG_SIGNED,
		ETC2_RGB,
		ETC2_RGBA1,
		ETC2_RGBA8,
		ASTC_RGB_4x4,
		ASTC_RGB_5x5,
		ASTC_RGB_6x6,
		ASTC_RGB_8x8,
		ASTC_RGB_10x10,
		ASTC_RGB_12x12,
		ASTC_RGBA_4x4,
		ASTC_RGBA_5x5,
		ASTC_RGBA_6x6,
		ASTC_RGBA_8x8,
		ASTC_RGBA_10x10,
		ASTC_RGBA_12x12,
		ETC_RGB4_3DS,
		ETC_RGBA8_3DS,
//		//Alpha8,
//		//ARGB4444,
//		RGB24,	// Color texture format, 8-bits per channel.
//		RGBA32, // Color with alpha texture format, 8-bits per channel.
//		ARGB32, // Color with an alpha channel texture format.
//		R16,    // A 16 bit color texture format that only has a red channel.
//		DXT1,   // Compressed color texture format.
//		DXT5,   // Compressed color with alpha channel texture format.
//		//RGBA4444,
//		BGRA32,
		R8,
//		R32,    // **New**, float * 1
		RG16,    // **New**, uint8_t * 2
		RG32,   // **New**, uint16_t * 2
//		RGHalf,	// Two color (RG) texture format, 16 bit floating point per channel.
//		RGFloat, // Two color (RG) texture format, 32 bit floating point per channel.
//		RFloat,
//		RGBAFloat,
	};

	void TextureFormat2GLFormat(
		TextureFormat format,
		GLenum& out_internalFormat,
		GLenum& out_externalFormat,
		GLenum& out_pixelType);

	enum class CubemapFace
	{
		Unknown = 6,
		PositiveX = 0,
		NegativeX = 1,
		PositiveY = 2,
		NegativeY = 3,
		PositiveZ = 4,
		NegativeZ = 5,
	};
}
