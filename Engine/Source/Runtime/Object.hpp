#ifndef Object_hpp
#define Object_hpp

#include "FishEngine.hpp"
#include <string>
#include <boost/uuid/uuid.hpp>
#include "Macro.hpp"

namespace FishEngine
{
    using UUID = boost::uuids::uuid;
    
    class FE_EXPORT Object
    {
    public:
        Object();
        virtual ~Object() = 0;

        static const std::string StaticClassName()
        {
            return "Object";
        }

        virtual const std::string ClassName() const
        {
            return "Object";
        }
        
        UUID GetGUID() const
        {
            return m_uuid;
        }
        
        // serialize
        template <typename Archive>
        friend Archive & operator << (Archive & archive, Object const & t);
        
        // deserialize
        template <typename Archive>
        friend Archive & operator >> (Archive & archive, Object & t);
        
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

#endif // Object_hpp
