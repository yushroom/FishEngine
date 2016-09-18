#include "Object.hpp"

#include "Scene.hpp"

NAMESPACE_FISHENGINE_BEGIN

Object::~Object() = default;

void Object::Destroy(std::shared_ptr<GameObject> obj, const float t /*= 0.0f*/) {
    Scene::Destroy(obj, t);
}


void FishEngine::Object::Destroy(std::shared_ptr<Component> component, float t /*= 0.0f*/)
{
    Scene::Destroy(component, t);
}


void FishEngine::Object::Destroy(std::shared_ptr<Script> script, float t /*= 0.0f*/)
{
    Scene::Destroy(script, t);
}


NAMESPACE_FISHENGINE_END