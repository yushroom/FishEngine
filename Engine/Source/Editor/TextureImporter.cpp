#include "TextureImporter.hpp"

#include <FreeImage.h>

#include <GLEnvironment.hpp>
#include <Debug.hpp>
#include <Common.hpp>
#include <Mathf.hpp>
#include <Texture2D.hpp>

#include "AssetDataBase.hpp"

#include <QImage>

using namespace FishEngine;

// RAII for FreeImage
class FreeImagePlugin
{
public:
	static FreeImagePlugin * instance()
	{
		static FreeImagePlugin s_instance;
		return & s_instance;
	}
	
	~FreeImagePlugin()
	{
		FreeImage_DeInitialise();
	}
	
private:
	
	FreeImagePlugin()
	{
		FreeImage_Initialise(false);
		std::cout << "Supported texture formats: ";
		for (int i = 0; i < FreeImage_GetFIFCount(); ++i)
		{
			std::string ext = FreeImage_GetFIFExtensionList(FREE_IMAGE_FORMAT(i));
			std::cout << ext << ", ";
		}
		std::cout << std::endl;
		
		// Set error handler
		FreeImage_SetOutputMessage(FreeImageLoadErrorHandler);
	}

	
	static void FreeImageLoadErrorHandler(FREE_IMAGE_FORMAT fif, const char *message)
	{
		// Callback method as required by FreeImage to report problems
		const char* typeName = FreeImage_GetFormatFromFIF(fif);
		if (typeName)
		{
			Debug::LogError("FreeImage error: '%s' when loading format", message, typeName);
		}
		else
		{
			Debug::LogError("FreeImage error: '%s'", message);
		}
	}
};

/// INPLACESWAP adopted from codeguru.com
template <class T> void INPLACESWAP(T& a, T& b) {
	a ^= b; b ^= a; a ^= b;
}

// ==========================================================
// Utility functions declared in Utilities.h

BOOL SwapRedBlue32(FIBITMAP* dib) {
	if(FreeImage_GetImageType(dib) != FIT_BITMAP) {
		return FALSE;
	}
	
	const unsigned bytesperpixel = FreeImage_GetBPP(dib) / 8;
	if(bytesperpixel > 4 || bytesperpixel < 3) {
		return FALSE;
	}
	
	const unsigned height = FreeImage_GetHeight(dib);
	const unsigned pitch = FreeImage_GetPitch(dib);
	const unsigned lineSize = FreeImage_GetLine(dib);
	
	BYTE* line = FreeImage_GetBits(dib);
	for(unsigned y = 0; y < height; ++y, line += pitch) {
		for(BYTE* pixel = line; pixel < line + lineSize ; pixel += bytesperpixel) {
			INPLACESWAP(pixel[0], pixel[2]);
		}
	}
	
	return TRUE;
}


namespace FishEditor
{
	TextureImporter& TextureImporter::operator=(TextureImporter const & rhs)
	{
		m_allowAlphaSplitting = rhs.m_allowAlphaSplitting;
		m_alphaIsTransparency = rhs.m_alphaIsTransparency;
		m_anisoLevel = rhs.m_anisoLevel;
		m_borderMipmap = rhs.m_borderMipmap;
		m_compressionQuality = rhs.m_compressionQuality;
		m_convertToNormalmap = rhs.m_convertToNormalmap;
		m_crunchedCompression = rhs.m_crunchedCompression;
		m_fadeout = rhs.m_fadeout;
		m_heightmapScale = rhs.m_heightmapScale;
		m_generateCubemap = rhs.m_generateCubemap;
		m_textureType = rhs.m_textureType;
		m_textureShape = rhs.m_textureShape;
		m_textureSettings = rhs.m_textureSettings;
		m_sRGBTexture = rhs.m_sRGBTexture;
		m_isReadable = rhs.m_isReadable;
		m_mipmapEnabled = rhs.m_mipmapEnabled;
		return *this;
	}
	
#if 0
	GLuint CreateTexture(const Path& path)
	{
		int width, height, components;
		uint8_t *data = stbi_load(path.string().c_str(), &width, &height, &components, 0);
		if (data == nullptr)
		{
			Debug::LogError("Texture not found, path: %s", path.c_str());
			abort();
		}
		//TextureImporter::FromRawData(data, width, height, TextureFormat::RGBA32);

		//assert(data!=nullptr);
		GLuint t;
		glGenTextures(1, &t);
		glBindTexture(GL_TEXTURE_2D, t);
		
		GLenum internal_format = GL_RGBA8;
		GLenum format = GL_RGBA;
		if (components == 4)
		{
		}
		else if (components == 3)
		{
			internal_format = GL_RGB8;
			format = GL_RGB;
		}
		else if (components == 1)
		{
			internal_format = GL_R8;
			format = GL_RED;
		}
		else if (components == 2)
		{
			internal_format = GL_RG8;
			format = GL_RG;
		}
		else
		{
			abort();
		}
#if 1
		GLsizei max_mipmap_level_count = Mathf::FloorToInt(std::log2f(Mathf::Max(width, height))) + 1;
		glTexStorage2D(GL_TEXTURE_2D, max_mipmap_level_count, internal_format, width, height);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, data);
#else
		glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
#endif
		glGenerateMipmap(GL_TEXTURE_2D);
		// Parameters
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
		free(data);
		return t;
	}
