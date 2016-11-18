#include "Component_gen.hpp"
#include "GameObject.hpp"

#include "Camera.hpp"
#include "Animator.hpp"
#include "MeshFilter.hpp"
#include "Rigidbody.hpp"
#include "MeshRenderer.hpp"
#include "SkinnedMeshRenderer.hpp"
#include "BoxCollider.hpp"
#include "CapsuleCollider.hpp"
#include "SphereCollider.hpp"
#include "Rigidbody.hpp"

FishEngine::ComponentPtr FishEngine::
AddComponentToGameObject(
    const std::string&  componentClassName, 
    GameObjectPtr&        gameObject)
{
#define CASE(T) if (componentClassName == #T) { return gameObject->AddComponent<T>(); }
    CASE(Camera)
    CASE(Animator)
    CASE(MeshFilter)
    CASE(MeshRenderer)
    CASE(Rigidbody)
    CASE(SkinnedMeshRenderer)
    CASE(BoxCollider)
    CASE(CapsuleCollider)
    //CASE(MeshCollider)
    CASE(SphereCollider)
#undef CASE
    Debug::LogError("UNKNOWN component type name: %s", componentClassName.c_str());
    abort();
}
