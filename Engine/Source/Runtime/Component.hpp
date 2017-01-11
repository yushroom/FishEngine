#ifndef Component_hpp
#define Component_hpp

#include "Object.hpp"
#include "ReflectClass.hpp"

namespace FishEngine
{
    // Base class for everything attached to GameObjects.
    class FE_EXPORT Component : public Object
    {
    public:
        virtual ~Component() = default;

        InjectClassName(Component)

        // TODO: remove this
        virtual void OnInspectorGUI() {}

        virtual void OnDrawGizmos() {}
        virtual void OnDrawGizmosSelected() {}

        // The game object this component is attached to. A component is always attached to a game object.
        GameObjectPtr gameObject() const { return m_gameObject.lock(); }

        // The tag of this game object.
        std::string tag() const;

        // The tag of this game object.
        virtual std::string name() const override;

        // The Transform attached to this GameObject (null if there is none attached).
        TransformPtr transform() const;

        //virtual void Init() {}
        virtual void Start() {}
        virtual void Update() {}

        //static PComponent CreateComponent(const std::string& componentClassName);

        //protected:
    private:
        friend class GameObject;
        friend class Scene;
        friend class FishEditor::SceneViewEditor;

        std::weak_ptr<GameObject> m_gameObject;

		Meta(NonSerializable)
        bool m_isStartFunctionCalled = false;
    };
}


#endif // Component_hpp
