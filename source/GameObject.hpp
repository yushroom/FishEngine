#ifndef GameObject_hpp
#define GameObject_hpp

#include "Transform.hpp"
#include "Script.hpp"
#include "Object.hpp"

namespace FishEngine
{
    // Base class for all entities in Unity scenes.
    // http://docs.unity3d.com/ScriptReference/GameObject.html
    class GameObject : public Object
    {
    public:
        //private:
        GameObject(const std::string& name);
        GameObject(const GameObject&) = delete;
        GameObject& operator=(const GameObject&) = delete;

    public:
        ~GameObject() = default;
        // {
        //     Debug::Log("~GameObject()");
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
        std::shared_ptr<Transform> transform() const {
            return m_transform;
        }

        //========== Public Functions ==========//

        // Returns the component of Type type if the game object has one attached, null if it doesn't.
        template<typename T>
        std::shared_ptr<T> GetComponent() const {
            for (auto& comp : m_components) {
                if (comp->ClassName() == T::StaticClassName()) {
                    return std::static_pointer_cast<T>(comp);
                }
            }
            //        for (auto& s : m_scripts) {
            //            if (s->ClassName() == T::StaticClassName()) {
            //                return std::static_pointer_cast<T>(s);
            //            }
            //        }
            return nullptr;
        }

        template<typename T>
        std::shared_ptr<T> GetScript() const {
            for (auto& s : m_scripts) {
                if (s->ClassName() == T::StaticClassName()) {
                    return std::static_pointer_cast<T>(s);
                }
            }
            return nullptr;
        }


        // Adds a component class named className to the game object.
        template<class T,
            std::enable_if_t<!std::is_base_of<Script, T>::value, int> = 42>
            void AddComponent(std::shared_ptr<T> component) {
            static_assert(std::is_base_of<Component, T>::value, "Component only");
            component->m_gameObject = m_transform->gameObject();
            m_components.push_back(component);
        }

        template<class T,
            std::enable_if_t<std::is_base_of<Script, T>::value, int> = 42>
            void AddComponent(std::shared_ptr<T> component) {
            static_assert(std::is_base_of<Script, T>::value, "Script only");
            component->m_gameObject = m_transform->gameObject();
            m_scripts.push_back(component);
        }

        //    void AddComponent(std::shared_ptr<Script> component) {
        //        component->m_gameObject = m_transform->gameObject();
        //        m_components.push_back(component);
        //    }

        template<class T,
            std::enable_if_t<!std::is_base_of<Script, T>::value, int> = 42>
            void AddComponent() {
            static_assert(std::is_base_of<Component, T>::value, "Component only");
            auto component = std::make_shared<T>();
            component->m_gameObject = m_transform->gameObject();
            m_components.push_back(component);
        }

        template<class T,
            std::enable_if_t<std::is_base_of<Script, T>::value, int> = 42>
            void AddComponent() {
            static_assert(std::is_base_of<Script, T>::value, "Script only");
            auto script = std::make_shared<T>();
            script->m_gameObject = m_transform->gameObject();
            m_scripts.push_back(script);
        }

        //    void AddScript(std::shared_ptr<Script> script) {
        //        script->m_gameObject = m_transform->gameObject();
        //        m_scripts.push_back(script);
        //    }

        //    template<typename T>
        //    void AddScript() {
        //        auto script = std::make_shared<T>();
        //        script->m_gameObject = m_transform->gameObject();
        //        m_scripts.push_back(script);
        //    }


        void RemoveComponent(std::shared_ptr<Component> component) {
            m_components.remove(component);
        }

        void RemoveScript(std::shared_ptr<Script> script) {
            m_scripts.remove(script);
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
        void OnDrawGizmos();

    private:
        friend class FishEditor::EditorGUI;
        std::list<std::shared_ptr<Component>> m_components;
        std::list<std::shared_ptr<Script>> m_scripts;

        bool m_activeSelf = true;
        int m_layer = 0;

        std::string m_tag;
        std::shared_ptr<Transform> m_transform;

        //static GameObject m_root;
        //static std::shared_ptr<GameObject> m_root;
    };
}

#endif // GameObject_hpp
