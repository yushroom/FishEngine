#include <FishEngine/Component_gen.hpp>
#include <FishEngine/GameObject.hpp>

#include <FishEngine/Camera.hpp>
#include <FishEngine/Animator.hpp>
#include <FishEngine/MeshFilter.hpp>
#include <FishEngine/Rigidbody.hpp>
#include <FishEngine/MeshRenderer.hpp>
#include <FishEngine/SkinnedMeshRenderer.hpp>
#include <FishEngine/BoxCollider.hpp>
#include <FishEngine/CapsuleCollider.hpp>
#include <FishEngine/SphereCollider.hpp>
#include <FishEngine/Rigidbody.hpp>
#include <FishEngine/Light.hpp>
#include <FishEngine/CameraController.hpp>

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
	if (componentClassName == "Light")
	{
		auto light = Light::Create();
		gameObject->AddComponent(light);
		return light;
	}
	CASE(CameraController)
#undef CASE
	//Debug::LogError("UNKNOWN component type name: %s", componentClassName.c_str());
	LogError("UNKNOWN component type name: " + componentClassName);
	//abort();
	return nullptr;
}
