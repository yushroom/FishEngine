#include "Object.hpp"

#include "Scene.hpp"

namespace FishEngine
{

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


    void FishEngine::Object::DestroyImmediate(std::shared_ptr<Script> script)
    {
        Scene::DestroyImmediate(script);
    }

    void FishEngine::Object::DestroyImmediate(std::shared_ptr<Component> component)
    {
        Scene::DestroyImmediate(component);
    }

}