#ifndef Component_hpp
#define Component_hpp

#include "Object.hpp"
#include "ReflectClass.hpp"

namespace FishEngine
{
	// Base class for everything attached to GameObjects.
	class FE_EXPORT Component : public Object
	{
	public:
		virtual ~Component() = default;

		InjectClassName(Component)

		// TODO: remove this
		virtual void OnInspectorGUI() {}

		virtual void OnDrawGizmos() {}
		virtual void OnDrawGizmosSelected() {}

		// The game object this component is attached to. A component is always attached to a game object.
		GameObjectPtr gameObject() const { return m_gameObject.lock(); }

		// The tag of this game object.
		std::string tag() const;

		// The tag of this game object.
		virtual std::string name() const override final;

		// The Transform attached to this GameObject (null if there is none attached).
		TransformPtr transform() const;

		template< class T >
		std::shared_ptr<T> GetComponent()
		{
			static_assert(std::is_base_of<Component, T>::value, "Component only");
			if (m_gameObject.expired())
				return nullptr;
			return gameObject()->GetComponent<T>();
		}

		template< class T >
		std::shared_ptr<T> GetComponentInChildren()
		{
			static_assert(std::is_base_of<Component, T>::value, "Component only");
			if (m_gameObject.expired())
				return nullptr;
			return gameObject()->GetComponentInChildren()<T>();
		}

		template<typename T>
		std::vector< std::shared_ptr<T> > GetComponents() const
		{
			static_assert(std::is_base_of<Component, T>::value, "Component only");
			if (m_gameObject.expired())
				return {};
			return gameObject()->GetComponents<T>();
		}

		template<typename T>
		void GetComponents(std::vector<std::shared_ptr<T>> & out_components) const
		{
			static_assert(std::is_base_of<Component, T>::value, "Component only");
			if (m_gameObject.expired())
				return;
			gameObject()->GetComponents<T>(out_components);
		}

		template< class T >
		std::vector< std::shared_ptr<T> > GetComponentsInChildren()
		{
			static_assert(std::is_base_of<Component, T>::value, "Component only");
			if (m_gameObject.expired())
				return nullptr;
			return gameObject()->GetComponentsInChildren<T>();
		}

		template< class T >
		void GetComponentsInChildren(std::vector< std::shared_ptr<T> > & out_components)
		{
			static_assert(std::is_base_of<Component, T>::value, "Component only");
			if (m_gameObject.expired())
				return;
			gameObject()->GetComponentsInChildren<T>(out_components);
		}


		//virtual void Init() {}
		virtual void Start() {}
		virtual void Update() {}
		virtual void OnDestroy() {}

		//static PComponent CreateComponent(const std::string& componentClassName);

	protected:
		virtual ComponentPtr Clone(CloneUtility & cloneUtility) const;
		void CopyValueTo(ComponentPtr target, CloneUtility & cloneUtility) const;

	private:
		friend class GameObject;
		friend class Scene;
		friend class FishEditor::SceneViewEditor;

		std::weak_ptr<GameObject> m_gameObject;

		Meta(NonSerializable)
		bool m_isStartFunctionCalled = false;
	};
}


#endif // Component_hpp
