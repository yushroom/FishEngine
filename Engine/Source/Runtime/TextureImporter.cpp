#include "TextureImporter.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <gli/gli.hpp>
#include "GLEnvironment.hpp"
#include "Debug.hpp"
#include "Common.hpp"


namespace FishEngine
{

    // http://gli.g-truc.net/0.8.1/api/a00006.html
    // bug fixed
    GLuint CreateTextureFromDDS(char const* Filename, FishEngine::TextureDimension* out_textureFormat)
    {
        glCheckError();
        
        gli::texture Texture = gli::load(Filename);
        if (Texture.empty())
        {
            Debug::LogError("Texture %s not found", Filename);
            abort();
        }
        
        gli::gl GL(gli::gl::PROFILE_GL33);
        gli::gl::format const Format = GL.translate(Texture.format(), Texture.swizzles());
        GLenum Target = GL.translate(Texture.target());
        
        GLuint TextureName = 0;
        glGenTextures(1, &TextureName);
        glBindTexture(Target, TextureName);
        glTexParameteri(Target, GL_TEXTURE_BASE_LEVEL, 0);
        glTexParameteri(Target, GL_TEXTURE_MAX_LEVEL, static_cast<GLint>(Texture.levels() - 1));
        glTexParameteri(Target, GL_TEXTURE_SWIZZLE_R, Format.Swizzles[0]);
        glTexParameteri(Target, GL_TEXTURE_SWIZZLE_G, Format.Swizzles[1]);
        glTexParameteri(Target, GL_TEXTURE_SWIZZLE_B, Format.Swizzles[2]);
        glTexParameteri(Target, GL_TEXTURE_SWIZZLE_A, Format.Swizzles[3]);
        
        glTexParameteri(Target, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(Target, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(Target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(Target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        glm::tvec3<GLsizei> const Extent(Texture.extent());
        //GLsizei const FaceTotal = static_cast<GLsizei>(Texture.layers() * Texture.faces());
        
        auto t = Texture.target();
        if (t == gli::TARGET_2D)
            *out_textureFormat = TextureDimension::Tex2D;
        else if (t == gli::TARGET_3D)
            *out_textureFormat = TextureDimension::Tex3D;
        else if (t == gli::TARGET_CUBE)
            *out_textureFormat = TextureDimension::Cube;

        switch (Texture.target())
        {
            case gli::TARGET_1D:
                glTexStorage1D(
                               Target, static_cast<GLint>(Texture.levels()), Format.Internal, Extent.x);
                break;
            case gli::TARGET_1D_ARRAY:
            case gli::TARGET_2D:
            case gli::TARGET_CUBE:
                glTexStorage2D(
                               Target, static_cast<GLint>(Texture.levels()), Format.Internal,
                               Extent.x, Extent.y);
                break;
            case gli::TARGET_2D_ARRAY:
            case gli::TARGET_3D:
            case gli::TARGET_CUBE_ARRAY:
                glTexStorage3D(
                               Target, static_cast<GLint>(Texture.levels()), Format.Internal,
                               Extent.x, Extent.y, Extent.z);
                break;
            default:
                assert(0);
                break;
        }
        
        for (std::size_t Layer = 0; Layer < Texture.layers(); ++Layer)
        {
            for (std::size_t Face = 0; Face < Texture.faces(); ++Face)
            {
                for (std::size_t Level = 0; Level < Texture.levels(); ++Level)
                {
                    GLsizei const LayerGL = static_cast<GLsizei>(Layer);
                    glm::tvec3<GLsizei> Extent(Texture.extent(Level));
                    Target = gli::is_target_cube(Texture.target())
                    ? static_cast<GLenum>(GL_TEXTURE_CUBE_MAP_POSITIVE_X + Face)
                    : Target;
                    
                    switch (Texture.target())
                    {
                        case gli::TARGET_1D:
                            if (gli::is_compressed(Texture.format()))
                                glCompressedTexSubImage1D(
                                                          Target, static_cast<GLint>(Level), 0, Extent.x,
                                                          Format.Internal, static_cast<GLsizei>(Texture.size(Level)),
                                                          Texture.data(Layer, Face, Level));
                            else
                                glTexSubImage1D(
                                                Target, static_cast<GLint>(Level), 0, Extent.x,
                                                Format.External, Format.Type,
                                                Texture.data(Layer, Face, Level));
                            break;
                        case gli::TARGET_1D_ARRAY:
                        case gli::TARGET_2D:
                        case gli::TARGET_CUBE:
                            if (gli::is_compressed(Texture.format()))
                                glCompressedTexSubImage2D(
                                                          Target, static_cast<GLint>(Level),
                                                          0, 0,
                                                          Extent.x,
                                                          Texture.target() == gli::TARGET_1D_ARRAY ? LayerGL : Extent.y,
                                                          Format.Internal, static_cast<GLsizei>(Texture.size(Level)),
                                                          Texture.data(Layer, Face, Level));
                            else {
                                glTexSubImage2D(
                                                Target, static_cast<GLint>(Level),
                                                0, 0,
                                                Extent.x,
                                                Texture.target() == gli::TARGET_1D_ARRAY ? LayerGL : Extent.y,
                                                Format.External, Format.Type,
                                                Texture.data(Layer, Face, Level));
                            }
                            break;
                        case gli::TARGET_2D_ARRAY:
                        case gli::TARGET_3D:
                        case gli::TARGET_CUBE_ARRAY:
                            if (gli::is_compressed(Texture.format()))
                                glCompressedTexSubImage3D(
                                                          Target, static_cast<GLint>(Level),
                                                          0, 0, 0,
                                                          Extent.x, Extent.y,
                                                          Texture.target() == gli::TARGET_3D ? Extent.z : LayerGL,
                                                          Format.Internal, static_cast<GLsizei>(Texture.size(Level)),
                                                          Texture.data(Layer, Face, Level));
                            else
                                glTexSubImage3D(
                                                Target, static_cast<GLint>(Level),
                                                0, 0, 0,
                                                Extent.x, Extent.y,
                                                Texture.target() == gli::TARGET_3D ? Extent.z : LayerGL,
                                                Format.External, Format.Type,
                                                Texture.data(Layer, Face, Level));
                            break;
                        default: assert(0); break;
                    }
                }
            }
        }
        glCheckError();
        return TextureName;
    }
    
    GLuint CreateTexture(const Path& path)
    {
        int width, height, n;
        uint8_t *data = stbi_load(path.string().c_str(), &width, &height, &n, 4);
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
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
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

    TexturePtr TextureImporter::FromFile(const Path& path)
    {
        auto t = std::make_shared<Texture>();
        GLuint texture;
        auto ext = path.extension();
        if (ext == ".dds")
        {
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

        t->m_texture = texture;
        auto name = path.stem().string();
        t->setName(name);
        Texture::m_textures.push_back(t);
        return t;
    }
    
    TexturePtr TextureImporter::FromRawData(const uint8_t* data, int width, int height, TextureFormat format)
    {
        GLenum internal_format, external_format, pixel_type;

        TextureFormat2GLFormat(format, internal_format, external_format, pixel_type);

        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glCheckError();
        glTexStorage2D(GL_TEXTURE_2D, 1, internal_format, width, height);
        glCheckError();
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
        t->m_texture = texture;
        t->m_width = width;
        t->m_height = height;
        t->m_dimension = TextureDimension::Tex2D;
        
        Texture::m_textures.push_back(t);
        return t;
    }
}
