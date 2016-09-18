#include "Texture.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <gli/gli.hpp>
#include "Debug.hpp"

NAMESPACE_FISHENGINE_BEGIN

Texture::~Texture() {
    glDeleteTextures(1, &m_texture);
}


// http://gli.g-truc.net/0.8.1/api/a00006.html
// bug fixed
GLuint create_texture(char const* Filename)
{
    glCheckError();
    
    gli::texture Texture = gli::load(Filename);
    if (Texture.empty()) {
        Debug::LogError("Texture %s not found", Filename);
        abort();
    }
    //assert(!Texture.empty());
    
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
    
    glm::tvec3<GLsizei> const Extent(Texture.extent());
    //GLsizei const FaceTotal = static_cast<GLsizei>(Texture.layers() * Texture.faces());
    
    switch(Texture.target())
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
    
    for(std::size_t Layer = 0; Layer < Texture.layers(); ++Layer)
        for(std::size_t Face = 0; Face < Texture.faces(); ++Face)
            for(std::size_t Level = 0; Level < Texture.levels(); ++Level)
            {
                GLsizei const LayerGL = static_cast<GLsizei>(Layer);
                glm::tvec3<GLsizei> Extent(Texture.extent(Level));
                Target = gli::is_target_cube(Texture.target())
                ? static_cast<GLenum>(GL_TEXTURE_CUBE_MAP_POSITIVE_X + Face)
                : Target;
                
                switch(Texture.target())
                {
                    case gli::TARGET_1D:
                        if(gli::is_compressed(Texture.format()))
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
                        if(gli::is_compressed(Texture.format()))
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
                        if(gli::is_compressed(Texture.format()))
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
    glCheckError();
    return TextureName;
}

//Texture& Texture::GetSimpleTexutreCubeMap()
//{
//    GLuint texture_id;
//    // Six 1x1 RGB faces
//    GLubyte cubePixels[6][3] =
//    {
//        // Face 0 - Red
//        255, 0, 0,
//        // Face 1 - Green,
//        0, 255, 0,
//        // Face 2 - Blue
//        0, 0, 255,
//        // Face 3 - Yellow
//        255, 255, 0,
//        // Face 4 - Purple
//        255, 0, 255,
//        // Face 5 - White
//        255, 255, 255
//    };
//    
//    // Generate a texture object
//    glGenTextures(1, &texture_id);
//    
//    // Bind the texture object
//    glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);
//    // Load the cube face - Positive X
//    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, 1, 1, 0,
//                 GL_RGB, GL_UNSIGNED_BYTE, &cubePixels[0]);
//    // Load the cube face - Negative X
//    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, 1, 1, 0,
//                 GL_RGB, GL_UNSIGNED_BYTE, &cubePixels[1]);
//    // Load the cube face - Positive Y
//    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, 1, 1, 0,
//                 GL_RGB, GL_UNSIGNED_BYTE, &cubePixels[2]);
//    // Load the cube face - Negative Y
//    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, 1, 1, 0,
//                 GL_RGB, GL_UNSIGNED_BYTE, &cubePixels[3]);
//    // Load the cube face - Positive Z
//    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, 1, 1, 0,
//                 GL_RGB, GL_UNSIGNED_BYTE, &cubePixels[4]);
//    // Load the cube face - Negative Z
//    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, 1, 1, 0,
//                 GL_RGB, GL_UNSIGNED_BYTE, &cubePixels[5]);
//    // Set the filtering mode
//    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
//    
//    glCheckError();
//    static Texture t(texture_id);
//    return t;
//}

GLuint CreateTexture(const std::string& path) {
    int width, height, n;
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &n, 3);
    assert(data!=nullptr);
    GLuint t;
    glGenTextures(1, &t);
    glBindTexture(GL_TEXTURE_2D, t);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
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

void Texture::FromFile(const std::string& path)
{
    auto pos = path.find_last_of('.');
    assert(pos != std::string::npos);
    auto ext = path.substr(pos + 1);
    if (ext == "dds") {
        m_texture = create_texture(path.c_str());
    }
    else if (ext == "bmp" || ext == "png" || ext == "jpg") {
        m_texture = CreateTexture(path);
    }
    else {
        Debug::LogError("texture type[%s] not supported\n", ext.c_str());
        abort();
    }
}

Texture::Texture(const std::string& path) {
    FromFile(path);
}

NAMESPACE_FISHENGINE_END
