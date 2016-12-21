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
        RGB32,
        RGBA32, // Color with alpha texture format, 8-bits per channel.
        ARGB32, // Color with an alpha channel texture format.
        R16,    // A 16 bit color texture format that only has a red channel.
        DXT1,   // Compressed color texture format.
        DXT5,   // Compressed color with alpha channel texture format.
        R8,
        R32,    // **New**, float * 1
        RG8,    // **New**, uint8_t * 2
        RG16,   // **New**, uint16_t * 2
        RGHalf,	// Two color (RG) texture format, 16 bit floating point per channel.
        RGFloat	// Two color (RG) texture format, 32 bit floating point per channel.
    };

    static void TextureFormat2GLFormat(
        TextureFormat format,
        GLenum& out_internalFormat,
        GLenum& out_externalFormat,
        GLenum& out_pixelType)
    {
        switch (format)
        {
        case TextureFormat::RGBA32:
            out_internalFormat = GL_RGBA8;
            out_externalFormat = GL_RGBA;
            out_pixelType = GL_UNSIGNED_BYTE;
            break;
        case TextureFormat::RG16:
            out_internalFormat = GL_RG16;
            out_externalFormat = GL_RG;
            out_pixelType = GL_UNSIGNED_SHORT;
            break;
        case TextureFormat::RG8:
            out_internalFormat = GL_RG8;
            out_externalFormat = GL_RG;
            out_pixelType = GL_UNSIGNED_BYTE;
            break;
        case TextureFormat::RGFloat:
            out_internalFormat = GL_RG32F;
            out_externalFormat = GL_RG;
            out_pixelType = GL_FLOAT;
            break;
        case TextureFormat::R32:
            out_internalFormat = GL_R32F;
            out_externalFormat = GL_RED;
            out_pixelType = GL_FLOAT;
            break;
        case TextureFormat::R8:
            out_internalFormat = GL_R8;
            out_externalFormat = GL_RED;
            out_pixelType = GL_UNSIGNED_BYTE;
            break;
        default:
            //Debug::LogError("Unknown texture format");
            abort();
        }
    }

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

    /************************************************************************/
    /* TextureImporter                                                      */
    /************************************************************************/

    // Select this to set basic parameters depending on the purpose of your texture.
    enum class TextureImporterType
    {
        Default,        // This is the most common setting used for all the textures in general.
        NormalMap,      // Select this to turn the color channels into a format suitable for real - time normal mapping.
        GUI,            // Use this if your texture is going to be used on any HUD / GUI Controls.
        Sprite,         // Select this if you will be using your texture for Sprite graphics.
        Cursor,         // Use this if your texture is going to be used as a cursor.
        Cookie,         // This sets up your texture with the basic parameters used for the Cookies of your lights.
        Lightmap,       // This sets up your texture with the parameters used by the lightmap.
        SingleChannel,  //Use this for texture containing a single channel.
    };

    enum class TextureImporterShape
    {
        Texture2D,
        TextureCube,
    };

    enum class TextureImporterCompression
    {
        Uncompressed,
        Compressed,
        CompressedHQ,
        CompressedLQ,
    };
}
