#include "Prefab.hpp"
#include "GameObject.hpp"

namespace FishEngine
{
	void Prefab::CreateInstance() const
	{
		auto instance = std::make_shared<Prefab>();
		instance->m_isPrefabParent = false;
		instance->m_parentPrefab = m_rootGameObject->prefabInternal();
	}
}
