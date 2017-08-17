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

	//static std::map<std::string, std::string> s_componentInheritance = {
	//	{"Rigidbody", "Component"},
	//	{"SphereCollider", "Collider"},
	//	{"Collider", "Component"},
	//	{"Script", "Behaviour"},
	//	{"Transform", "Component"},
	//	{"Camera", "Behaviour"},
	//	{"Component", ""},
	//	{"Behaviour", "Component"},
	//	{"CapsuleCollider", "Collider"},
	//	{"SkinnedMeshRenderer", "Renderer"},
	//	{"Light", "Behaviour"},
	//	{"Animator", "Component"},
	//	{"MeshFilter", "Component"},
	//	{"CameraController", "Script"},
	//	{"BoxCollider", "Collider"},
	//	{"Renderer", "Component"},
	//	{"MeshRenderer", "Renderer"},
	//	{"Skybox", "Behaviour"},
	//};

	static std::map<int, int> s_objectInheritance =
	{
		{ ClassID<FishEditor::AssetImporter>(), ClassID<FishEngine::Object>() },
		{ ClassID<FishEditor::DDSImporter>(), ClassID<FishEditor::AssetImporter>() },
		{ ClassID<FishEditor::ModelImporter>(), ClassID<FishEditor::AssetImporter>() },
		{ ClassID<FishEditor::NativeFormatImporter>(), ClassID<FishEditor::AssetImporter>() },
		{ ClassID<FishEditor::ShaderImporter>(), ClassID<FishEditor::AssetImporter>() },
		{ ClassID<FishEditor::TextureImporter>(), ClassID<FishEditor::AssetImporter>() },
		{ ClassID<FishEngine::Animation>(), ClassID<FishEngine::Behaviour>() },
		{ ClassID<FishEngine::AnimationClip>(), ClassID<FishEngine::Motion>() },
		{ ClassID<FishEngine::Animator>(), ClassID<FishEngine::Component>() },
		{ ClassID<FishEngine::Avatar>(), ClassID<FishEngine::Object>() },
		{ ClassID<FishEngine::Behaviour>(), ClassID<FishEngine::Component>() },
		{ ClassID<FishEngine::BoxCollider>(), ClassID<FishEngine::Collider>() },
		{ ClassID<FishEngine::Camera>(), ClassID<FishEngine::Behaviour>() },
		{ ClassID<FishEngine::CameraController>(), ClassID<FishEngine::Script>() },
		{ ClassID<FishEngine::CapsuleCollider>(), ClassID<FishEngine::Collider>() },
		{ ClassID<FishEngine::Collider>(), ClassID<FishEngine::Component>() },
		{ ClassID<FishEngine::Component>(), ClassID<FishEngine::Object>() },
		{ ClassID<FishEngine::Cubemap>(), ClassID<FishEngine::Texture>() },
		{ ClassID<FishEngine::GameObject>(), ClassID<FishEngine::Object>() },
		{ ClassID<FishEngine::Light>(), ClassID<FishEngine::Behaviour>() },
		{ ClassID<FishEngine::Material>(), ClassID<FishEngine::Object>() },
		{ ClassID<FishEngine::Mesh>(), ClassID<FishEngine::Object>() },
		{ ClassID<FishEngine::MeshFilter>(), ClassID<FishEngine::Component>() },
		{ ClassID<FishEngine::MeshRenderer>(), ClassID<FishEngine::Renderer>() },
		{ ClassID<FishEngine::Motion>(), ClassID<FishEngine::Object>() },
		{ ClassID<FishEngine::Object>(), -1 },
		{ ClassID<FishEngine::Prefab>(), ClassID<FishEngine::Object>() },
		{ ClassID<FishEngine::Renderer>(), ClassID<FishEngine::Component>() },
		{ ClassID<FishEngine::Rigidbody>(), ClassID<FishEngine::Component>() },
		{ ClassID<FishEngine::Script>(), ClassID<FishEngine::Behaviour>() },
		{ ClassID<FishEngine::Shader>(), ClassID<FishEngine::Object>() },
		{ ClassID<FishEngine::SkinnedMeshRenderer>(), ClassID<FishEngine::Renderer>() },
		{ ClassID<FishEngine::Skybox>(), ClassID<FishEngine::Behaviour>() },
		{ ClassID<FishEngine::SphereCollider>(), ClassID<FishEngine::Collider>() },
		{ ClassID<FishEngine::Texture>(), ClassID<FishEngine::Object>() },
		{ ClassID<FishEngine::Texture2D>(), ClassID<FishEngine::Texture>() },
		{ ClassID<FishEngine::Transform>(), ClassID<FishEngine::Component>() },
	};
}
