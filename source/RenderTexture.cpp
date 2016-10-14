#include "RenderTexture.hpp"

using namespace FishEngine;

PRenderTexture FishEngine::RenderTexture::CreateShadowMap()
{
    auto rt = std::make_shared<RenderTexture>();
    rt->m_width = 4096;
    rt->m_height = 4096;
    rt->m_format = RenderTextureFormat::Shadowmap;

    glGenFramebuffers(1, &rt->m_depthMapFBO);
    glGenTextures(1, &rt->m_texture);
    glBindTexture(GL_TEXTURE_2D, rt->m_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
        rt->m_width, rt->m_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindFramebuffer(GL_FRAMEBUFFER, rt->m_depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, rt->m_texture, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return rt;
}
