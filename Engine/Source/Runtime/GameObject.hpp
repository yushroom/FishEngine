#ifndef GameObject_hpp
#define GameObject_hpp

#include "Transform.hpp"
#include "Script.hpp"
#include "Object.hpp"
#include "ReflectClass.hpp"
#include "generate/Class_ComponentInfo.hpp"
#include "Component_gen.hpp"

#include <memory>
#include "PrimitiveType.hpp"

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
		~GameObject() = default;

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
		int layer() const
		{
			return m_layer;
		}

		void setLayer(int layer)
		{
			m_layer = layer;
		}

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
		std::shared_ptr<T> GetComponent() const
		{
			static_assert(std::is_base_of<Component, T>::value, "Component only");
			for (auto& comp : m_components)
			{
				int id = comp->ClassID();
				if ( IsSubClassOf<T>(id) )
				//if (comp->ClassName() == T::StaticClassName())
				{
					return std::static_pointer_cast<T>(comp);
				}
			}
			return nullptr;
		}

		//bool AddComponent(std::string const & typeName);

#if 1
		// Adds a component class named className to the game object.
		bool AddComponent(ComponentPtr component)
		{
			// TODO
			//if (IsUniqueComponent<T>() && GetComponent<T>() != nullptr)
			//{
			//    return false;
			//}
			component->m_gameObject = m_transform->gameObject();
			m_components.push_back(component);
			if (IsScript(component->ClassName()))
			{
				auto script = std::static_pointer_cast<Script>(component);
				script->Reset();
			}
			return true;
		}
#endif

		template<class T>
		std::shared_ptr<T> AddComponent()
		{
			static_assert(std::is_base_of<Component, T>::value, "Component only");
			if (IsUniqueComponent<T>() && GetComponent<T>() != nullptr)
			{
				return nullptr;
			}
			auto component = std::make_shared<T>();
			component->m_gameObject = m_transform->gameObject();
			m_components.push_back(component);
			return component;
		}

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
		friend class Scene;
		void Start();
		void Update();
		void OnDrawGizmos();
		void OnDrawGizmosSelected();

	private:
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

#endif // GameObject_hpp
