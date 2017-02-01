#include "TextureProperty.hpp"
#include <cassert>

namespace FishEngine
{
    void TextureFormat2GLFormat(
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
}

