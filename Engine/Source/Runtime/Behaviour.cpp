#include "Behaviour.hpp"
#include "GameObject.hpp"

namespace FishEngine
{
    bool Behaviour::isActiveAndEnabled() const
    {
        return  m_enabled && gameObject()->activeInHierarchy();
    }
}
