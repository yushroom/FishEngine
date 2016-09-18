#include "GameObject.hpp"
#include "Scene.hpp"

NAMESPACE_FISHENGINE_BEGIN

GameObject::PGameObject GameObject::m_root = std::make_shared<GameObject>("Root");

bool FishEngine::GameObject::activeInHierarchy() const
{
    if (m_activeSelf && m_transform->parent() != nullptr) {
        return m_transform->parent()->gameObject()->activeInHierarchy();
    }
    return m_activeSelf;
}

FishEngine::GameObject::GameObject(const std::string& name) : m_tag("Untagged")
{
    //m_transform->m_gameObject = this;
    m_transform = std::make_shared<Transform>();
    m_name = name;
}

GameObject::PGameObject GameObject::Find(const std::string& name)
{
    return Scene::Find(name);
}

void FishEngine::GameObject::Update()
{
    m_transform->Update();
    for (auto& s : m_scripts) {
        s->Update();
    }
}


void FishEngine::GameObject::Start()
{
    //        for (auto& c : m_components) {
    //            c->init();
    //        }
    for (auto& s : m_scripts) {
        s->Start();
    }
}


NAMESPACE_FISHENGINE_END
