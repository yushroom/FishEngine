#include <FishEngine/Object.hpp>
#include <FishEngine/Scene.hpp>
#include <FishEngine/GameObject.hpp>
#include <FishEngine/Prefab.hpp>

#include <map>
#include <set>
#include <boost/lexical_cast.hpp>

using namespace FishEngine;

static std::multimap<int, ObjectPtr> s_classIDToObjects;

namespace FishEngine
{
	std::multimap<int, ObjectPtr> Object::s_classIDToObjects;

	GameObjectPtr Object::Instantiate(GameObjectPtr const & original)
	{
		CloneUtility cu;
		GameObjectPtr cloned;
		if (original->m_prefabInternal != nullptr && original->m_prefabInternal->m_rootGameObject == original)
		{
			cloned = Instantiate(original->m_prefabInternal)->m_rootGameObject;
		}
		else
		{
			cloned = original->Clone(cu);
		}

		std::set<std::string> siblingNames;
		auto parent = original->transform()->parent();
		if (parent == nullptr)
		{
			for (auto & go : Scene::GameObjects())
			{
				siblingNames.insert(go->name());
			}
			Scene::AddGameObject(cloned);
		}
		else
		{
			for (auto & child : parent->children())
			{
				siblingNames.insert(child->name());
			}
			cloned->transform()->SetParent( parent, false );
		}
		
		int id = 1;
		std::string name = original->name();
		std::string prefix = name;
		if (name[name.size()-1] == ')')
		{
			auto pos = name.find_last_of("(");
			if (pos != std::string::npos)
			{
				prefix = name.substr(0, pos);
				std::string strID = name.substr(pos+1, name.size()-pos-2);
				try
				{
					id = boost::lexical_cast<int>(strID) + 1;
				}
				catch(const boost::bad_lexical_cast &)
				{
					id = 1;
				}
			}
		}

		auto findResult = siblingNames.find(name);
		if (findResult != siblingNames.end())
		{
			do {
				name = prefix + "(" + boost::lexical_cast<std::string>(id) + ")";
				auto it = siblingNames.find(name);
				if (it == siblingNames.end())
				{
					break;
				}
				id++;
			} while (true);
		}
		
		cloned->setName(name);
		return cloned;
	}

	FishEngine::PrefabPtr Object::Instantiate(PrefabPtr const & original)
	{
		CloneUtility cloneUtility;
		auto instance = MakeShared<Prefab>();
		instance->m_isPrefabParent = false;
		instance->m_parentPrefab = original->m_rootGameObject->prefabInternal();
		while (instance->m_parentPrefab->m_parentPrefab != nullptr)
		{
			instance->m_parentPrefab = instance->m_parentPrefab->m_parentPrefab;
		}
		cloneUtility.m_clonedObject[original->GetInstanceID()] = instance;
		instance->m_rootGameObject = original->m_rootGameObject->Clone(cloneUtility);
		return instance;
	}

	void Object::CopyValueTo(ObjectPtr target, CloneUtility & cloneUtility) const
	{
		cloneUtility.Clone(this->m_objectHideFlags, target->m_objectHideFlags); // FishEngine::HideFlags
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
