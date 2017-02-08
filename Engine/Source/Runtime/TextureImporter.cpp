#include "TextureImporter.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <gli/gli.hpp>
#include "GLEnvironment.hpp"
#include "Debug.hpp"
#include "Common.hpp"
#include "Mathf.hpp"
#include "Texture2D.hpp"


namespace FishEngine
{
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

	FishEngine::TexturePtr TextureImporter::Import(Path const & path)
	{
		//std::shared_ptr<Texture> texture;
		auto ext = path.extension();
		if (ext == ".dds")
		{
			abort();
		}
		else if (ext == ".bmp" || ext == ".png" || ext == ".jpg" || ext == ".tga" || ext == ".hdr")
		{
			int width, height, components;
			uint8_t *data = stbi_load(path.string().c_str(), &width, &height, &components, 0);
			if (data == nullptr)
			{
				Debug::LogError("Texture not found, path: %s", path.c_str());
				abort();
			}
			//TextureImporter::FromRawData(data, width, height, TextureFormat::RGBA32);

			auto texture = std::make_shared<Texture2D>();
			int length = width * height * components;
			texture->m_data.resize(length);
			std::copy(data, data + length, texture->m_data.begin());
			free(data);
			texture->m_width = width;
			texture->m_height = height;

			switch (components)
			{
			case 1:
				texture->m_format = TextureFormat::R8;
				break;
			case 2:
				texture->m_format = TextureFormat::RG16;
				break;
			case 3:
				texture->m_format = TextureFormat::RGB24;
				break;
			case 4:
				texture->m_format = TextureFormat::RGBA32;
				break;
			default:
				abort();
			}
			return texture;
		}
		else
		{
			abort();
		}

		return nullptr;
	}

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
}
