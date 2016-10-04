#ifndef PhysicsSystem_hpp
#define PhysicsSystem_hpp

#include "FishEngine.hpp"

namespace FishEngine
{
    class PhysicsSystem
    {
    public:
        PhysicsSystem() = delete;
        
        static void Init();
        static void Start();
        static void FixedUpdate();
        static void Clean();
    private:
        
    };
}

#endif // PhysicsSystem_hpp
