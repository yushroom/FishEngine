#include "Rigidbody.hpp"
#include "GameObject.hpp"
#include "PhysicsSystem.hpp"
#include "Gizmos.hpp"
#include "Transform.hpp"
#include "PhysicsSystem.hpp"

using namespace FishEngine;
using namespace physx;

extern physx::PxPhysics*				gPhysics;
extern physx::PxScene*				gScene;
extern physx::PxMaterial*				gMaterial;

static physx::PxVec3 PxVec3(const FishEngine::Vector3& v)
{
    return physx::PxVec3(v.x, v.y, v.z);
}

void FishEngine::Rigidbody::
Start(PxShape* shape)
{
    const auto& t = transform();
    const Vector3 p = t->position();
    const auto& q = t->rotation();
    PxTransform localTm(p.x, p.y, p.z, PxQuat(q.x, q.y, q.z, q.w));
    m_physxRigidDynamic = gPhysics->createRigidDynamic(localTm);
    m_physxRigidDynamic->attachShape(*shape);
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
    transform()->setPosition(t.p.x, t.p.y, t.p.z);
    transform()->setLocalRotation(Quaternion(t.q.x, t.q.y, t.q.z, t.q.w));
}
