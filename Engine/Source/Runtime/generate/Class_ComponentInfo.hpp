#pragma once

#include "../ClassID.hpp"

namespace FishEngine
{
    template<typename T>
    constexpr bool IsUniqueComponent()
    {
        static_assert(std::is_base_of<Component, T>::value, "Component only");
        return (std::is_base_of<Rigidbody, T>::value) || std::is_base_of<Animator, T>::value;
    }
	
	static std::map<std::string, std::string> s_componentInheritance = {
		{"Rigidbody", "Component"},
		{"SphereCollider", "Collider"},
		{"Collider", "Component"},
		{"Script", "Behaviour"},
		{"Transform", "Component"},
		{"Camera", "Behaviour"},
		{"Component", ""},
		{"Behaviour", "Component"},
		{"CapsuleCollider", "Collider"},
		{"SkinnedMeshRenderer", "Renderer"},
		{"Light", "Behaviour"},
		{"Animator", "Component"},
		{"MeshFilter", "Component"},
		{"CameraController", "Script"},
		{"BoxCollider", "Collider"},
		{"Renderer", "Component"},
		{"MeshRenderer", "Renderer"},
		{"Skybox", "Behaviour"},
	};
	
	static std::map<int, int> s_componentInheritance_int =
	{
		{ClassID<Rigidbody>(), ClassID<Component>()},
		{ClassID<SphereCollider>(), ClassID<Collider>()},
		{ClassID<Collider>(), ClassID<Component>()},
		{ClassID<Script>(), ClassID<Behaviour>()},
		{ClassID<Component>(), 0},
		{ClassID<Transform>(), ClassID<Component>()},
		{ClassID<Behaviour>(), ClassID<Component>()},
		{ClassID<CapsuleCollider>(), ClassID<Collider>()},
		{ClassID<SkinnedMeshRenderer>(), ClassID<Renderer>()},
		{ClassID<BoxCollider>(), ClassID<Collider>()},
		{ClassID<Light>(), ClassID<Behaviour>()},
		{ClassID<Animator>(), ClassID<Component>()},
		{ClassID<MeshFilter>(), ClassID<Component>()},
		{ClassID<CameraController>(), ClassID<Script>()},
		{ClassID<Camera>(), ClassID<Behaviour>()},
		{ClassID<Renderer>(), ClassID<Component>()},
		{ClassID<MeshRenderer>(), ClassID<Renderer>()},
		{ClassID<Skybox>(), ClassID<Behaviour>()},
	};

}
