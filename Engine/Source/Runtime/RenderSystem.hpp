#ifndef RenderSystem_hpp
#define RenderSystem_hpp

#include "RenderTexture.hpp"

namespace FishEngine
{
    class FE_EXPORT RenderSystem
    {
    public:
        RenderSystem() = delete;
        
        static void Init();
        
        static void Render();
        
        static void Clean();

        static void ResizeBufferSize(const int width, const int height);

        static ColorBufferPtr   m_GBuffer[3];
        static DepthBufferPtr   m_depthBuffer;
        static RenderTargetPtr  m_deferredRenderTarget;
    };
}

#endif // RenderSystem_hpp
