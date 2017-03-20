#pragma once

#include "Object.hpp"

namespace FishEngine
{	
	class FE_EXPORT Prefab : public Object
	{
		InjectClassName(Prefab);
		
	public:
		Prefab() = default;
		
		Prefab(Prefab const &) = delete;
		Prefab & operator=(Prefab const &) = delete;
		
		GameObjectPtr rootGameObject() const
		{
			return m_rootGameObject;
		}
		
		void setRootGameObject(GameObjectPtr const &  root)
		{
			m_rootGameObject = root;
		}

		// TODO: make it protected
		bool isPrefabParent() const
		{
			return m_isPrefabParent;
		}

		PrefabPtr parentPrefab() const
		{
			return m_parentPrefab;
		}
		
		// TODO: make it protected
		void setIsPrefabParent(bool isPrefabParent)
		{
			m_isPrefabParent = isPrefabParent;
		}
	
	private:
		friend class Object;
		PrefabPtr		m_parentPrefab;
		GameObjectPtr	m_rootGameObject;
		bool			m_isPrefabParent = false;
	};
}
