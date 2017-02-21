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
#include "Light.hpp"
#include "CameraController.hpp"

FishEngine::ComponentPtr FishEngine::
AddComponentToGameObject(
    std::string const &		componentClassName, 
    GameObjectPtr const &	gameObject)
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
	CASE(Light)
	CASE(CameraController)
#undef CASE
    Debug::LogError("UNKNOWN component type name: %s", componentClassName.c_str());
    //abort();
	return nullptr;
}
