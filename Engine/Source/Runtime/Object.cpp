#include "Object.hpp"
#include "Scene.hpp"
#include "GameObject.hpp"

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

	template<>
	std::shared_ptr<GameObject> MakeShared()
	{
		return GameObject::Create();
	}

}
