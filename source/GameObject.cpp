#include "GameObject.hpp"
#include "Scene.hpp"

NAMESPACE_FISHENGINE_BEGIN

GameObject::PGameObject GameObject::m_root = std::make_shared<GameObject>("Root");

GameObject::PGameObject GameObject::Find(const std::string& name)
{
    return Scene::Find(name);
}

NAMESPACE_FISHENGINE_END
