#include "Object.hpp"

#include "Scene.hpp"

namespace FishEngine
{

    Object::~Object() = default;

    void Object::Destroy(PGameObject obj, const float t /*= 0.0f*/) {
        Scene::Destroy(obj, t);
    }


    void Object::Destroy(PComponent component, float t /*= 0.0f*/)
    {
        Scene::Destroy(component, t);
    }


    void Object::Destroy(PScript script, float t /*= 0.0f*/)
    {
        Scene::Destroy(script, t);
    }


    void Object::DestroyImmediate(PScript script)
    {
        Scene::DestroyImmediate(script);
    }

    void Object::DestroyImmediate(PComponent component)
    {
        Scene::DestroyImmediate(component);
    }

    void Object::DestroyImmediate(PGameObject obj)
    {
        Scene::DestroyImmediate(obj);
    }

}