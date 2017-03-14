#pragma once

#include "Object.hpp"

namespace FishEngine
{	
	class Prefab : public Object
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
		
		void CreateInstance() const;
		
		// TODO: make it protected
		bool prefabParent() const
		{
			return m_isPrefabParent;
		}
		
		// TODO: make it protected
		void setIsPrefabParent(bool isPrefabParent)
		{
			m_isPrefabParent = isPrefabParent;
		}
	
	private:
		PrefabPtr		m_parentPrefab;
		GameObjectPtr	m_rootGameObject;
		bool			m_isPrefabParent = false;
	};
}
