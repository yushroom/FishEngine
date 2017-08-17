#ifndef GameObject_hpp
#define GameObject_hpp

#include <memory>

#include "Transform.hpp"
#include "Object.hpp"
#include "ReflectClass.hpp"
#include "PrimitiveType.hpp"
#include "Generated/Class_ComponentInfo.hpp"
#include "Component_gen.hpp"

template< class T >
using PtrVector = std::vector< std::shared_ptr<T> >;

template< class T >
using PtrList = std::list< std::shared_ptr<T> >;

namespace FishEngine
{
	// Base class for all entities in Unity scenes.
	// http://docs.unity3d.com/ScriptReference/GameObject.html
	class FE_EXPORT GameObject : public Object
	{
	public:
		InjectClassName(GameObject)

		GameObject();
		GameObject(const std::string& name);
		
		// noncopyable
		GameObject(const GameObject&) = delete;
		GameObject& operator=(const GameObject&) = delete;

		static GameObjectPtr Create();
		
	public:
		~GameObject();

		/************************************************************************/
		/*                            Variables                                 */
		/************************************************************************/

		// Is the GameObject active in the scene ?
		bool activeInHierarchy() const;

		// The local active state of this GameObject. (Read Only)
		bool activeSelf() const
		{
			return m_activeSelf;
		}

		// The layer the game object is in. A layer is in the range [0...31].
		int layer() const { return m_layer; }
		void setLayer(int layer) { m_layer = layer; }

		
		// The tag of this game object.
		std::string const & tag() const;
		void setTag(const std::string& tag);

		
		// The Transform attached to this GameObject.
		TransformPtr transform() const
		{
			return m_transform;
		}

		/************************************************************************/
		/*                         Public Functions                             */
		/************************************************************************/

		// Returns the component of Type type if the game object has one attached, null if it doesn't.
		template<typename T>
		std::shared_ptr<T> GetComponent() const;

		// Returns the component of type T in the GameObject or any of its children using depth first search.
		// If includeInactive is set true, a component is returned only if it is found on an active GameObject.
		template<typename T>
		std::shared_ptr<T>
		GetComponentInChildren(bool includeInactive = false) const;

		template<typename T>
		PtrVector<T> GetComponents() const;
		
		template<typename T>
		PtrVector<T> GetComponentsInChildren(bool includeInactive = false) const;

		template<typename T>
		void GetComponents(PtrVector<T> & out_components) const;
		
		template<typename T>
		void GetComponentsInChildren(PtrVector<T> & out_components) const;

		// Adds a component class named className to the game object.
		bool AddComponent(ComponentPtr component);

		template<class T>
		std::shared_ptr<T> AddComponent();

		void RemoveComponent(ComponentPtr component)
		{
			m_components.remove(component);
		}

		// Activates/Deactivates the GameObject (activeSelf).
		void SetActive(bool value)
		{
			m_activeSelf = value;
		}

		/************************************************************************/
		/*                    Static Functions                                  */
		/************************************************************************/

		// Creates a game object with a primitive mesh renderer and appropriate collider.
		static GameObjectPtr CreatePrimitive(PrimitiveType type);

		// Finds a game object by name and returns it.
		static GameObjectPtr Find(const std::string& name);

		// make a new copy of this gameobject (deep copy)
		//virtual ObjectPtr Clone() const override;
		//virtual void CopyValueTo(ObjectPtr target) const override;
		
		std::list<ComponentPtr> const & Components() const
		{
			return m_components;
		}

	protected:
		void Start();
		void Update();
		void OnDrawGizmos();
		void OnDrawGizmosSelected();

		GameObjectPtr Clone(CloneUtility & cloneUtility);
		void CopyValueTo(GameObjectPtr target, CloneUtility & cloneUtility);

	private:
		friend class Object;
		friend class Scene;
		friend class Transform;
		friend class CloneUtility;
		friend class ::UIGameObjectHeader;
		friend class FishEditor::Inspector;
		friend class FishEditor::EditorGUI;
		friend class FishEditor::SceneViewEditor;

		std::list<ComponentPtr> m_components;

		bool			m_activeSelf	= true;
		int				m_layer			= 0;
		int				m_tagIndex		= 0;		// index in TagManager
		TransformPtr	m_transform;
	};
}


/************************************************************************/
/*                          Implementation                              */
/************************************************************************/

template<typename T>
std::shared_ptr<T> FishEngine::GameObject::GetComponent() const
{
	static_assert(std::is_base_of<Component, T>::value, "Component only");
	for (auto& comp : m_components)
	{
		int id = comp->ClassID();
		if ( IsSubClassOf<T>(id) )
		{
			return std::static_pointer_cast<T>(comp);
		}
	}
	return nullptr;
}