#endif
	
	void TextureImporter::ImportTo(FishEngine::Texture2DPtr & texture)
	{
		auto fm_instance = FreeImagePlugin::instance();
		//std::shared_ptr<Texture> texture;
		auto ext = m_assetPath.extension();
		if (ext == ".dds")
		{
			//abort();
			return;
		}
		else if (ext == ".bmp" || ext == ".png" || ext == ".jpg" || ext == ".tga" || ext == ".hdr")
		{
			FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
			FIBITMAP *dib = nullptr;
			uint8_t * bits = nullptr;
			unsigned int width = 0, height = 0;
#if FISHENGINE_PLATFORM_WINDOWS
			auto filename = m_assetPath.wstring().c_str();
			fif = FreeImage_GetFileTypeU(filename);
			if (fif == FIF_UNKNOWN)
			{
				fif = FreeImage_GetFIFFromFilenameU(filename);
			}
			if (fif == FIF_UNKNOWN)
			{
				abort();
			}
			if (FreeImage_FIFSupportsReading(fif))
			{
				dib = FreeImage_LoadU(fif, filename);
			}
			else
			{
				abort();
			}
#else
			auto filename = m_assetPath.c_str();
			fif = FreeImage_GetFileType(filename);
			if (fif == FIF_UNKNOWN)
			{
				fif = FreeImage_GetFIFFromFilename(filename);
			}
			if (fif == FIF_UNKNOWN)
			{
				abort();
			}
			if (FreeImage_FIFSupportsReading(fif))
			{
				dib = FreeImage_Load(fif, filename);
			}
			else
			{
				abort();
			}
#endif
			
			//retrieve the image data
			bits = FreeImage_GetBits(dib);
			//get the image width and height
			width = FreeImage_GetWidth(dib);
			height = FreeImage_GetHeight(dib);
			//if this somehow one of these failed (they shouldn't), return failure
			if ((bits == 0) || (width == 0) || (height == 0))
			{
				abort();
			}

			bool needResize = false;
			if ( ! Mathf::IsPowerOfTwo(width) )
			{
				width = Mathf::NextPowerOfTwo(width);
				needResize = true;
			}
			if ( ! Mathf::IsPowerOfTwo(height) )
			{
				height = Mathf::NextPowerOfTwo(height);
				needResize = true;
			}
			
			if (needResize)
			{
				Debug::LogWarning("resize image");
				auto newdib = FreeImage_Rescale(dib, width, height);
				FreeImage_Unload(dib);
				newdib = dib;
			}
			
			auto imageType = FreeImage_GetImageType(dib);
			auto colorType = FreeImage_GetColorType(dib);
			auto bpp = FreeImage_GetBPP(dib);
			
			TextureFormat format;
			
			if (imageType == FIT_BITMAP)
			{
				if (colorType == FIC_MINISWHITE || colorType == FIC_MINISBLACK)
				{
					auto newBitmap = FreeImage_ConvertToGreyscale(dib);
					FreeImage_Unload(dib);
					dib = newBitmap;
					bpp = FreeImage_GetBPP(dib);
					colorType = FreeImage_GetColorType(dib);
				}
				else if (bpp < 8 || colorType == FIC_PALETTE || colorType == FIC_CMYK)
				{
					auto newBitmap = FreeImage_ConvertTo24Bits(dib);
					FreeImage_Unload(dib);
					dib = newBitmap;
					bpp = FreeImage_GetBPP(dib);
					colorType = FreeImage_GetColorType(dib);
				}
				
				// by this stage, 8-bit is greyscale, 16/24/32 bit are RGB[A]
				switch (bpp)
				{
					case 8:
						format = TextureFormat::R8;
						break;
					case 16:
						if (FreeImage_GetGreenMask(dib) == FI16_565_GREEN_MASK)
						{
							//"Format not supported by the engine. TODO."
							abort();
						}
						else
						{
							//"Format not supported by the engine. TODO."
							abort();
						}
						break;
					case 24:
#if FREEIMAGE_COLORORDER == FREEIMAGE_COLORORDER_BGR
						// unity does not support BGR24
						SwapRedBlue32(dib);
#endif
						format = TextureFormat::RGB24;
						break;
					case 32:
#if FREEIMAGE_COLORORDER == FREEIMAGE_COLORORDER_RGB
						format = TextureFormat::RGBA32;
#else
						format = TextureFormat::BGRA32;
#endif
						break;
					default:
						break;
				}
			}
			else if (imageType == FIT_UINT16 || imageType == FIT_INT16)
			{
				// "No INT pixel formats supported currently. TODO."
				abort();
			}
			else if (imageType == FIT_FLOAT)
			{
				format = TextureFormat::RFloat;
			}
			else if (imageType == FIT_RGB16)
			{
				// Format not supported by the engine. TODO.
				abort();
			}
			else
			{
				abort();
			}
			
			unsigned char* srcData = FreeImage_GetBits(dib);
			unsigned srcPitch = FreeImage_GetPitch(dib);
			if (srcPitch != width * (bpp / 8))
			{
				abort();
			}
			
			int length = width * height * (bpp / 8);
//			if (expectedBytes != length)
//			{
//				abort();
//			}
			auto data = srcData;
			texture->m_width = width;
			texture->m_height = height;
			texture->m_format = format;
			texture->m_data.resize(length);
			std::copy(data, data + length, texture->m_data.begin());
			
			// get icon
			//FreeImage_Rescale();
			auto thumbnail = FreeImage_MakeThumbnail(dib, 64);
			FreeImage_FlipVertical(thumbnail);	// flip for Qt
			QImage::Format qformat;
			if (format == TextureFormat::RGBA32)
			{
				qformat = QImage::Format_RGBA8888;
			}
			else if (format == TextureFormat::BGRA32)
			{
				// TODO
				SwapRedBlue32(thumbnail);
				//data = FreeImage_GetBits(dib);
				qformat = QImage::Format_RGBA8888;
			}
			else if (format == TextureFormat::RGB24)
			{
				qformat = QImage::Format_RGB888;
				
			}
			else if (format == TextureFormat::R8)
			{
				qformat = QImage::Format_Grayscale8;
			}
			else
			{
				abort();
			}
			data = FreeImage_GetBits(thumbnail);
			width = FreeImage_GetWidth(thumbnail);
			height = FreeImage_GetHeight(thumbnail);
			length = width * height * (bpp / 8);
			auto qimage = QImage(width, height, qformat);
			std::copy(data, data + length, qimage.bits());
			auto qpixmap = QPixmap::fromImage(std::move(qimage));
			AssetDatabase::s_cacheIcons[m_assetPath] = QIcon(qpixmap);
			FreeImage_Unload(thumbnail);

			FreeImage_Unload(dib);
			return;
		}
		else
		{
			abort();
		}
	}

	FishEngine::TexturePtr TextureImporter::Import(Path const & path)
	{
		m_assetPath = path;
		auto texture = std::make_shared<Texture2D>();
		this->ImportTo(texture);
		m_assetObject = texture;
		return texture;
	}
	
	void TextureImporter::Reimport()
	{
		auto texture = AssetImporter::s_importerGUIDToObject[this->m_guid];
		auto texture2d = std::dynamic_pointer_cast<Texture2D>(texture);
		ImportTo(texture2d);
	}
}
