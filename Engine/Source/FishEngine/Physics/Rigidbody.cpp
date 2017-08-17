#include <FishEngine/Rigidbody.hpp>
#include <FishEngine/GameObject.hpp>
#include <FishEngine/PhysicsSystem.hpp>
#include <FishEngine/Gizmos.hpp>
#include <FishEngine/Transform.hpp>
#include <FishEngine/PhysicsSystem.hpp>
#include <FishEngine/Collider.hpp>

using namespace FishEngine;
using namespace physx;

extern physx::PxPhysics*				gPhysics;
extern physx::PxScene*				gScene;
extern physx::PxMaterial*				gMaterial;

inline physx::PxVec3 PxVec3(const FishEngine::Vector3& v)
{
	return physx::PxVec3(v.x, v.y, v.z);
}

void FishEngine::Rigidbody::Initialize(PxShape* shape)
{
	const auto& t = transform();
	const Vector3 p = t->position();
	const auto& q = t->rotation();
	m_physxRigidDynamic = gPhysics->createRigidDynamic(PxTransform(p.x, p.y, p.z, PxQuat(q.x, q.y, q.z, q.w)));
	if (shape)
	{
		m_physxRigidDynamic->attachShape(*shape);
		shape->release();
	}
}

void FishEngine::Rigidbody::Start()
{
	auto collider = gameObject()->GetComponent<Collider>();
	if (collider != nullptr)
		Initialize(collider->physicsShape());
	else
		Initialize(nullptr);
	
	const auto& t = transform();
	const Vector3 p = t->position();
	const auto& q = t->rotation();
	m_physxRigidDynamic->setGlobalPose(PxTransform(p.x, p.y, p.z, PxQuat(q.x, q.y, q.z, q.w)));
	PxRigidBodyExt::updateMassAndInertia(*m_physxRigidDynamic, 10.0f);
	m_physxRigidDynamic->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, !m_useGravity);
	gScene->addActor(*m_physxRigidDynamic);
}

void FishEngine::Rigidbody::Update()
{
	//m_physxRigidDynamic->user
	const auto& t = m_physxRigidDynamic->getGlobalPose();
	//const auto& pt = t.actor2World;
	transform()->setPosition(t.p.x, t.p.y, t.p.z);
	transform()->setLocalRotation(Quaternion(t.q.x, t.q.y, t.q.z, t.q.w));
}

void Rigidbody::OnDestroy()
{
	m_physxRigidDynamic = nullptr;
}

bool Rigidbody::IsInitialized() const
{
	return m_physxRigidDynamic != nullptr;
}
