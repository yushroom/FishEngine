#ifndef FishEngine_h
#define FishEngine_h

#if defined(_WIN32) || defined(_WIN64)
#define FISHENGINE_PLATFORM_WINDOWS 1
#elif defined(__APPLE__)
#define FISHENGINE_PLATFORM_APPLE 1
#else //defined(__linux__)
#define FISHENGINE_PLATFORM_LINUX 1
#endif

#define NAMESPACE_FISHENGINE_BEGIN namespace FishEngine {
#define NAMESPACE_FISHENGINE_END }

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
    struct Avatar;
    struct Ray;
    struct Bounds;
}

// hack: inject FishEditor namespace
namespace FishEditor {
    class EditorGUI;
    class EditorRenderSystem;
    class FishEditorWindow;
    class SceneView;
}

namespace boost {
    namespace serialization {
        class access;
    }
}

#endif /* FishEngine_h */
