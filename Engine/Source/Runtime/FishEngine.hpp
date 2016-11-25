#ifndef FishEngine_h
#define FishEngine_h

#if defined(_WIN32) || defined(_WIN64)
    #define FISHENGINE_PLATFORM_WINDOWS 1
#elif defined(__APPLE__)
    #define FISHENGINE_PLATFORM_APPLE 1
#else //defined(__linux__)
    #define FISHENGINE_PLATFORM_LINUX 1
#endif

#if defined(FishEngine_SHARED_LIB) && FISHENGINE_PLATFORM_WINDOWS
    #ifdef FishEngine_EXPORTS
        #define FE_EXPORT __declspec(dllexport)
    #else
        #define FE_EXPORT __declspec(dllimport)
    #endif
#else
    #define FE_EXPORT
#endif

#include <string>
#include <vector>
#include <map>
#include <list>
#include <memory>

namespace FishEngine
{
    class Object;
    class GameObject;
    class Transform;
    class Component;
    class Behaviour;
    class Camera;
    class Renderer;
    class MeshFilter;
    class MeshRenderer;
    class SkinnedMeshRenderer;
    class Script;
    class Debug;
    class GameObject;
    class GameLoop;
    class Input;
    class Material;
    class Animator;
    class Mesh;
    class SimpleMesh;
    class Shader;
    class Texture;
    class Texture2D;
    class Texture3D;
    class Cubemap;
    class Time;
    class App;
    class Mathf;
    class Vector2;
    class Vector3;
    class Vector4;
    class Quaternion;
    class Matrix4x4;
    class Light;
    class Color;
    class RenderTexture;
    class AssetImporter;
    class Model;
    class ModelImporter;
    class Rigidbody;
    class Collider;
    class BoxCollider;
    class CapsuleCollider;
    class SphereCollider;
    class MeshCollider;
    class Pipeline;
    class Serialization;
    class RenderSystem;
    class Graphics;

    struct Avatar;
    struct Ray;
    struct Bounds;

    typedef std::shared_ptr<Object> ObjectPtr;
    typedef std::shared_ptr<GameObject> GameObjectPtr;
    typedef std::shared_ptr<Transform> TransformPtr;
    typedef std::shared_ptr<Component> ComponentPtr;
    typedef std::shared_ptr<Behaviour> BehaviourPtr;
    typedef std::shared_ptr<Camera> CameraPtr;
    typedef std::shared_ptr<Renderer> RendererPtr;
    typedef std::shared_ptr<MeshFilter> MeshFilterPtr;
    typedef std::shared_ptr<MeshRenderer> MeshRendererPtr;
    typedef std::shared_ptr<SkinnedMeshRenderer> SkinnedMeshRendererPtr;
    typedef std::shared_ptr<Script> ScriptPtr;
    typedef std::shared_ptr<Material> MaterialPtr;
    typedef std::shared_ptr<Animator> AnimatorPtr;
    typedef std::shared_ptr<Mesh> MeshPtr;
    typedef std::shared_ptr<SimpleMesh> SimpleMeshPtr;
    typedef std::shared_ptr<Shader> ShaderPtr;
    typedef std::shared_ptr<Texture> TexturePtr;
    typedef std::shared_ptr<Texture2D> Texture2DPtr;
    typedef std::shared_ptr<Texture3D> Texture3DPtr;
    typedef std::shared_ptr<Cubemap> CubemapPtr;
    typedef std::shared_ptr<App> AppPtr;
    typedef std::shared_ptr<Light> LightPtr;
    typedef std::shared_ptr<RenderTexture> RenderTexturePtr;
    typedef std::shared_ptr<Model> ModelPtr;
    typedef std::shared_ptr<Rigidbody> RigidbodyPtr;
    typedef std::shared_ptr<Collider> ColliderPtr;
    typedef std::shared_ptr<BoxCollider> BoxColliderPtr;
    typedef std::shared_ptr<CapsuleCollider> CapsuleColliderPtr;
    typedef std::shared_ptr<SphereCollider> SphereColliderPtr;
    typedef std::shared_ptr<MeshCollider> MeshColliderPtr;
    typedef std::shared_ptr<Avatar> AvatarPtr;
}

// hack: inject FishEditor namespace
namespace FishEditor {
    class EditorGUI;
    class EditorRenderSystem;
    class FishEditorWindow;
    class SceneViewEditor;
    class EditorInput;
}

#if defined(_DEBUG) || defined(DEBUG)
#include <cassert>
#define Assert(exp) assert(exp)
#else
#define Assert(exp) (void(0))
#endif

#endif /* FishEngine_h */
