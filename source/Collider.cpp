#include "Collider.hpp"
#include "GameObject.hpp"
#include "Rigidbody.hpp"
//#include "<#header#>"
#define NDEBUG
#include "PxPhysicsAPI.h"

using namespace physx;

extern physx::PxDefaultAllocator		gAllocator;
extern physx::PxDefaultErrorCallback	gErrorCallback;

extern physx::PxFoundation*			gFoundation;
extern physx::PxPhysics*				gPhysics;

extern physx::PxDefaultCpuDispatcher*	gDispatcher;
extern physx::PxScene*				gScene;

extern physx::PxMaterial*				gMaterial;

using namespace FishEngine;

void FishEngine::Collider::
Start()
{
    auto rigidbody = gameObject()->GetComponent<Rigidbody>();
    if (rigidbody != nullptr) {
        //return;
        
    }
    PxRigidStatic* groundPlane = PxCreatePlane(*gPhysics, PxPlane(0,1,0,0), *gMaterial);
    gScene->addActor(*groundPlane);
}
