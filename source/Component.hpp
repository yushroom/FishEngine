#ifndef Component_hpp
#define Component_hpp

#include "Object.hpp"

namespace FishEngine
{
    // Base class for everything attached to GameObjects.
    class Component : public Object
    {
    public:
        virtual std::string ClassName() const = 0;

        virtual ~Component() = default;

        // TODO: remove this
        virtual void OnInspectorGUI() {}

        virtual void OnDrawGizmos() {}
        virtual void OnDrawGizmosSelected() {}

        // The game object this component is attached to. A component is always attached to a game object.
        PGameObject gameObject() const { return m_gameObject.lock(); }

        // The tag of this game object.
        std::string tag() const;

        // The tag of this game object.
        virtual std::string name() const override;

        // The Transform attached to this GameObject (null if there is none attached).
        PTransform transform() const;

        //virtual void Init() {}
        virtual void Start() {}
        virtual void Update() {}

        //static PComponent CreateComponent(const std::string& componentClassName);

        //protected:
    private:
        friend class GameObject;
        friend class Scene;
        friend class FishEditor::SceneView;
        friend class Serialization;

        std::weak_ptr<GameObject> m_gameObject;
        bool m_isStartFunctionCalled = false;

        //friend class boost::serialization::access;
        //template<class Archive>
        //inline void serialize(Archive& ar, const unsigned int version)
        //{
        //    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Object);
        //    //ar & BOOST_SERIALIZATION_NVP(m_gameObject);
        //}
    };
}

#define InjectClassName(name) \
    static std::string StaticClassName() { \
        return #name; \
    } \
    virtual std::string ClassName() const override { \
        return StaticClassName(); \
    }

#endif // Component_hpp
