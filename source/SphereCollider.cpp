#include "SphereCollider.hpp"
#include "Transform.hpp"
#include "Gizmos.hpp"

FishEngine::SphereCollider::
SphereCollider(const Vector3& center,
               const float radius)
: m_center(center), m_radius(radius)
{
    //m_physxShape = gPhysics->createShape(PxBoxGeometry(size.x*0.5f, size.y*0.5f, size.z*0.5f), *gMaterial);
}



void FishEngine::SphereCollider::
OnDrawGizmos()
{
    //Gizmos::DrawWireCube(transform()->position()+m_center, m_size);
    Gizmos::DrawWireSphere(transform()->position(), m_radius);
}
