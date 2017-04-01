#include "TextureProperty.hpp"
#include <cassert>

namespace FishEngine
{

	int BytePerPixel(TextureFormat format)
	{
		switch (format)
		{
		case TextureFormat::Alpha8:
		case TextureFormat::R8:
			return 1;
		case TextureFormat::ARGB4444:
		case TextureFormat::RGB565:
		case TextureFormat::R16:
		case TextureFormat::RGBA4444:
		case TextureFormat::RHalf:
		case TextureFormat::RG16:
			return 2;
		case TextureFormat::RGB24:
			return 3;
		case TextureFormat::RGBA32:
		case TextureFormat::ARGB32:
		case TextureFormat::BGRA32:
		case TextureFormat::RGHalf:
		case TextureFormat::RFloat:
		case TextureFormat::RG32:
			return 4;
		case TextureFormat::RGBAHalf:
		case TextureFormat::RGFloat:
			return 8;
		case TextureFormat::RGBAFloat: // 32*4
			return 16;
		default:
			return -1;
		}
	}

	void TextureFormat2GLFormat(
		TextureFormat format,
		GLenum* out_internalFormat,
		GLenum* out_externalFormat,
		GLenum* out_pixelType)
	{
		switch (format)
		{
		case TextureFormat::RGB24:
			*out_internalFormat = GL_RGB8;
			*out_externalFormat = GL_RGB;
			*out_pixelType = GL_UNSIGNED_BYTE;
			break;
		case TextureFormat::RGBA32:
			*out_internalFormat = GL_RGBA8;
			*out_externalFormat = GL_RGBA;
			*out_pixelType = GL_UNSIGNED_BYTE;
			break;
		case TextureFormat::BGRA32:
			*out_internalFormat = GL_RGBA8;
			*out_externalFormat = GL_BGRA;
			*out_pixelType = GL_UNSIGNED_BYTE;
			break;
		case TextureFormat::RG32:
			*out_internalFormat = GL_RG16;
			*out_externalFormat = GL_RG;
			*out_pixelType = GL_UNSIGNED_SHORT;
			break;
		case TextureFormat::RG16:
			*out_internalFormat = GL_RG8;
			*out_externalFormat = GL_RG;
			*out_pixelType = GL_UNSIGNED_BYTE;
			break;
		case TextureFormat::RGFloat:
			*out_internalFormat = GL_RG32F;
			*out_externalFormat = GL_RG;
			*out_pixelType = GL_FLOAT;
			break;
		case TextureFormat::RFloat:
			*out_internalFormat = GL_R32F;
			*out_externalFormat = GL_RED;
			*out_pixelType = GL_FLOAT;
			break;
		case TextureFormat::R8:
			*out_internalFormat = GL_R8;
			*out_externalFormat = GL_RED;
			*out_pixelType = GL_UNSIGNED_BYTE;
			break;
		default:
			//Debug::LogError("Unknown texture format");
			abort();
		}
	}
}

