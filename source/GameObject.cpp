#include "GameObject.hpp"
#include "Scene.hpp"
#include "Gizmos.hpp"
#include "ModelImporter.hpp"

namespace FishEngine
{
    //GameObject::PGameObject GameObject::m_root = std::make_shared<GameObject>("Root");

    bool GameObject::activeInHierarchy() const
    {
        if (m_activeSelf && m_transform->parent() != nullptr) {
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

    FishEngine::PGameObject GameObject::CreatePrimitive(PrimitiveType type)
    {
        //abort();
        return Model::builtinModel(type)->CreateGameObject();
    }

    PGameObject GameObject::Find(const std::string& name)
    {
        return Scene::Find(name);
    }

    void GameObject::Update()
    {
        m_transform->Update();
        for (auto& s : m_scripts)
        {
            if (!s->m_isStartFunctionCalled)
            {
                // TODO
                s->Awake();
                s->OnEnable();
                s->Start();
                s->m_isStartFunctionCalled = true;
            }
            s->Update();
        }

        for (auto& c : m_components)
        {
            if (!c->m_isStartFunctionCalled)
            {
                c->Start();
                c->m_isStartFunctionCalled = true;
            }
            c->Update();
        }
    }

    void GameObject::OnDrawGizmos()
    {
        for (auto& c : m_components) {
            c->OnDrawGizmos();
            Gizmos::setColor(Color::green);
        }

        for (auto& s : m_scripts) {
            s->OnDrawGizmos();
            Gizmos::setColor(Color::green);
        }
    }

    void GameObject::OnDrawGizmosSelected()
    {
        for (auto& c : m_components) {
            c->OnDrawGizmosSelected();
            Gizmos::setColor(Color::green);
        }

        for (auto& s : m_scripts) {
            s->OnDrawGizmosSelected();
            Gizmos::setColor(Color::green);
        }
    }

    void GameObject::Start()
    {
        for (auto& c : m_components) {
            c->Start();
            c->m_isStartFunctionCalled = true;
        }

        // TODO:
        for (auto& s : m_scripts) {
            s->Awake();
            s->OnEnable();
            s->Start();
            s->m_isStartFunctionCalled = true;
        }
    }
}
