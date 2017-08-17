#include <FishEngine/Component.hpp>
#include <FishEngine/GameObject.hpp>

#include <FishEngine/Debug.hpp>

#include <FishEngine/Camera.hpp>
#include <FishEngine/Animator.hpp>
#include <FishEngine/MeshFilter.hpp>
#include <FishEngine/MeshRenderer.hpp>
#include <FishEngine/Rigidbody.hpp>
#include <FishEngine/SkinnedMeshRenderer.hpp>
#include <FishEngine/BoxCollider.hpp>
#include <FishEngine/CapsuleCollider.hpp>
#include <FishEngine/MeshCollider.hpp>
#include <FishEngine/SphereCollider.hpp>

namespace FishEngine
{

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

	TransformPtr Component::transform() const
	{
		if (m_gameObject.expired())
			return nullptr;
		return gameObject()->transform();
	}

//    std::shared_ptr<Component> Component::CreateComponent(const std::string& componentClassName)
//    {
//#define CASE(T) else if (componentClassName == #T) { return std::make_shared<T>(); }
//        if (componentClassName == "Camera") {
//            return std::make_shared<Camera>();
//        }
//        CASE(Animator)
//        CASE(MeshFilter)
//        CASE(MeshRenderer)
//        CASE(Rigidbody)
//        CASE(SkinnedMeshRenderer)
//        CASE(BoxCollider)
//        CASE(CapsuleCollider)
//        //CASE(MeshCollider)
//        CASE(SphereCollider)
//#undef CASE
//        Debug::LogError("UNKNOWN component type name: %s", componentClassName.c_str());
//        abort();
//        return nullptr;
//    }

}
