#include "Component.hpp"
#include "GameObject.hpp"

#include "Debug.hpp"

#include "Camera.hpp"
#include "Animator.hpp"
#include "MeshFilter.hpp"
#include "MeshRenderer.hpp"
#include "Rigidbody.hpp"
#include "SkinnedMeshRenderer.hpp"
#include "BoxCollider.hpp"
#include "CapsuleCollider.hpp"
#include "MeshCollider.hpp"
#include "SphereCollider.hpp"

namespace FishEngine
{

    //Component::~Component()
    //{
    //    Debug::Log("~Component");
    //    //if (m_gameObject != nullptr) {
    //    //    m_gameObject->
    //    //}
    //}

    std::string Component::name() const
    {
        const auto& go = gameObject();
        if (go == nullptr)
            return Object::name();
        return gameObject()->name();
    }

    std::string Component::tag() const
    {
        if (m_gameObject.expired())
            return "";
        return gameObject()->tag();
    }

    std::shared_ptr<Transform> Component::transform() const
    {
        if (m_gameObject.expired())
            return nullptr;
        return gameObject()->transform();
    }

    std::shared_ptr<Component> Component::CreateComponent(const std::string& componentClassName)
    {
#define CASE(T) else if (componentClassName == #T) { return std::make_shared<T>(); }
        if (componentClassName == "Camera") {
            return std::make_shared<Camera>();
        }
        CASE(Animator)
        CASE(MeshFilter)
        CASE(MeshRenderer)
        CASE(Rigidbody)
        CASE(SkinnedMeshRenderer)
        CASE(BoxCollider)
        CASE(CapsuleCollider)
        CASE(MeshRenderer)
        CASE(SphereCollider)
#undef CASE
        Debug::LogError("UNKNOWN component type name: %s", componentClassName.c_str());
        abort();
        return nullptr;
    }

}
