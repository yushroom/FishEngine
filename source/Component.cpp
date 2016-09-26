#include "Component.hpp"
#include "GameObject.hpp"

#include "Debug.hpp"

NAMESPACE_FISHENGINE_BEGIN

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

NAMESPACE_FISHENGINE_END
