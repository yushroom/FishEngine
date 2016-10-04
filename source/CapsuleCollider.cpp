#include "CapsuleCollider.hpp"
#include "Gizmos.hpp"

using namespace FishEngine;

void FishEngine::CapsuleCollider::
OnDrawGizmos()
{
    Gizmos::DrawWireCapsule(m_center, m_radius, m_height);
}
