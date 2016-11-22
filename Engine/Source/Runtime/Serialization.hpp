#ifndef Serialization_hpp
#define Serialization_hpp

#include "FishEngine.hpp"

#include <cereal/cereal.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/xml.hpp>
//#include <cereal/archives/binary.hpp>
//#include <cereal/archives/portable_binary.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/list.hpp>

#include "Vector3.hpp"
#include "Quaternion.hpp"
#include "Matrix4x4.hpp"
#include "Object.hpp"
#include "GameObject.hpp"
#include "Component.hpp"
#include "Behaviour.hpp"
#include "Script.hpp"
#include "Transform.hpp"
#include "Camera.hpp"
#include "CameraController.hpp"
#include "Scene.hpp"

//CEREAL_REGISTER_POLYMORPHIC_RELATION(BaseClass, DerivedClassOne);
CEREAL_REGISTER_TYPE_WITH_NAME(FishEngine::Object, "Object");
CEREAL_REGISTER_TYPE_WITH_NAME(FishEngine::Component, "Component");
CEREAL_REGISTER_TYPE_WITH_NAME(FishEngine::Behaviour, "Behaviour");
CEREAL_REGISTER_TYPE_WITH_NAME(FishEngine::Camera, "Camera");
CEREAL_REGISTER_TYPE_WITH_NAME(FishEngine::Script, "Script");
CEREAL_REGISTER_TYPE_WITH_NAME(FishEngine::CameraController, "CameraController");
//CEREAL_REGISTER_TYPE_WITH_NAME(FishEngine::Scene, "Scene");

CEREAL_REGISTER_POLYMORPHIC_RELATION(FishEngine::Object, FishEngine::Component);
CEREAL_REGISTER_POLYMORPHIC_RELATION(FishEngine::Component, FishEngine::Behaviour);
CEREAL_REGISTER_POLYMORPHIC_RELATION(FishEngine::Behaviour, FishEngine::Camera);

CEREAL_REGISTER_POLYMORPHIC_RELATION(FishEngine::Behaviour, FishEngine::Script);
CEREAL_REGISTER_POLYMORPHIC_RELATION(FishEngine::Script, FishEngine::CameraController);

namespace FishEngine
{
    class FE_EXPORT Serialization
    {
    public:
        Serialization() = delete;
        
        template<typename Archive, typename T>
        static void Serialize(Archive& archive, T& v)
        {
            Debug::LogError("Serialization for %s not implemented", T::StaticClassName().c_str());
            abort();
        }
        
        template<typename Archive>
        static void Serialize(Archive& archive, Vector3& v)
        {
            archive(cereal::make_nvp("x", v.x),
                    cereal::make_nvp("y", v.y),
                    cereal::make_nvp("z", v.z));
        }

        template<typename Archive>
        static void Serialize(Archive& archive, Quaternion& q)
        {
            archive(cereal::make_nvp("x", q.x),
                cereal::make_nvp("y", q.y),
                cereal::make_nvp("z", q.z),
                cereal::make_nvp("w", q.w));
        }

        template<typename Archive>
        static void Serialize(Archive& archive, Object& value)
        {
            archive(cereal::make_nvp("name", value.m_name));
        }

#define CEREAL_BASE_CLASS(T) cereal::make_nvp(#T, cereal::base_class<T>(&value))

        template<typename Archive>
        static void Serialize(Archive& archive, Component& value)
        {
            archive(CEREAL_BASE_CLASS(Object));
        }

        template<typename Archive>
        static void Serialize(Archive& archive, Transform& value)
        {
            archive(
                CEREAL_BASE_CLASS(Component),
                cereal::make_nvp("localPosition", value.m_localPosition),
                cereal::make_nvp("localRotation", value.m_localRotation),
                cereal::make_nvp("localScale", value.m_localScale)
            );
        }

        template<typename Archive>
        static void Serialize(Archive& archive, Behaviour& value)
        {
            archive(
                CEREAL_BASE_CLASS(Component),
                cereal::make_nvp("enabled", value.m_enabled)
            );
        }

        template<typename Archive>
        static void Serialize(Archive& archive, Camera& value)
        {
            archive(
                CEREAL_BASE_CLASS(Behaviour),
                cereal::make_nvp("fieldOfView",     value.m_fieldOfView),
                cereal::make_nvp("orthographic",    value.m_orthographic),
                cereal::make_nvp("aspect",          value.m_aspect),
                cereal::make_nvp("isAspectSet",     value.m_isAspectSet),
                cereal::make_nvp("farClipPlane",    value.m_farClipPlane),
                cereal::make_nvp("nearClipPlane",   value.m_nearClipPlane),
                cereal::make_nvp("orthographic",    value.m_orthographic)
            );
        }

        template<typename Archive>
        static void Serialize(Archive& archive, GameObject& value)
        {
            archive(
                CEREAL_BASE_CLASS(Object),
                cereal::make_nvp("tag", value.m_tag),
                cereal::make_nvp("activeSelf", value.m_activeSelf),
                cereal::make_nvp("layer", value.m_layer),
                cereal::make_nvp("transform", value.m_transform),
                cereal::make_nvp("components", value.m_components),
                cereal::make_nvp("Scripts", value.m_scripts)
            );
        }

        template<typename Archive>
        static void Serialize(Archive& archive, Script& value)
        {
            archive(
                CEREAL_BASE_CLASS(Behaviour)
            );
        }

        template<typename Archive>
        static void Serialize(Archive& archive, CameraController& value)
        {
            archive(
                CEREAL_BASE_CLASS(Script)
            );
        }

        template<typename Archive>
        static void Serialize(Archive& archive, Scene& value)
        {
            archive(
                value.m_gameObjects
            );
        }
#undef CEREAL_BASE_CLASS
    };

#if 0
    // not work....
    // for cereal serialization
    template<typename Archive, typename T>
    static void serialize(Archive& archive, T& value)
    {
        Serialization::Serialize<Archive>(archive, value);
    }
    
#else
    
#define FE_SERIALIZE(T) \
    template<typename Archive> \
    static void serialize(Archive& archive, T& value) \
    { \
        Serialization::Serialize(archive, value); \
    }

    FE_SERIALIZE(Vector3);
    FE_SERIALIZE(Quaternion);

    FE_SERIALIZE(Object);
    FE_SERIALIZE(Component);
    FE_SERIALIZE(Transform);
    FE_SERIALIZE(GameObject);
    FE_SERIALIZE(Camera);
    FE_SERIALIZE(Script);
    FE_SERIALIZE(CameraController);
    FE_SERIALIZE(Scene);
    
#undef FE_SERIALIZE
#endif
}

#endif //Serialization_hpp
