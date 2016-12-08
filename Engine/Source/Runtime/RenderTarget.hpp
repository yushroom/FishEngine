#pragma once

#include "FishEngine.hpp"

namespace FishEngine
{

    class FE_EXPORT RenderTarget
    {
    public:
        RenderTarget() = default;

        void SetColorBufferOnly(ColorBufferPtr colorBuffer);
        void SetDepthBufferOnly(DepthBufferPtr depthBuffer);
        void Set(ColorBufferPtr colorBuffer, DepthBufferPtr depthBuffer);
        void Set(ColorBufferPtr colorBuffer1, ColorBufferPtr colorBuffer2, ColorBufferPtr colorBuffer3, DepthBufferPtr depthBuffer);

        void Attach();
        void Detach();

    private:
        bool            m_useDepthBuffer = true;
        uint32_t        m_activeColorBufferCount = 1;
        ColorBufferPtr  m_colorBuffers[3];
        DepthBufferPtr  m_depthBuffer;
        unsigned int    m_fbo = 0;

        void Init();
    };
}
