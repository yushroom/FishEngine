#include "GameObject.hpp"

#include <deque>

#include "Scene.hpp"
#include "Gizmos.hpp"
#include "Mesh.hpp"
#include "MeshFilter.hpp"
#include "BoxCollider.hpp"
#include "SphereCollider.hpp"
#include "CapsuleCollider.hpp"
#include "Rigidbody.hpp"
#include "MeshRenderer.hpp"
#include "Script.hpp"

#include "TagManager.hpp"
#include "generate/Enum_PrimitiveType.hpp"

namespace FishEngine
{
	bool GameObject::activeInHierarchy() const
	{
		if (m_activeSelf && m_transform->parent() != nullptr)
		{
			return m_transform->parent()->gameObject()->activeInHierarchy();
		}
		return m_activeSelf;
	}
	
	GameObject::GameObject() : GameObject("")
	{
		
	}

	GameObject::GameObject(const std::string& name)
		: m_transform(std::make_shared<Transform>())
	{
		m_name = name;
	}


	GameObject::~GameObject()
	{
		//Debug::Log("GameObject::~GameObject: %s", m_name.c_str());
	}
	
	GameObjectPtr GameObject::Create()
	{
		auto go = std::make_shared<GameObject>();
		go->m_transform->m_gameObject = go;
		go->m_transform->m_gameObjectStrongRef = go;
		return go;
	}

	std::string const & GameObject::tag() const
	{
		return TagManager::IndexToTag(m_tagIndex);
	}
	
	void GameObject::setTag(const std::string& tag)
	{
		m_tagIndex = TagManager::TagToIndex(tag);
	}

	FishEngine::GameObjectPtr GameObject::CreatePrimitive(PrimitiveType type)
	{
		auto mesh = Mesh::builtinMesh(type);
		auto go = Scene::CreateGameObject(EnumToString(type));
		go->AddComponent<MeshFilter>()->SetMesh(mesh);
		switch (type)
		{
		case PrimitiveType::Cube:
			go->AddComponent<BoxCollider>();
			break;
		case PrimitiveType::Sphere:
			go->AddComponent<SphereCollider>();
			break;
		case PrimitiveType::Capsule:
			go->AddComponent<CapsuleCollider>();
			break;
		default:
			break;
		}
		go->AddComponent<MeshRenderer>()->SetMaterial(Material::defaultMaterial());
		return go;
	}

	GameObjectPtr GameObject::Find(const std::string& name)
	{
		return Scene::Find(name);
	}

	void GameObject::Update()
	{
		m_transform->Update();

		for (auto& c : m_components)
		{
			if (!c->m_isStartFunctionCalled)
			{
				if (IsScript(c->ClassID()))
				{
					auto s = std::static_pointer_cast<Script>(c);
					// TODO
					s->Awake();
					s->OnEnable();
				}
				c->Start();
				c->m_isStartFunctionCalled = true;
			}
			c->Update();
		}
	}

	void GameObject::OnDrawGizmos()
	{
		for (auto& c : m_components)
		{
			c->OnDrawGizmos();
			Gizmos::setColor(Color::green);
			Gizmos::setMatrix(Matrix4x4::identity);
		}
	}

	void GameObject::OnDrawGizmosSelected()
	{
		for (auto& c : m_components)
		{
			c->OnDrawGizmosSelected();
			Gizmos::setColor(Color::green);
			Gizmos::setMatrix(Matrix4x4::identity);
		}
	}

	GameObjectPtr GameObject::Clone(CloneUtility & cloneUtility)
	{
		// step 1. clone all gameobjects and transforms in the hierarchy tree
		std::deque<TransformPtr> workingQueue;
		workingQueue.push_back(m_transform);
		while (!workingQueue.empty())
		{
			auto current = workingQueue.front();
			workingQueue.pop_front();
			auto clonedGameObject = MakeShared<GameObject>();
			auto clonedTransform = clonedGameObject->m_transform;
			cloneUtility.m_clonedObject[current->gameObject()->GetInstanceID()] = clonedGameObject;
			cloneUtility.m_clonedObject[current->GetInstanceID()] = clonedTransform;
			auto parentOfCurrent = current->parent();
			if (parentOfCurrent != nullptr)
			{
				auto parentOfCloned = As<Transform>(cloneUtility.m_clonedObject[parentOfCurrent->GetInstanceID()]);
				clonedTransform->SetParent(parentOfCloned, false);
			}
			
			for (auto const & child : current->children())
			{
				workingQueue.push_back(child);
			}
		}
		
		// step 2. copy serializable data
		auto clonedGameObject = As<GameObject>(cloneUtility.m_clonedObject[GetInstanceID()]);
		this->CopyValueTo(clonedGameObject, cloneUtility);
		return clonedGameObject;
	}

	void GameObject::CopyValueTo(GameObjectPtr destGameObject, CloneUtility & cloneUtility)
	{
		Object::CopyValueTo(destGameObject, cloneUtility);
		//cloneUtility.Clone(this->m_components, ptr->m_components); // std::list<ComponentPtr>
		for (auto & component : this->m_components)
		{
			auto clonedComponent = component->Clone(cloneUtility);
			destGameObject->AddComponent(clonedComponent);
		}
		destGameObject->m_activeSelf = this->m_activeSelf; // bool
		destGameObject->m_layer = this->m_layer; // int
		destGameObject->m_tagIndex = this->m_tagIndex; // int
		//cloneUtility.Clone(this->m_transform, ptr->m_transform); // TransformPtr
		this->m_transform->CopyValueTo(destGameObject->transform(), cloneUtility);
	}

	void GameObject::Start()
	{
		for (auto& c : m_components)
		{
			if (IsScript(c->ClassID()))
			{
				// // TODO:
				auto s = std::static_pointer_cast<Script>(c);
				s->Awake();
				s->OnEnable();
			}
			c->Start();
			c->m_isStartFunctionCalled = true;
		}
	}

//	bool GameObject::AddComponent(const std::string &typeName)
//	{
//		if (typeName == Rigidbody::StaticClassName())
//		{
//			return AddComponent<Rigidbody>() != nullptr;
//		}
//		else
//		{
//			Debug::LogError("GameObject::AddComponent, unknown typeNmae %s", typeName.c_str());
//			abort();
//			return false;
//		}
//	}
}
