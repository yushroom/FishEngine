#include "Rigidbody.hpp"
#include "PhysicsSystem.hpp"
#include "Gizmos.hpp"
#include "Transform.hpp"
#define NDEBUG
#include "PxPhysicsAPI.h"

using namespace FishEngine;
using namespace physx;

extern physx::PxDefaultAllocator		gAllocator;
extern physx::PxDefaultErrorCallback	gErrorCallback;

extern physx::PxFoundation*			gFoundation;
extern physx::PxPhysics*				gPhysics;

extern physx::PxDefaultCpuDispatcher*	gDispatcher;
extern physx::PxScene*				gScene;

extern physx::PxMaterial*				gMaterial;

static physx::PxVec3 PxVec3(const FishEngine::Vector3& v)
{
    return physx::PxVec3(v.x, v.y, v.z);
}

void FishEngine::Rigidbody::
Start()
{
    //auto m_physxShape = gPhysics->createShape(PxBoxGeometry(0.5f, 0.5f, 0.5f), *gMaterial);
    auto m_physxShape = gPhysics->createShape(PxSphereGeometry(0.5f), *gMaterial);
    const auto& t = transform();
    const Vector3 p = t->position();
    PxTransform localTm(p.x, p.y, p.z);
    m_physxRigidDynamic = gPhysics->createRigidDynamic(localTm);
    m_physxRigidDynamic->attachShape(*m_physxShape);
    //m_physxRigidDynamic->userData = (void*)(t.get());
    PxRigidBodyExt::updateMassAndInertia(*m_physxRigidDynamic, 10.0f);
    m_physxRigidDynamic->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, !m_useGravity);
    gScene->addActor(*m_physxRigidDynamic);
    //m_physxShape->release();
}

void FishEngine::Rigidbody::
Update()
{
    //m_physxRigidDynamic->user
    const auto& t = m_physxRigidDynamic->getGlobalPose();
    //const auto& pt = t.actor2World;
    transform()->setLocalPosition(t.p.x, t.p.y, t.p.z);
}
