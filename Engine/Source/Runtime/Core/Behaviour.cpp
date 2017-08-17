#include <FishEngine/Behaviour.hpp>
#include <FishEngine/GameObject.hpp>

namespace FishEngine
{
	bool Behaviour::isActiveAndEnabled() const
	{
		return  m_enabled && gameObject()->activeInHierarchy();
	}
}
