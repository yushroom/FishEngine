#ifndef RenderSystem_hpp
#define RenderSystem_hpp

#include "GLEnvironment.hpp"

namespace FishEngine
{
    class RenderSystem
    {
    public:
        RenderSystem() = delete;
        
        static void Init();
        
        static void Render();
        
        static void Clean();
    };

}

#endif // RenderSystem_hpp
