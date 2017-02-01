#ifndef PhysicsSystem_hpp
#define PhysicsSystem_hpp

#include "FishEngine.hpp"
//#ifndef _DEBUG
//#define NDEBUG
//#endif
#ifndef _DEBUG
#ifndef NDEBUG
#define NDEBUG
#endif
#endif
#include <PxPhysicsAPI.h>

#include "ReflectClass.hpp"

namespace FishEngine
{
    class FE_EXPORT Meta(NonSerializable) PhysicsSystem
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
