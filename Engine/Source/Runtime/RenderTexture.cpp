#include "RenderTexture.hpp"
#include "Debug.hpp"
#include <boost/lexical_cast.hpp>

namespace FishEngine
{

    RenderTexture::~RenderTexture()
    {
        glDeleteFramebuffers(1, &m_FBO);
    }

    RenderTexturePtr RenderTexture::CreateShadowMap()
    {
        auto rt = std::make_shared<RenderTexture>();
        rt->m_width = 4096;
        rt->m_height = 4096;
        rt->m_format = RenderTextureFormat::Shadowmap;

        glGenFramebuffers(1, &rt->m_FBO);
        glGenTextures(1, &rt->m_texture);
        glBindTexture(GL_TEXTURE_2D, rt->m_texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
            rt->m_width, rt->m_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glBindFramebuffer(GL_FRAMEBUFFER, rt->m_FBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, rt->m_texture, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        return rt;
    }

    RenderTexturePtr RenderTexture::CreateColorMap(const int width, const int height)
    {
        auto rt = std::make_shared<RenderTexture>();
        rt->m_width = width;
        rt->m_height = height;
        rt->m_format = RenderTextureFormat::ARGB32;

        
        //glGenRenderbuffers(1, &rt->m_texture);
        //glBindRenderbuffer(GL_RENDERBUFFER, rt->m_texture);
        //glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA, rt->m_width, rt->m_height);

        glGenFramebuffers(1, &rt->m_FBO);
        glGenTextures(1, &rt->m_texture);
        glBindTexture(GL_TEXTURE_2D, rt->m_texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, rt->m_width, rt->m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glGenTextures(1, &rt->m_depthBuffer);
        glBindTexture(GL_TEXTURE_2D, rt->m_depthBuffer);
        //glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, rt->m_width, rt->m_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, rt->m_width, rt->m_height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glBindFramebuffer(GL_FRAMEBUFFER, rt->m_FBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, rt->m_texture, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, rt->m_depthBuffer, 0);
        //glDrawBuffer(GL_NONE);
        //glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        return rt;
    }

    void RenderTexture::Resize(const int newWidth, const int newHeight)
    {
        Debug::Log("RenderTexture::Resize");
        m_width = newWidth;
        m_height = newHeight;
        glBindTexture(GL_TEXTURE_2D, m_texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glBindTexture(GL_TEXTURE_2D, m_depthBuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_width, m_height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
    }

    GBuffer::GBuffer()
    {
        for (int i = 0; i < 3; ++i)
        {
            m_colorBuffers[i] = Texture::Create();
            m_colorBuffers[i]->setDimension(TextureDimension::Tex2D);
            m_colorBuffers[i]->setName("GBuffer-RT" + boost::lexical_cast<std::string>(i));
        }
        m_depthBuffer = Texture::Create();
        m_depthBuffer->setDimension(TextureDimension::Tex2D);
        m_depthBuffer->setName("GBuffer-Depth");
    }

    GBuffer::~GBuffer()
    {
        //glDeleteTextures(3, m_rt);
        //glDeleteTextures(1, &m_depthBuffer);
        glDeleteFramebuffers(1, &m_FBO);
    }

    void GBuffer::Init(const int width, const int height)
    {
        m_width = width;
        m_height = height;
        glGenFramebuffers(1, &m_FBO);
        glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

        for (int i = 0; i < 3; ++i)
        {
            auto& rt = m_colorBuffers[i]->m_texture;
            glGenTextures(1, &rt);
            glBindTexture(GL_TEXTURE_2D, rt);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+i, GL_TEXTURE_2D, rt, 0);
        }

        GLuint attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
        glDrawBuffers(3, attachments);

        glGenTextures(1, &m_depthBuffer->m_texture);
        glBindTexture(GL_TEXTURE_2D, m_depthBuffer->m_texture);
        //glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, rt->m_width, rt->m_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depthBuffer->m_texture, 0);
        
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glCheckError();
    }

    void GBuffer::Resize(const int newWidth, const int newHeight)
    {
        Debug::Log("GBuffer::Resize");
        m_width = newWidth;
        m_height = newHeight;
        for (int i = 0; i < 3; ++i)
        {
            glBindTexture(GL_TEXTURE_2D, m_colorBuffers[i]->m_texture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        }

        glBindTexture(GL_TEXTURE_2D, m_depthBuffer->m_texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_width, m_height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
        glCheckError();
    }

} // namespace FishEngine
