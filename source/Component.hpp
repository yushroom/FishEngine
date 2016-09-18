#ifndef Component_hpp
#define Component_hpp

#include "Object.hpp"

NAMESPACE_FISHENGINE_BEGIN

// Base class for everything attached to GameObjects.
class Component : public Object
{
public:
    virtual std::string ClassName() const = 0;
    
    
    virtual void OnEditorGUI() {};

    // The game object this component is attached to. A component is always attached to a game object.
    GameObject* gameObject() const { return m_gameObject; }
    
    // The tag of this game object.
    std::string tag() const;
    
    // The Transform attached to this GameObject (null if there is none attached).
    Transform* transform() const;
    
    friend class GameObject;
    
protected:
    GameObject* m_gameObject = nullptr;
    //std::shared_ptr<GameObject> m_gameObject = nullptr;
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
