#include "DDSImporter.hpp"

#if 1
#include <gli/gli.hpp>
#include <gli/convert.hpp>
#include <glm/detail/type_half.hpp>
#else
#include <PVRAssets/FileIO/TextureReaderDDS.h>
#include <PVRCore/FileStream.h>
#endif

#include <GLEnvironment.hpp>
#include <Debug.hpp>
#include <Common.hpp>
#include <Mathf.hpp>
#include <Texture2D.hpp>
#include <Cubemap.hpp>

#include "AssetDataBase.hpp"

#include <QIcon>
#include <QImage>

using namespace FishEngine;
using namespace FishEditor;


FishEngine::TexturePtr FishEditor::DDSImporter::Load()
{
	TexturePtr ret;
	auto path = m_assetPath.string();
	
#if 0
	pvr::FileStream::ptr_type stream;
	stream.reset(new pvr::FileStream(path, "rb"));
	pvr::assets::assetReaders::TextureReaderDDS reader(stream);
	
#else
	gli::texture gli_texture = gli::load(path);
	if (gli_texture.empty())
	{
		LogError("Texture not found: " + path);
		abort();
	}
	
	gli::gl GL(gli::gl::PROFILE_GL33);
	gli::gl::format const gli_format = GL.translate(gli_texture.format(), gli_texture.swizzles());
	//GLenum target = GL.translate(gli_texture.target());
	
	glm::tvec3<GLsizei> const extent = gli_texture.extent();
	//GLsizei const FaceTotal = static_cast<GLsizei>(Texture.layers() * Texture.faces());
	//auto type = gli_format.Type;
	
	int width = extent.x;
	int height = extent.y;
	
	TextureDimension dimension;
	auto t = gli_texture.target();
	if (t == gli::TARGET_2D)
	{
		abort();
		dimension = TextureDimension::Tex2D;
	}
//	else if (t == gli::TARGET_3D)
//	{
//		dimension = TextureDimension::Tex3D;
//	}
	else if (t == gli::TARGET_CUBE)
	{
		dimension = TextureDimension::Cube;
	}
	else
	{
		abort();
	}
	
	TextureFormat format;
	
	int bpp = 0; // bytes per pixel
	switch (gli_format.Internal)
	{
		case gli::gl::INTERNAL_ALPHA8:
			format = TextureFormat::Alpha8;
			bpp = 1;
			break;
		case gli::gl::INTERNAL_R8U:
			format = TextureFormat::R8;
			bpp = 1;
			break;
		case gli::gl::INTERNAL_RGB8U:
			format = TextureFormat::RGB24;
			bpp = 3;
			break;
		case gli::gl::INTERNAL_RGBA8U:
			format = TextureFormat::RGBA32;
			bpp = 4;
			break;
		case gli::gl::INTERNAL_RGBA16F:
			format = TextureFormat::RGBAHalf;
			bpp = 8;
			break;
		case gli::gl::INTERNAL_RGBA32F:
			format = TextureFormat::RGBAFloat;
			bpp = 16;
			break;
		default:
			abort();
	}
	
	int mipmapCount = static_cast<int>(gli_texture.levels());
	
	if (dimension == TextureDimension::Tex2D)
	{
		auto tex2d = std::make_shared<Texture2D>();
		tex2d->m_format = format;
		tex2d->m_width = width;
		tex2d->m_height = height;
		
		int length = width * height * bpp;
		tex2d->m_data.resize(length);
		
		uint8_t* srcData = (uint8_t*)gli_texture.data(0, 0, 0);
		std::copy(srcData, srcData + length, tex2d->m_data.begin());
		
		ret = tex2d;
	}
	else if (dimension == TextureDimension::Cube)
	{
		assert(width == height);
		assert(gli_texture.faces() == 6);
		assert(gli_texture.layers() == 1);
		
		auto texCube = std::make_shared<Cubemap>(width, format, true);
		texCube->m_mipmapCount = mipmapCount;
		for (auto & face: texCube->m_pixels)
		{
			face.resize(mipmapCount);
		}
		
		for (int face = 0; face < 6; ++face)
		{
			for (int level = 0; level < mipmapCount; ++level)
			{
				glm::tvec3<GLsizei> levelSize = gli_texture.extent(level);
				int length = levelSize.x * levelSize.y * bpp;
				uint8_t* srcData = (uint8_t *)gli_texture.data(0, face, level);
				
				auto & pixels = texCube->m_pixels[face][level];
				pixels.resize(length);
				std::copy(srcData, srcData + length, pixels.begin());
			}
		}
		
		ret = texCube;
		
		QImage::Format qformat;
		if (format == TextureFormat::RGBAHalf)
		{
			//gli::texture_cube t(gli_texture);
			//auto converted = gli::convert(t, gli::FORMAT_RGBA8_UINT_PACK8);	// no effect
			qformat = QImage::Format_RGBA8888;
			auto qimage = QImage(width, height, qformat);
			auto length = width*height*4;
			//uint8_t * data = (uint8_t *)converted.data(0, 0, 0);
			//std::copy(data, data + length, qimage.bits());
			glm::detail::hdata * src = (glm::detail::hdata *)gli_texture.data(0, 0, 0);
			auto dest = qimage.bits();
			for (int i = 0; i < length; ++i)
			{
				dest[i] = glm::detail::toFloat32(src[i]) * 255;
			}
			//std::copy(data, data + length, qimage.bits());
			auto qpixmap = QPixmap::fromImage(std::move(qimage));
			AssetDatabase::s_cacheIcons[m_assetPath] = QIcon(qpixmap);
		}
		else
		{
			abort();
		}
	}
	
#endif
	
	return ret;
}

#if 0

// https://github.com/g-truc/gli/blob/master/manual.md
// http://gli.g-truc.net/0.8.1/api/a00006.html
// bug fixed
GLuint CreateTextureFromDDS(char const* path, FishEngine::TextureDimension* out_textureFormat)
{
	glCheckError();

	gli::texture gli_texture = gli::load(path);
	if (gli_texture.empty())
	{
		LogError(std::string("Texture not found: ") + path);
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

#endif
