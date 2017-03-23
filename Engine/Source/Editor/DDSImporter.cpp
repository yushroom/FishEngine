#include "DDSImporter.hpp"

#include <gli/gli.hpp>

#include <GLEnvironment.hpp>
#include <Debug.hpp>
#include <Common.hpp>
#include <Mathf.hpp>
#include <Texture2D.hpp>

using namespace FishEngine;
using namespace FishEditor;

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


FishEngine::TexturePtr FishEditor::DDSImporter::Load()
{
	TexturePtr ret;
	auto path = m_assetPath.string();

	gli::texture gli_texture = gli::load(path);
	if (gli_texture.empty())
	{
		Debug::LogError("Texture %s not found", path);
		abort();
	}

	gli::gl GL(gli::gl::PROFILE_GL33);
	gli::gl::format const gli_format = GL.translate(gli_texture.format(), gli_texture.swizzles());
	GLenum target = GL.translate(gli_texture.target());

	glm::tvec3<GLsizei> const extent(gli_texture.extent());
	//GLsizei const FaceTotal = static_cast<GLsizei>(Texture.layers() * Texture.faces());
	auto t = gli_texture.target();

	TextureFormat format;

	switch (gli_format.Internal)
	{
	case gli::gl::INTERNAL_R8I:
		format = TextureFormat::R8;
		break;
	case gli::gl::INTERNAL_ALPHA8:
		format = TextureFormat::Alpha8;
		break;
	case gli::gl::INTERNAL_RGB8I:
		format = TextureFormat::RGB24;
	}

	return nullptr;
}

