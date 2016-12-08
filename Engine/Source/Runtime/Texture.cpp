#include "Texture.hpp"
#include "GLEnvironment.hpp"
#include "Debug.hpp"
#include <cassert>

namespace FishEngine
{

    void Texture::BindSampler()
    {
        assert(m_texture != 0);
        const auto& sampler = TextureSampler::GetSampler(m_filterMode, m_wrapMode);
        glBindSampler(m_texture, sampler.m_nativeGLSampler);
    }

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
        if (m_useStencil)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_width, m_height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
        else
            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL);
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

    std::shared_ptr<LayeredDepthBuffer> LayeredDepthBuffer::Create(const int width, const int height, const int layers, bool useStencil /*= true*/)
    {
        auto t = std::make_shared<LayeredDepthBuffer>();
        Texture::m_textures.push_back(t);
        t->m_useStencil = useStencil;
        t->m_dimension = TextureDimension::Tex2DArray;
        t->m_width = width;
        t->m_height = height;
        //t->m_format = format;
        t->m_layers = layers;

        glGenTextures(1, &t->m_texture);
        glBindTexture(GL_TEXTURE_2D_ARRAY, t->m_texture);
        if (useStencil)
            glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH24_STENCIL8, t->m_width, t->m_height, layers, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
        else
            glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH_COMPONENT, t->m_width, t->m_height, layers, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL);
        glCheckError();
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
        glCheckError();
        return t;
    }

    TextureSampler::~TextureSampler()
    {
        glDeleteSamplers(1, &m_nativeGLSampler);
    }


    FishEngine::TextureSampler TextureSampler::s_samplers[6];


    void TextureSampler::Init()
    {
        // point, repeat
        int idx = 0;
        glGenSamplers(1, &s_samplers[0].m_nativeGLSampler);
        glSamplerParameteri(s_samplers[0].m_nativeGLSampler, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glSamplerParameteri(s_samplers[0].m_nativeGLSampler, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glSamplerParameteri(s_samplers[0].m_nativeGLSampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glSamplerParameteri(s_samplers[0].m_nativeGLSampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        // point, clamp
        idx++;
        glGenSamplers(1, &s_samplers[1].m_nativeGLSampler);
        glSamplerParameteri(s_samplers[1].m_nativeGLSampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glSamplerParameteri(s_samplers[1].m_nativeGLSampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glSamplerParameteri(s_samplers[1].m_nativeGLSampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glSamplerParameteri(s_samplers[1].m_nativeGLSampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        // bilinear, repeat
        idx++;
        glGenSamplers(1, &s_samplers[idx].m_nativeGLSampler);
        glSamplerParameteri(s_samplers[idx].m_nativeGLSampler, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glSamplerParameteri(s_samplers[idx].m_nativeGLSampler, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glSamplerParameteri(s_samplers[idx].m_nativeGLSampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glSamplerParameteri(s_samplers[idx].m_nativeGLSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

        // bilinear, clamp
        idx++;
        glGenSamplers(1, &s_samplers[idx].m_nativeGLSampler);
        glSamplerParameteri(s_samplers[idx].m_nativeGLSampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glSamplerParameteri(s_samplers[idx].m_nativeGLSampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glSamplerParameteri(s_samplers[idx].m_nativeGLSampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glSamplerParameteri(s_samplers[idx].m_nativeGLSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

        // trilinear, repeat
        idx++;
        glGenSamplers(1, &s_samplers[idx].m_nativeGLSampler);
        glSamplerParameteri(s_samplers[idx].m_nativeGLSampler, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glSamplerParameteri(s_samplers[idx].m_nativeGLSampler, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glSamplerParameteri(s_samplers[idx].m_nativeGLSampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glSamplerParameteri(s_samplers[idx].m_nativeGLSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

        // trilinear, clamp
        idx++;
        glGenSamplers(1, &s_samplers[idx].m_nativeGLSampler);
        glSamplerParameteri(s_samplers[idx].m_nativeGLSampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glSamplerParameteri(s_samplers[idx].m_nativeGLSampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glSamplerParameteri(s_samplers[idx].m_nativeGLSampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glSamplerParameteri(s_samplers[idx].m_nativeGLSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    }

    const TextureSampler& TextureSampler::GetSampler(FilterMode filterMode, TextureWrapMode wrapMode)
    {
        auto idx = static_cast<int>(filterMode) * 3 + static_cast<int>(wrapMode);
        return s_samplers[idx];
    }
}
