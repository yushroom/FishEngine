#include "TextureImporter.hpp"

#include <FreeImage.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
//#define STB_IMAGE_RESIZE_IMPLEMENTATION
//#include <stb/stb_image_resize.h>

#include <gli/gli.hpp>

#include <GLEnvironment.hpp>
#include <Debug.hpp>
#include <Common.hpp>
#include <Mathf.hpp>
#include <Texture2D.hpp>
#include <Debug.hpp>

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
	
	// https://github.com/g-truc/gli/blob/master/manual.md
	// http://gli.g-truc.net/0.8.1/api/a00006.html
	// bug fixed
	GLuint CreateTextureFromDDS(char const* path, FishEngine::TextureDimension* out_textureFormat)
	{
		glCheckError();
		
		gli::texture gli_texture = gli::load(path);
		if (gli_texture.empty())
		{
			Debug::LogError("Texture %s not found", path);
			abort();
		}
		
		gli::gl GL(gli::gl::PROFILE_GL33);
		gli::gl::format const gli_format = GL.translate(gli_texture.format(), gli_texture.swizzles());
		GLenum target = GL.translate(gli_texture.target());
		
		GLuint gl_texture_name = 0;
		glGenTextures(1, &gl_texture_name);
		glBindTexture(target, gl_texture_name);
		glTexParameteri(target, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(target, GL_TEXTURE_MAX_LEVEL, static_cast<GLint>(gli_texture.levels() - 1));
		glTexParameteri(target, GL_TEXTURE_SWIZZLE_R, gli_format.Swizzles[0]);
		glTexParameteri(target, GL_TEXTURE_SWIZZLE_G, gli_format.Swizzles[1]);
		glTexParameteri(target, GL_TEXTURE_SWIZZLE_B, gli_format.Swizzles[2]);
		glTexParameteri(target, GL_TEXTURE_SWIZZLE_A, gli_format.Swizzles[3]);
		
		glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
		glm::tvec3<GLsizei> const extent(gli_texture.extent());
		//GLsizei const FaceTotal = static_cast<GLsizei>(Texture.layers() * Texture.faces());
		
		auto t = gli_texture.target();
		if (t == gli::TARGET_2D)
			*out_textureFormat = TextureDimension::Tex2D;
		else if (t == gli::TARGET_3D)
			*out_textureFormat = TextureDimension::Tex3D;
		else if (t == gli::TARGET_CUBE)
			*out_textureFormat = TextureDimension::Cube;

		switch (gli_texture.target())
		{
			case gli::TARGET_1D:
				glTexStorage1D(target, static_cast<GLint>(gli_texture.levels()), gli_format.Internal, extent.x);
				break;
			case gli::TARGET_1D_ARRAY:
			case gli::TARGET_2D:
			case gli::TARGET_CUBE:
				glTexStorage2D(target, static_cast<GLint>(gli_texture.levels()), gli_format.Internal, extent.x, extent.y);
				break;
			case gli::TARGET_2D_ARRAY:
			case gli::TARGET_3D:
			case gli::TARGET_CUBE_ARRAY:
				glTexStorage3D(target, static_cast<GLint>(gli_texture.levels()), gli_format.Internal, extent.x, extent.y, extent.z);
				break;
			default:
				assert(0);
				break;
		}
		
		for (std::size_t Layer = 0; Layer < gli_texture.layers(); ++Layer)
		{
			for (std::size_t Face = 0; Face < gli_texture.faces(); ++Face)
			{
				for (std::size_t Level = 0; Level < gli_texture.levels(); ++Level)
				{
					GLsizei const LayerGL = static_cast<GLsizei>(Layer);
					glm::tvec3<GLsizei> Extent(gli_texture.extent(Level));
					if (gli::is_target_cube(gli_texture.target()))
						target = static_cast<GLenum>(GL_TEXTURE_CUBE_MAP_POSITIVE_X + Face);
					
					switch (gli_texture.target())
					{
						case gli::TARGET_1D:
							if (gli::is_compressed(gli_texture.format()))
								glCompressedTexSubImage1D(
														  target, static_cast<GLint>(Level), 0, Extent.x,
														  gli_format.Internal, static_cast<GLsizei>(gli_texture.size(Level)),
														  gli_texture.data(Layer, Face, Level));
							else
								glTexSubImage1D(
												target, static_cast<GLint>(Level), 0, Extent.x,
												gli_format.External, gli_format.Type,
												gli_texture.data(Layer, Face, Level));
							break;
						case gli::TARGET_1D_ARRAY:
						case gli::TARGET_2D:
						case gli::TARGET_CUBE:
							if (gli::is_compressed(gli_texture.format()))
								glCompressedTexSubImage2D(
														  target, static_cast<GLint>(Level),
														  0, 0,
														  Extent.x,
														  gli_texture.target() == gli::TARGET_1D_ARRAY ? LayerGL : Extent.y,
														  gli_format.Internal, static_cast<GLsizei>(gli_texture.size(Level)),
														  gli_texture.data(Layer, Face, Level));
							else {
								glTexSubImage2D(
												target, static_cast<GLint>(Level),
												0, 0,
												Extent.x,
												gli_texture.target() == gli::TARGET_1D_ARRAY ? LayerGL : Extent.y,
												gli_format.External, gli_format.Type,
												gli_texture.data(Layer, Face, Level));
							}
							break;
						case gli::TARGET_2D_ARRAY:
						case gli::TARGET_3D:
						case gli::TARGET_CUBE_ARRAY:
							if (gli::is_compressed(gli_texture.format()))
								glCompressedTexSubImage3D(
														  target, static_cast<GLint>(Level),
														  0, 0, 0,
														  Extent.x, Extent.y,
														  gli_texture.target() == gli::TARGET_3D ? Extent.z : LayerGL,
														  gli_format.Internal, static_cast<GLsizei>(gli_texture.size(Level)),
														  gli_texture.data(Layer, Face, Level));
							else
								glTexSubImage3D(
												target, static_cast<GLint>(Level),
												0, 0, 0,
												Extent.x, Extent.y,
												gli_texture.target() == gli::TARGET_3D ? Extent.z : LayerGL,
												gli_format.External, gli_format.Type,
												gli_texture.data(Layer, Face, Level));
							break;
						default: assert(0); break;
					}
				}
			}
		}
		glCheckError();
		return gl_texture_name;
	}
	
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
	
	void TextureImporter::ImportTo(FishEngine::Texture2DPtr & texture)
	{
		auto fm_instance = FreeImagePlugin::instance();
		//std::shared_ptr<Texture> texture;
		auto ext = m_assetPath.extension();
		if (ext == ".dds")
		{
			abort();
		}
		else if (ext == ".bmp" || ext == ".png" || ext == ".jpg" || ext == ".tga" || ext == ".hdr")
		{
#if 1
			FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
			FIBITMAP *dib = nullptr;
			uint8_t * bits = nullptr;
			unsigned int width = 0, height = 0;
			const char* filename = m_assetPath.c_str();
			fif = FreeImage_GetFileType(filename);
			if (fif == FIF_UNKNOWN)
				fif = FreeImage_GetFIFFromFilename(filename);
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
			
			//retrieve the image data
			bits = FreeImage_GetBits(dib);
			//get the image width and height
			width = FreeImage_GetWidth(dib);
			height = FreeImage_GetHeight(dib);
			//if this somehow one of these failed (they shouldn't), return failure
			if((bits == 0) || (width == 0) || (height == 0))
				abort();
			
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
					bpp = FreeImage_GetColorType(dib);
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
			
			//AssetDatabase::s_cacheIcons[m_assetPath] = QIcon(QPixmap::fromImage(std::move(image)));
			AssetDatabase::s_cacheIcons[m_assetPath] = QIcon();
			
			FreeImage_Unload(dib);
			return;
			
#else
			QImage image(QString::fromStdString(m_assetPath.string()));
			//auto texture = std::make_shared<Texture2D>();
			int width = image.width();
			int height = image.height();
			texture->m_width = width;
			texture->m_height = height;
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
				image = image.scaled(width, height);
			}
			
			//int bytesPerLine = image.bytesPerLine();
			//int bytes = image.byteCount();
			int expectedBytes = 0;
			
			auto format = image.format();
			
			// QImage is 32-bit aligned
			switch (format)
			{
				case QImage::Format_RGB32:
					//image.convertToFormat(QImage::Format_RGB888);
					//texture->m_format = TextureFormat::RGB24;
					//expectedBytes = width * height * 3;
					//break;
				case QImage::Format_ARGB32:
					image = image.convertToFormat(QImage::Format_RGBA8888);
					texture->m_format = TextureFormat::RGBA32;
					expectedBytes = width * height * 4;
					break;
				case QImage::Format_Mono:
					image = image.convertToFormat(QImage::Format_Grayscale8);
					//break;
				case QImage::Format_Grayscale8:
					texture->m_format = TextureFormat::R8;
					expectedBytes = width * height;
					break;
				default:
					abort();
			}
			
			int length = image.byteCount();
			if (expectedBytes != length)
			{
				abort();
			}
			auto data = image.bits();
			texture->m_data.resize(length);
			std::copy(data, data + length, texture->m_data.begin());
			
			AssetDatabase::s_cacheIcons[m_assetPath] = QIcon(QPixmap::fromImage(std::move(image)));
			//AssetDatabase::m_cacheIcons.emplace({ path, QIcon(QPixmap::fromImage(std::move(image))) });
			return;
#endif
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
		return texture;
	}
	
	void TextureImporter::Reimport()
	{
		auto texture = AssetImporter::s_importerGUIDToObject[this->m_guid];
		auto texture2d = std::dynamic_pointer_cast<Texture2D>(texture);
		ImportTo(texture2d);
	}

#if 0

	TexturePtr TextureImporter::FromFile(const Path& path)
	{
		auto t = std::make_shared<Texture>();
		GLuint texture;
		auto ext = path.extension();
		if (ext == ".dds")
		{
			//abort();
			TextureDimension format;
			texture = CreateTextureFromDDS(path.string().c_str(), &format);
			t->m_dimension = format;
		}
		else if (ext == ".bmp" || ext == ".png" || ext == ".jpg" || ext == ".tga" || ext == ".hdr")
		{
			texture = CreateTexture(path);
			t->m_dimension = TextureDimension::Tex2D;
		}
		else
		{
			Debug::LogError("texture type[%s] not supported\n", ext.string().c_str());
			abort();
		}

		t->m_GLNativeTexture = texture;
		auto name = path.stem().string();
		t->setName(name);
		Texture::s_textures.push_back(t);
		return t;
	}
	
	TexturePtr TextureImporter::FromRawData(const uint8_t* data, int width, int height, TextureFormat format)
	{
		GLenum internal_format, external_format, pixel_type;

		TextureFormat2GLFormat(format, internal_format, external_format, pixel_type);

		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexStorage2D(GL_TEXTURE_2D, 1, internal_format, width, height);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height,
			external_format, pixel_type, data);
		glCheckError();

		GLenum wrap_mode = (m_wrapMode == TextureWrapMode::Clamp ? GL_CLAMP_TO_EDGE : GL_REPEAT);
		
		GLenum min_filter_mode;
		GLenum mag_filter_mode = GL_LINEAR;
		if (m_filterMode == FilterMode::Point)
		{
			mag_filter_mode = min_filter_mode = GL_NEAREST;
		}
		else if (m_filterMode == FilterMode::Bilinear)
		{
			min_filter_mode = GL_LINEAR;
		}
		else    // Trilinear
		{
			min_filter_mode = GL_LINEAR_MIPMAP_LINEAR;
		}
		glCheckError();
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_mode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_mode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter_mode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter_mode);
		glBindTexture(GL_TEXTURE_2D, 0);
		glCheckError();
		
		auto t = std::make_shared<Texture>();
		t->m_GLNativeTexture = texture;
		t->m_width = width;
		t->m_height = height;
		t->m_dimension = TextureDimension::Tex2D;
		
		Texture::s_textures.push_back(t);
		return t;
	}
#endif
}
