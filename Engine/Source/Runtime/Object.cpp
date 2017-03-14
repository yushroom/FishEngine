#include "Object.hpp"
#include "Scene.hpp"
#include "GameObject.hpp"

namespace FishEngine
{

	GameObjectPtr Object::Instantiate(GameObjectPtr const & original)
	{
		CloneUtility cu;
		auto cloned = original->Clone(cu);
		cloned->setName(original->name() + "-copy");
		auto parent = original->transform()->parent();
		if (parent == nullptr)
		{
			Scene::AddGameObject(cloned);
		}
		else
		{
			cloned->transform()->SetParent( parent, false );
		}
		return cloned;
	}

	ComponentPtr Object::Instantiate(ComponentPtr const & original)
	{
		abort();
		return nullptr;
	}

	void Object::CopyValueTo(ObjectPtr target, CloneUtility & cloneUtility) const
	{
		cloneUtility.Clone(this->m_hideFlags, target->m_hideFlags); // FishEngine::HideFlags
		cloneUtility.Clone(this->m_name, target->m_name); // std::string
		cloneUtility.Clone(this->m_prefabParentObject, target->m_prefabParentObject); // PrefabPtr
		cloneUtility.Clone(this->m_prefabInternal, target->m_prefabInternal); // PrefabPtr
	}

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
