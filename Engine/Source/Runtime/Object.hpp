#ifndef Object_hpp
#define Object_hpp

#include "FishEngine.hpp"
#include <string>
#include <boost/uuid/uuid.hpp>

namespace FishEngine
{
    using UUID = boost::uuids::uuid;
    
    class FE_EXPORT Object
    {
    public:
        Object();
        virtual ~Object() = 0;

        static std::string StaticClassName()
        {
            return "Object";
        }

        virtual std::string ClassName() const
        {
            return "Object";
        }
        
        // The name of the object.
        virtual std::string name() const { return m_name; }
        void setName(const std::string& name) { m_name = name; }

        //virtual std::string ToString() const;

        // Removes a gameobject.
        static void Destroy(GameObjectPtr obj, const float t = 0.0f);

        // Removes a component.
        static void Destroy(ComponentPtr component, float t = 0.0f);

        static void Destroy(ScriptPtr script, float t = 0.0f);

        static void DestroyImmediate(GameObjectPtr obj);
        // Removes a component.
        static void DestroyImmediate(ComponentPtr component);
        static void DestroyImmediate(ScriptPtr script);

    protected:
        std::string m_name;
        UUID        m_uuid;

        friend class Serialization;
    };
}

#define InjectClassName(name) \
    static std::string StaticClassName() { \
        return #name; \
    } \
    virtual std::string ClassName() const override { \
        return StaticClassName(); \
    }

#endif // Object_hpp
