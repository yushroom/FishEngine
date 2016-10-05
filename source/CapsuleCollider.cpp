#include "CapsuleCollider.hpp"
#include "Gizmos.hpp"
#include "Debug.hpp"

using namespace FishEngine;

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

void FishEngine::CapsuleCollider::
OnDrawGizmos()
{
    Gizmos::DrawWireCapsule(m_center, m_radius, m_height);
}
