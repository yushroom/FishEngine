#ifndef Object_hpp
#define Object_hpp

#include "GLEnvironment.hpp"
#include <string>

NAMESPACE_FISHENGINE_BEGIN

class Object
{
public:
    // The name of the object.
    std::string name() const {return m_name; }
    void setName(const std::string& name) { m_name = name; }

    virtual ~Object() = 0;

    // Removes a gameobject.
    static void Destroy(GameObject* obj, const float t = 0.0f);
    
    // Removes a gameobject, component or asset.
    //static void Destroy(Object& obj, float t = 0.0f) {
        
    //}

    
protected:
    std::string m_name;
};

NAMESPACE_FISHENGINE_END

#endif // Object_hpp
