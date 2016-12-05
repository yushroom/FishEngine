#include "Texture.hpp"
#include "GLEnvironment.hpp"
#include "Debug.hpp"

namespace FishEngine
{
    std::vector<TexturePtr> Texture::m_textures;
    
    Texture::~Texture()
    {
        glDeleteTextures(1, &m_texture);
    }

    FishEngine::TexturePtr Texture::Create()
    {
        auto t = std::make_shared<Texture>();
        m_textures.push_back(t);
        return t;
    }


    std::shared_ptr<ColorBuffer> ColorBuffer::Create(const int width, const int height, TextureFormat  format /*= TextureFormat::RGBA32*/)
    {
        auto t = std::make_shared<ColorBuffer>();
        Texture::m_textures.push_back(t);
        t->m_dimension = TextureDimension::Tex2D;
        t->m_format = format;
        t->m_width = width;
        t->m_height = height;
        glGenTextures(1, &t->m_texture);
        glBindTexture(GL_TEXTURE_2D, t->m_texture);
        GLenum internal_format, external_format, pixel_type;
        TextureFormat2GLFormat(format, internal_format, external_format, pixel_type);
        glTexImage2D(GL_TEXTURE_2D, 0, internal_format, t->m_width, t->m_height, 0, external_format, pixel_type, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glBindTexture(GL_TEXTURE_2D, 0);
        glCheckError();
        return t;
    }


    void ColorBuffer::Resize(const int newWidth, const int newHeight)
    {
        Debug::Log("ColorBuffer::Resize");
        m_width = newWidth;
        m_height = newHeight;
        GLenum internal_format, external_format, pixel_type;
        TextureFormat2GLFormat(m_format, internal_format, external_format, pixel_type);
        glBindTexture(GL_TEXTURE_2D, m_texture);
        glTexImage2D(GL_TEXTURE_2D, 0, internal_format, m_width, m_height, 0, external_format, pixel_type, NULL);
        glCheckError();
    }

    std::shared_ptr<DepthBuffer> DepthBuffer::Create(const int width, const int height)
    {
        auto t = std::make_shared<DepthBuffer>();
        Texture::m_textures.push_back(t);
        t->m_dimension = TextureDimension::Tex2D;
        t->m_width = width;
        t->m_height = height;
        glGenTextures(1, &t->m_texture);
        glBindTexture(GL_TEXTURE_2D, t->m_texture);
        //glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, rt->m_width, rt->m_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, t->m_width, t->m_height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glBindTexture(GL_TEXTURE_2D, 0);
        glCheckError();
        return t;
    }

    void DepthBuffer::Resize(const int newWidth, const int newHeight)
    {
        Debug::Log("DepthBuffer::Resize");
        m_width = newWidth;
        m_height = newHeight;
        glBindTexture(GL_TEXTURE_2D, m_texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_width, m_height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
        glCheckError();
    }

    std::shared_ptr<LayeredColorBuffer> LayeredColorBuffer::Create(const int width, const int height, const int layers, TextureFormat format /*= TextureFormat::RGBA32*/)
    {
        auto t = std::make_shared<LayeredColorBuffer>();
        Texture::m_textures.push_back(t);
        t->m_dimension = TextureDimension::Tex2DArray;
        t->m_width = width;
        t->m_height = height;
        t->m_format = format;
        t->m_layers = layers;

        glGenTextures(1, &t->m_texture);
        glBindTexture(GL_TEXTURE_2D_ARRAY, t->m_texture);
        GLenum internal_format, external_format, pixel_type;
        TextureFormat2GLFormat(format, internal_format, external_format, pixel_type);
        glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, internal_format, t->m_width, t->m_height, layers, 0, external_format, pixel_type, NULL);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
        glCheckError();
        return t;
    }

    std::shared_ptr<LayeredDepthBuffer> LayeredDepthBuffer::Create(const int width, const int height, const int layers)
    {
        auto t = std::make_shared<LayeredDepthBuffer>();
        Texture::m_textures.push_back(t);
        t->m_dimension = TextureDimension::Tex2DArray;
        t->m_width = width;
        t->m_height = height;
        //t->m_format = format;
        t->m_layers = layers;

        glGenTextures(1, &t->m_texture);
        glBindTexture(GL_TEXTURE_2D_ARRAY, t->m_texture);
        //GLenum internal_format, external_format, pixel_type;
        //TextureFormat2GLFormat(format, internal_format, external_format, pixel_type);
        //glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, internal_format, t->m_width, t->m_height, layers, 0, external_format, pixel_type, NULL);
        glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH24_STENCIL8, t->m_width, t->m_height, layers, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
        glCheckError();
        return t;
    }

}
