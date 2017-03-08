#ifndef Object_hpp
#define Object_hpp

#include <string>

#include "FishEngine.hpp"
#include "Macro.hpp"
#include "HideFlags.hpp"
#include "ReflectClass.hpp"

namespace FishEngine
{
	class InputArchive;
	class OutputArchive;

	class FE_EXPORT Object
	{
	public:
		
		Object()
		{
			static int sInstanceID = 0;
			m_instanceID = (++sInstanceID);
		}

		virtual ~Object() = default;

		static const std::string StaticClassName()
		{
			return "Object";
		}

		virtual const std::string ClassName() const
		{
			return "Object";
		}
		
		virtual int ClassID() const = 0;
		
		//InjectSerializationFunctions(Object);
		virtual void Serialize(OutputArchive & archive) const;
		virtual void Deserialize(InputArchive & archive);
		
		// The name of the object.
		virtual inline std::string name() const { return m_name; }
		inline void setName(const std::string& name) { m_name = name; }

		// Should the object be hidden, saved with the scene or modifiable by the user ?
		inline HideFlags hideFlags() const { return m_hideFlags; }
		inline void setHideFlags(HideFlags hideFlags) { m_hideFlags = hideFlags; }

		inline int GetInstanceID() const
		{
			return m_instanceID;
		}

		//virtual std::string ToString() const;

		// Removes a gameobject.
		static void Destroy(GameObjectPtr obj, const float t = 0.0f);

		// Removes a component.
		static void Destroy(ComponentPtr component, float t = 0.0f);

		static void DestroyImmediate(GameObjectPtr obj);
		// Removes a component.
		static void DestroyImmediate(ComponentPtr component);
		static void DestroyImmediate(ScriptPtr script);

		virtual void CopyValueTo(ObjectPtr target) const;

		template<class T>
		static std::shared_ptr<T> Instantiate(std::shared_ptr<T> const & original)
		{
			auto ret = std::make_shared<T>();
			original->CopyValueTo(ret);
			return ret;
		}

	protected:
		// Should the object be hidden, saved with the scene or modifiable by the user?
		HideFlags	m_hideFlags = HideFlags::None;
		std::string m_name;

		Meta(NonSerializable)
		int			m_instanceID;

	};	// end of Class Object


	inline bool operator== (Object const & lhs, Object const & rhs)
	{ 
		return lhs.GetInstanceID() == rhs.GetInstanceID();
	}

	inline bool operator< (Object const & lhs, Object const & rhs)
	{
		return lhs.GetInstanceID() < rhs.GetInstanceID();
	}


	//template<>
	//GameObjectPtr Object::Instantiate(GameObjectPtr const & original);
}

#endif // Object_hpp
