#include "GameObject.hpp"
#include "Scene.hpp"
#include "Gizmos.hpp"
#include "ModelImporter.hpp"
#include "BoxCollider.hpp"
#include "SphereCollider.hpp"
#include "CapsuleCollider.hpp"

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

    GameObject::GameObject(const std::string& name) : m_tag("Untagged")
    {
        //m_transform->m_gameObject = this;
        m_transform = std::make_shared<Transform>();
        m_name = name;
    }

    FishEngine::GameObjectPtr GameObject::CreatePrimitive(PrimitiveType type)
    {
        auto go = Model::builtinModel(type)->CreateGameObject();
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
                if (IsScript(c->ClassName()))
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
            if (IsScript(c->ClassName()))
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
}
