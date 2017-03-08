#include "GameObject.hpp"
#include "Scene.hpp"
#include "Gizmos.hpp"
//#include "ModelImporter.hpp"
#include "Mesh.hpp"
#include "MeshFilter.hpp"
#include "BoxCollider.hpp"
#include "SphereCollider.hpp"
#include "CapsuleCollider.hpp"
#include "Rigidbody.hpp"
#include "MeshRenderer.hpp"

#include "TagManager.hpp"
#include "generate/Enum_PrimitiveType.hpp"

namespace FishEngine
{
	//GameObject::PGameObject GameObject::m_root = std::make_shared<GameObject>("Root");

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
	
	GameObjectPtr GameObject::Create()
	{
		auto go = std::make_shared<GameObject>();
		go->transform()->m_gameObject = go;
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

	//ObjectPtr GameObject::Clone() const
	//{
	//	auto ret = std::make_shared<GameObject>(m_name);
	//	GameObject::CopyValueTo(ret);
	//	return ret;
	//}

	void GameObject::CopyValueTo(ObjectPtr target) const
	{
		Object::CopyValueTo(target);
		auto go = std::dynamic_pointer_cast<GameObject>(target);
		m_transform->CopyValueTo(go->m_transform);
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
