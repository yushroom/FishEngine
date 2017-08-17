#ifndef Object_hpp
#define Object_hpp

#include <string>
#include <map>

#include "FishEngine.hpp"
#include "Macro.hpp"
#include "HideFlags.hpp"
#include "ReflectClass.hpp"
#include "Private/CloneUtility.hpp"

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
		
		// The name of the object.
		virtual inline std::string name() const { return m_name; }
		inline void setName(const std::string& name) { m_name = name; }

		// Should the object be hidden, saved with the scene or modifiable by the user ?
		inline HideFlags hideFlags() const { return m_objectHideFlags; }
		inline void setHideFlags(HideFlags hideFlags) { m_objectHideFlags = hideFlags; }

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

		static GameObjectPtr Instantiate(GameObjectPtr const & original);
		static ComponentPtr Instantiate(ComponentPtr const & original);
		static PrefabPtr Instantiate(PrefabPtr const & original);
		
		// TODO: make it protected
		PrefabPtr prefabInternal() const
		{
			return m_prefabInternal;
		}

		PrefabPtr prefabParentObject() const
		{
			return m_prefabParentObject;
		}
		
		// TODO: make it protected
		void setPrefabInternal(PrefabPtr prefab)
		{
			m_prefabInternal = prefab;
		}

	protected:
		void CopyValueTo(FishEngine::ObjectPtr target, CloneUtility & cloneUtility) const;

		// Should the object be hidden, saved with the scene or modifiable by the user?
		HideFlags	m_objectHideFlags = HideFlags::None;
		std::string m_name;
		
		PrefabPtr	m_prefabParentObject;
		PrefabPtr	m_prefabInternal;

		Meta(NonSerializable)
		int			m_instanceID = 0;

	public:	// TODO make it private
		static std::multimap<int, ObjectPtr> s_classIDToObjects;

		template<class T>
		static void FindObjectsOfType(std::vector<std::shared_ptr<T>> & out_objects);

		template<class T>
		static std::shared_ptr<T> FindObjectOfType();
	};	// end of Class Object

	template< class T, class... Args >
	inline std::shared_ptr<T> MakeShared(Args&&... args)
	{
		static_assert(std::is_base_of<Object, T>::value, "Object only");
		auto ret = std::make_shared<T>(std::forward<Args>(args)...);
		Object::s_classIDToObjects.emplace( ClassID<T>(), ret );
		return ret;
	}

	template<>
	FE_EXPORT std::shared_ptr<GameObject> MakeShared();

	template<>
	std::shared_ptr<Transform> MakeShared() = delete;

	inline bool operator== (Object const & lhs, Object const & rhs)
	{ 
		return lhs.GetInstanceID() == rhs.GetInstanceID();
	}

	inline bool operator< (Object const & lhs, Object const & rhs)
	{
		return lhs.GetInstanceID() < rhs.GetInstanceID();
	}

	template<class T>
	inline std::shared_ptr<T> As(ObjectPtr const & object)
	{
		return std::dynamic_pointer_cast<T>(object);
	}

	template<class T>
	void Object::FindObjectsOfType(std::vector<std::shared_ptr<T>> & out_objects)
	{
		out_objects.clear();
		auto result = s_classIDToObjects.equal_range(FishEngine::ClassID<T>());
		for (auto it = result.first; it != result.second; ++it)
		{
			out_objects.push_back(As<T>(it->second));
		}
	}

	template<class T>
	std::shared_ptr<T> Object::FindObjectOfType()
	{
		auto result = s_classIDToObjects.find(FishEngine::ClassID<T>());
		if (result != s_classIDToObjects.end())
		{
			return As<T>(result->second);
		}
		return nullptr;
	}
}

#endif // Object_hpp
