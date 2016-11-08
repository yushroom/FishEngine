#ifndef FishEngine_h
#define FishEngine_h

#if defined(_WIN32) || defined(_WIN64)
#define FISHENGINE_PLATFORM_WINDOWS 1
#elif defined(__APPLE__)
#define FISHENGINE_PLATFORM_APPLE 1
#else //defined(__linux__)
#define FISHENGINE_PLATFORM_LINUX 1
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

    struct Avatar;
    struct Ray;
    struct Bounds;

    typedef std::shared_ptr<Object> PObject;
    typedef std::shared_ptr<GameObject> PGameObject;
    typedef std::shared_ptr<Transform> PTransform;
    typedef std::shared_ptr<Component> PComponent;
    typedef std::shared_ptr<Behaviour> PBehaviour;
    typedef std::shared_ptr<Camera> PCamera;
    typedef std::shared_ptr<Renderer> PRenderer;
    typedef std::shared_ptr<MeshFilter> PMeshFilter;
    typedef std::shared_ptr<MeshRenderer> PMeshRenderer;
    typedef std::shared_ptr<SkinnedMeshRenderer> PSkinnedMeshRenderer;
    typedef std::shared_ptr<Script> PScript;
    typedef std::shared_ptr<GameObject> PGameObject;
    typedef std::shared_ptr<Material> PMaterial;
    typedef std::shared_ptr<Animator> PAnimator;
    typedef std::shared_ptr<Mesh> PMesh;
    typedef std::shared_ptr<SimpleMesh> PSimpleMesh;
    typedef std::shared_ptr<Shader> PShader;
    typedef std::shared_ptr<Texture> PTexture;
    typedef std::shared_ptr<App> PApp;
    typedef std::shared_ptr<Light> PLight;
    typedef std::shared_ptr<RenderTexture> PRenderTexture;
    typedef std::shared_ptr<Model> PModel;
    typedef std::shared_ptr<Rigidbody> PRigidbody;
    typedef std::shared_ptr<Collider> PCollider;
    typedef std::shared_ptr<BoxCollider> PBoxCollider;
    typedef std::shared_ptr<CapsuleCollider> PCapsuleCollider;
    typedef std::shared_ptr<SphereCollider> PSphereCollider;
    typedef std::shared_ptr<MeshCollider> PMeshCollider;
    typedef std::shared_ptr<Avatar> PAvatar;
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
