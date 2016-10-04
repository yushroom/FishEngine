#include "BoxCollider.hpp"
#include "Transform.hpp"
#include "Gizmos.hpp"

FishEngine::BoxCollider::
BoxCollider(const Vector3& center,
            const Vector3& size)
    : m_center(center), m_size(size)
{
    //m_physxShape = gPhysics->createShape(PxBoxGeometry(size.x*0.5f, size.y*0.5f, size.z*0.5f), *gMaterial);
}



void FishEngine::BoxCollider::
OnDrawGizmos()
{
    Gizmos::DrawWireCube(transform()->position()+m_center, m_size);
}
