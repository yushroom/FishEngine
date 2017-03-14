#ifndef Object_hpp
#define Object_hpp

#include <string>

#include "FishEngine.hpp"
#include "Macro.hpp"
#include "HideFlags.hpp"
#include "ReflectClass.hpp"
#include "private/CloneUtility.hpp"

namespace FishEngine
{
	class InputArchive;
	class OutputArchive;
	
	class CloneUtility;

	class FE_EXPORT Object
	{
	public:
		
		Object()
		{
			static int sInstanceID = 0;
			m_instanceID = (++sInstanceID);
		}
		
		Object(Object const &) = delete;
		Object & operator=(Object const &) = delete;

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

		virtual ObjectPtr Clone(CloneUtility & cloneUtility) const;
		virtual void CopyValueTo(ObjectPtr & target, CloneUtility & cloneUtility) const;
		
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

		template<class T>
		static std::shared_ptr<T> Instantiate(std::shared_ptr<T> const & original);
		
		// TODO: make it protected
		PrefabPtr prefabInternal() const
		{
			return m_prefabInternal;
		}
		
		// TODO: make it protected
		void setPrefabInternal(PrefabPtr prefab)
		{
			m_prefabInternal = prefab;
		}

	protected:
		// Should the object be hidden, saved with the scene or modifiable by the user?
		HideFlags	m_hideFlags = HideFlags::None;
		std::string m_name;
		
		PrefabPtr	m_prefabParentObject;
		PrefabPtr	m_prefabInternal;

		Meta(NonSerializable)
		int			m_instanceID = 0;

	};	// end of Class Object

	template<class T>
	inline std::shared_ptr<T> MakeShared()
	{
		return std::make_shared<T>();
	}

	template<>
	std::shared_ptr<GameObject> MakeShared();

	template<class T>
	std::shared_ptr<T>
	FishEngine::Object::Instantiate(std::shared_ptr<T> const & original)
	{
		auto ret = MakeShared<T>();
		original->CopyValueTo(ret);
		return ret;
	}

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
