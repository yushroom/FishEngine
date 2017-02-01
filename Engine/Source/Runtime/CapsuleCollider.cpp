#include "CapsuleCollider.hpp"
#include "Gizmos.hpp"
#include "Debug.hpp"
#include "PhysicsSystem.hpp"
#include "Transform.hpp"

using namespace FishEngine;

using namespace physx;
extern physx::PxPhysics*    gPhysics;
extern physx::PxScene*      gScene;
extern physx::PxMaterial*   gMaterial;

FishEngine::CapsuleCollider::
CapsuleCollider(const Vector3&  center,
                const float     height,
                const float     radius)
    : m_center(center), m_height(height), m_radius(radius)
{
    if (height < 2*radius) {
        Debug::LogWarning("Invalid parameter height");
        m_height = 2*radius;
    }
}

void FishEngine::CapsuleCollider::OnDrawGizmosSelected()
{
    Gizmos::setColor(Color::green);
    auto center = m_center + transform()->position();
    Gizmos::DrawWireCapsule(center, m_radius, m_height);
}

void FishEngine::CapsuleCollider::CreatePhysicsShape()
{
    //if (m_physxShape == nullptr) {
        //PxTransform relativePose(PxQuat(PxHalfPi, PxVec3(0, 0, 1))); // standing upright
        m_physxShape = gPhysics->createShape(PxCapsuleGeometry(m_radius, m_height*0.5f-m_radius), *gMaterial);
        m_physxShape->setLocalPose(PxTransform(m_center.x, m_center.y, m_center.z, PxQuat(PxHalfPi, PxVec3(0, 0, 1))));
    //}
}
