#ifndef Object_hpp
#define Object_hpp

#include "FishEngine.hpp"
#include <string>
#include <boost/uuid/uuid.hpp>
#include "Macro.hpp"
#include "HideFlags.hpp"

namespace FishEngine
{
    using UUID = boost::uuids::uuid;
	
	class InputArchive;
	class OutputArchive;

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
		
		virtual int ClassID() const = 0;
        
        inline UUID const & GetGUID() const
        {
            return m_uuid;
        }
        
        //InjectSerializationFunctions(Object);
		virtual void Serialize(OutputArchive & archive) const;
		virtual void Deserialize(InputArchive & archive);
		
        // The name of the object.
        virtual std::string name() const { return m_name; }
        void setName(const std::string& name) { m_name = name; }

        //virtual std::string ToString() const;

        // Removes a gameobject.
        static void Destroy(GameObjectPtr obj, const float t = 0.0f);

        // Removes a component.
        static void Destroy(ComponentPtr component, float t = 0.0f);

        static void DestroyImmediate(GameObjectPtr obj);
        // Removes a component.
        static void DestroyImmediate(ComponentPtr component);
        static void DestroyImmediate(ScriptPtr script);

	protected:
		// Should the object be hidden, saved with the scene or modifiable by the user?
		HideFlags	m_hideFlags;
        std::string m_name;
        UUID        m_uuid;

    };


	inline bool operator== (Object const & lhs, Object const & rhs)
	{ 
		return lhs.GetGUID() == rhs.GetGUID();
	}

	inline bool operator< (Object const & lhs, Object const & rhs)
	{
		return lhs.GetGUID() < rhs.GetGUID();
	}
}

#endif // Object_hpp
