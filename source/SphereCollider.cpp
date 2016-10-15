#include "SphereCollider.hpp"
#include "Transform.hpp"
#include "Gizmos.hpp"
#include "PhysicsSystem.hpp"

using namespace physx;
extern physx::PxPhysics*    gPhysics;
extern physx::PxScene*      gScene;
extern physx::PxMaterial*   gMaterial;

FishEngine::SphereCollider::
SphereCollider(const Vector3& center,
               const float radius)
: m_center(center), m_radius(radius)
{
}


void FishEngine::SphereCollider::
CreatePhysicsShape()
{
    //if (m_physxShape == nullptr) {
        m_physxShape = gPhysics->createShape(PxSphereGeometry(m_radius), *gMaterial);
        m_physxShape->setLocalPose(PxTransform(m_center.x, m_center.y, m_center.z));
    //}
}


void FishEngine::SphereCollider::
OnDrawGizmosSelected()
{
    Gizmos::setColor(Color::green);
    Gizmos::DrawWireSphere(m_center, m_radius, transform()->localToWorldMatrix());
}
