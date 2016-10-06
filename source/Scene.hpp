#ifndef Scene_hpp
#define Scene_hpp

#include "GameObject.hpp"
#include "Camera.hpp"

NAMESPACE_FISHENGINE_BEGIN

class Scene
{
public:
    
    Scene() = delete;

    static std::shared_ptr<GameObject> CreateGameObject(const std::string& name);
    
    static void Init();
    static void Start();
    static void Update();
    static void RenderShadow(std::shared_ptr<Light>& light);
    static void OnDrawGizmos();
    //static void Render();
    //static void OnInspectorGUI();
    
    //typedef std::weak_ptr<GameObject> WPGameObject;
    typedef GameObject* WPGameObject;
    
    static GameObject::PGameObject Find(const std::string& name);
    
    static void Destroy(std::shared_ptr<GameObject> obj, const float t = 0.0f) {
        m_gameObjectsToBeDestroyed.push_back(obj);
    }

    static void Destroy(std::shared_ptr<Component> c, const float t = 0.0f) {
        m_componentsToBeDestroyed.push_back(c);
    }

    static void Destroy(std::shared_ptr<Script> s, const float t = 0.0f) {
        m_scriptsToBeDestroyed.push_back(s);
    }
    
private:
    friend class RenderSystem;
    friend class FishEditor::EditorGUI;
    friend class FishEditor::EditorRenderSystem;
    static std::list<std::shared_ptr<GameObject>> m_gameObjects;
    static std::vector<std::shared_ptr<GameObject>> m_gameObjectsToBeDestroyed;
    static std::vector<std::shared_ptr<Component>> m_componentsToBeDestroyed;
    static std::vector<std::shared_ptr<Script>> m_scriptsToBeDestroyed;
};

NAMESPACE_FISHENGINE_END

#endif /* Scene_hpp */
