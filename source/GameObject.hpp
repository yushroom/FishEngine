#ifndef GameObject_hpp
#define GameObject_hpp

#include "Transform.hpp"
#include "Behaviour.hpp"
#include "Object.hpp"

NAMESPACE_FISHENGINE_BEGIN

// Base class for all entities in Unity scenes.
// http://docs.unity3d.com/ScriptReference/GameObject.html
class GameObject : public Object
{
public:
//private:
    GameObject(const std::string& name);
    
public:
    ~GameObject() = default;
    // {
    //     Debug::Log("~GameObject()");
    //     m_components.clear();
    //     m_scripts.clear();
    // }

    typedef std::shared_ptr<GameObject> PGameObject;

    //========== Variables ==========//

    // Is the GameObject active in the scene ?
    bool activeInHierarchy() const;

    // The local active state of this GameObject. (Read Only)
    bool activeSelf() const {
        return m_activeSelf;
    }

    // The layer the game object is in. A layer is in the range [0...31].
    int layer() const {
        return m_layer;
    }

    void SetLayer(int layer) {
        m_layer = layer;
    }

    // The tag of this game object.
    std::string tag() const {
        return m_tag;
    }

    void SetTag(const std::string& tag) {
        m_tag = tag;
    }

    // The Transform attached to this GameObject.
    Transform* transform() {
        return &m_transform;
    }

    //========== Public Functions ==========//

    // Returns the component of Type type if the game object has one attached, null if it doesn't.
    template<typename T>
    std::shared_ptr<T> GetComponent() {
        for (auto& comp : m_components) {
            if (comp->ClassName() == T::StaticClassName()) {
                return std::static_pointer_cast<T>(comp);
            }
        }
        return nullptr;
    }

    // Adds a component class named className to the game object.
    void AddComponent(std::shared_ptr<Component> component) {
        component->m_gameObject = this;
        m_components.push_back(component);
    }
    
    void AddScript(std::shared_ptr<Script> script) {
        script->m_gameObject = this;
        m_scripts.push_back(script);
    }

    // Activates/Deactivates the GameObject (activeSelf).
    void SetActive(bool value) {
        m_activeSelf = value;
    }

    //========== Static Functions ==========//

    // Finds a game object by name and returns it.
    static PGameObject Find(const std::string& name);


protected:
    friend class Scene;
    void Start();

    void Update();
    
    const Transform* transform() const {
        return &m_transform;
    }

private:
    friend class EditorGUI;
    std::vector<std::shared_ptr<Component>> m_components;
    std::vector<std::shared_ptr<Script>> m_scripts;
    
    bool m_activeSelf = true;
    int m_layer = 0;

    std::string m_tag;
    Transform m_transform;
    
    //static GameObject m_root;
    static std::shared_ptr<GameObject> m_root;
};

NAMESPACE_FISHENGINE_END

#endif // GameObject_hpp
