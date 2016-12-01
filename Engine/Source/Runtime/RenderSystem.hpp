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

    //private:
        static GBuffer m_GBuffer;
    };
}

#endif // RenderSystem_hpp
