#ifndef Component_hpp
#define Component_hpp

#include "Object.hpp"

NAMESPACE_FISHENGINE_BEGIN

// Base class for everything attached to GameObjects.
class Component : public Object
{
public:
    virtual std::string ClassName() const = 0;

    virtual ~Component() = default;
    
    virtual void OnInspectorGUI() {}
    virtual void OnDrawGizmos() {}

    // The game object this component is attached to. A component is always attached to a game object.
    std::shared_ptr<GameObject> gameObject() const { return m_gameObject.lock(); }
    
    // The tag of this game object.
    std::string tag() const;
    
    // The tag of this game object.
    virtual std::string name() const override;
    
    // The Transform attached to this GameObject (null if there is none attached).
    std::shared_ptr<Transform> transform() const;
    
    //virtual void Init() {}
    virtual void Update() {}
    
//protected:
private:
    friend class GameObject;
    friend class Scene;
    std::weak_ptr<GameObject> m_gameObject;
};

NAMESPACE_FISHENGINE_END

#define InjectClassName(name) \
    static std::string StaticClassName() { \
        return #name; \
    } \
    virtual std::string ClassName() const override { \
        return StaticClassName(); \
    }

#endif // Component_hpp
