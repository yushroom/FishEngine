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

    std::shared_ptr<ColorBuffer> ColorBuffer::Create(const int width, const int height)
    {
        auto t = std::make_shared<ColorBuffer>();
        Texture::m_textures.push_back(t);
        t->m_dimension = TextureDimension::Tex2D;
        t->m_width = width;
        t->m_height = height;
        glGenTextures(1, &t->m_texture);
        glBindTexture(GL_TEXTURE_2D, t->m_texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, t->m_width, t->m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
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
        glBindTexture(GL_TEXTURE_2D, m_texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
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

}
