#ifndef Scene_hpp
#define Scene_hpp

#include "GameObject.hpp"
#include "Camera.hpp"

NAMESPACE_FISHENGINE_BEGIN

class Scene
{
public:
    static std::shared_ptr<Camera> mainCamera() {
        return m_mainCamera;
    }
    
    static std::shared_ptr<GameObject> CreateGameObject(const std::string& name);
    
    static void Init();
    static void Start();
    static void Update();
    static void Render();
    //static void OnEditorGUI();
    
    //typedef std::weak_ptr<GameObject> WPGameObject;
    typedef GameObject* WPGameObject;

    static void SelectGameObject(WPGameObject gameObject) {
        m_activeGameObject = gameObject;
    }
    
    static GameObject::PGameObject Find(const std::string& name);
    
    static void Destroy(GameObject* obj, const float t = 0.0f) {
        m_gameObjectsToBeDestroyed.push_back(obj);
    }
    
private:
    friend class RenderSystem;
    friend class EditorGUI;
    static std::shared_ptr<Camera> m_mainCamera;
    static std::list<std::shared_ptr<GameObject>> m_gameObjects;
    static std::vector<GameObject*> m_gameObjectsToBeDestroyed;
    
    // the selected GameObject in the editor
    static WPGameObject m_activeGameObject;
};

NAMESPACE_FISHENGINE_END

#endif /* Scene_hpp */
