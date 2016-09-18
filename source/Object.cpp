#include "Object.hpp"

#include "Scene.hpp"

NAMESPACE_FISHENGINE_BEGIN

Object::~Object() = default;

void Object::Destroy(GameObject* obj, const float t) {
    Scene::Destroy(obj, t);
}

NAMESPACE_FISHENGINE_END
