#include "GameObject.hpp"
#include "Scene.hpp"
#include "Gizmos.hpp"
#include "ModelImporter.hpp"
#include "BoxCollider.hpp"
#include "SphereCollider.hpp"
#include "CapsuleCollider.hpp"
#include "Rigidbody.hpp"

#include "TagManager.hpp"

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

    GameObject::GameObject(const std::string& name) : m_tagIndex(0)
    {
        //m_transform->m_gameObject = this;
        m_transform = std::make_shared<Transform>();
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
        auto go = Model::builtinModel(type)->CreateGameObject();
		//go->AddComponent<Rigidbody>();
        if (type == PrimitiveType::Cube)
            go->AddComponent<BoxCollider>();
        else if (type == PrimitiveType::Sphere)
            go->AddComponent<SphereCollider>();
        else if (type == PrimitiveType::Capsule)
            go->AddComponent<CapsuleCollider>();
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
