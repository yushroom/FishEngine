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
		//Alpha8,
		//ARGB4444,
        RGB24,	// Color texture format, 8-bits per channel.
        RGBA32, // Color with alpha texture format, 8-bits per channel.
        ARGB32, // Color with an alpha channel texture format.
        R16,    // A 16 bit color texture format that only has a red channel.
        DXT1,   // Compressed color texture format.
        DXT5,   // Compressed color with alpha channel texture format.
        //RGBA4444,
		R8,
        R32,    // **New**, float * 1
        RG16,    // **New**, uint8_t * 2
        RG32,   // **New**, uint16_t * 2
        RGHalf,	// Two color (RG) texture format, 16 bit floating point per channel.
        RGFloat	// Two color (RG) texture format, 32 bit floating point per channel.
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
