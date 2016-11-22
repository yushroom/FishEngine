#ifndef App_hpp
#define App_hpp

#include "FishEngine.hpp"

namespace FishEngine
{
    class FE_EXPORT App
    {
    public:
        virtual ~App() = 0;
        virtual void Init() = 0;
        //virtual void Update() = 0;
        //virtual void Clean() = 0;
    };
}

#endif // App_hpp
