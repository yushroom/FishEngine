#include "Object.hpp"
#include "Scene.hpp"

namespace FishEngine
{
	void Object::Destroy(GameObjectPtr obj, const float t /*= 0.0f*/)
	{
		
		Scene::Destroy(obj, t);
	}

	void Object::Destroy(ComponentPtr component, float t /*= 0.0f*/)
	{
		Scene::Destroy(component, t);
	}

	void Object::DestroyImmediate(ComponentPtr component)
	{
		Scene::DestroyImmediate(component);
	}

	void Object::DestroyImmediate(GameObjectPtr obj)
	{
		Scene::DestroyImmediate(obj);
	}

	void Object::CopyValueTo(ObjectPtr target) const
	{
		target->m_name = m_name;
		target->m_hideFlags = m_hideFlags;
	}

}