template<typename T>
std::shared_ptr<T> FishEngine::GameObject::GetComponentInChildren(bool includeInactive) const
{
	static_assert(std::is_base_of<Component, T>::value, "Component only");
	
	if ( !includeInactive && !activeInHierarchy() )
	{
		return nullptr;
	}
	
	auto comp = this->GetComponent<T>();
	if (comp != nullptr)
	{
		return comp;
	}
	
	// DFS
	for (auto & child : m_transform->m_children)
	{
		if (!includeInactive && !child->gameObject()->activeSelf())
		{
			continue;
		}
		auto comp = child->m_gameObjectStrongRef->GetComponentInChildren<T>();
		if (comp != nullptr)
		{
			return comp;
		}
	}
	return nullptr;
}


template<typename T>
PtrVector<T> FishEngine::GameObject::GetComponents() const
{
	static_assert(std::is_base_of<Component, T>::value, "Component only");
	std::vector<std::shared_ptr<T>> results;
	GetComponents<T>(results);
	return results;
}


template<typename T>
void FishEngine::GameObject::GetComponents(PtrVector<T> & out_components) const
{
	static_assert(std::is_base_of<Component, T>::value, "Component only");
	for (auto& comp : m_components)
	{
		int id = comp->ClassID();
		if (IsSubClassOf<T>(id))
		{
			out_components.push_back( std::static_pointer_cast<T>(comp) );
		}
	}
}


template<typename T>
PtrVector<T> FishEngine::GameObject::GetComponentsInChildren(bool includeInactive) const
{
	static_assert(std::is_base_of<Component, T>::value, "Component only");
	std::vector<std::shared_ptr<T>> result;
	this->GetComponentsInChildren(result);
	return result;
}


template<typename T>
void FishEngine::GameObject::GetComponentsInChildren(PtrVector<T> & out_components) const
{
	static_assert(std::is_base_of<Component, T>::value, "Component only");
	auto comp = this->GetComponent<T>();
	if (comp != nullptr)
	{
		out_components.push_back(comp);
	}
	for (auto & child : m_transform->m_children)
	{
		child->m_gameObjectStrongRef->GetComponentsInChildren(out_components);
	}
}


// Adds a component class named className to the game object.
inline bool FishEngine::GameObject::AddComponent(ComponentPtr component)
{
	// TODO
	//if (IsUniqueComponent<T>() && GetComponent<T>() != nullptr)
	//{
	//    return false;
	//}
	component->m_gameObject = m_transform->gameObject();
	m_components.push_back(component);
	component->Reset();
	return true;
}


template<class T>
std::shared_ptr<T> FishEngine::GameObject::AddComponent()
{
	static_assert(std::is_base_of<Component, T>::value, "Component only");
	if (IsUniqueComponent<T>() && GetComponent<T>() != nullptr)
	{
		return nullptr;
	}
	auto component = MakeShared<T>();
	component->m_gameObject = m_transform->gameObject();
	m_components.push_back(component);
	return component;
}


template< class T >
std::shared_ptr<T> FishEngine::Component::GetComponent()
{
	static_assert(std::is_base_of<Component, T>::value, "Component only");
	if (m_gameObject.expired())
		return nullptr;
	return gameObject()->GetComponent<T>();
}

template< class T >
std::shared_ptr<T> FishEngine::Component::GetComponentInChildren()
{
	static_assert(std::is_base_of<Component, T>::value, "Component only");
	if (m_gameObject.expired())
		return nullptr;
	return gameObject()->GetComponentInChildren<T>();
}

template<typename T>
std::vector< std::shared_ptr<T> > FishEngine::Component::GetComponents() const
{
	static_assert(std::is_base_of<Component, T>::value, "Component only");
	if (m_gameObject.expired())
		return {};
	return gameObject()->GetComponents<T>();
}

template<typename T>
void FishEngine::Component::GetComponents(std::vector<std::shared_ptr<T>> & out_components) const
{
	static_assert(std::is_base_of<Component, T>::value, "Component only");
	if (m_gameObject.expired())
		return;
	gameObject()->GetComponents<T>(out_components);
}

template< class T >
std::vector< std::shared_ptr<T> > FishEngine::Component::GetComponentsInChildren()
{
	static_assert(std::is_base_of<Component, T>::value, "Component only");
	if (m_gameObject.expired())
		return nullptr;
	return gameObject()->GetComponentsInChildren<T>();
}

template< class T >
void FishEngine::Component::GetComponentsInChildren(std::vector< std::shared_ptr<T> > & out_components)
{
	static_assert(std::is_base_of<Component, T>::value, "Component only");
	if (m_gameObject.expired())
		return;
	gameObject()->GetComponentsInChildren<T>(out_components);
}



#endif // GameObject_hpp
