#include "Behaviour.hpp"
#include "GameObject.hpp"

NAMESPACE_FISHENGINE_BEGIN

bool Behaviour::isActiveAndEnabled() const
{
    return  m_enabled && m_gameObject->activeInHierarchy();
}

NAMESPACE_FISHENGINE_END
