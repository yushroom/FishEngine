#ifndef RenderSystem_hpp
#define RenderSystem_hpp

#include "GLEnvironment.hpp"

namespace FishEngine
{
    class FE_EXPORT RenderSystem
    {
    public:
        RenderSystem() = delete;
        
        static void Init();
        
        static void Render();
        
        static void Clean();
    };

}

#endif // RenderSystem_hpp
