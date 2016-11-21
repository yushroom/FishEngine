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
    GLuint CreateTextureFromDDS(char const* Filename)
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
    
    GLuint CreateTexture(const std::string& path)
    {
        int width, height, n;
        unsigned char *data = stbi_load(path.c_str(), &width, &height, &n, 4);
        if (data == nullptr)
        {
            Debug::LogError("Texture not found, path: %s", path.c_str());
            abort();
        }
        //assert(data!=nullptr);
        GLuint t;
        glGenTextures(1, &t);
        glBindTexture(GL_TEXTURE_2D, t);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        
        // Parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);
        free(data);
        return t;
    }

    
    TexturePtr TextureImporter::FromFile(const std::string& path)
    {
        GLuint texture;
        auto ext = getExtensionWithoutDot(path);
        if (ext == "dds")
        {
            texture = CreateTextureFromDDS(path.c_str());
        }
        else if (ext == "bmp" || ext == "png" || ext == "jpg" || ext == "tga")
        {
            texture = CreateTexture(path);
        }
        else
        {
            Debug::LogError("texture type[%s] not supported\n", ext.c_str());
            abort();
        }
        auto t = std::make_shared<Texture>();
        t->m_texture = texture;
        auto name = getFileNameWithoutExtension(path);
        t->setName(name);
        Texture::m_textures.push_back(t);
        return t;
    }
}
