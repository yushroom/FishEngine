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

		virtual void Reset() {}
		
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
		std::shared_ptr<T> GetComponent();

		template< class T >
		std::shared_ptr<T> GetComponentInChildren();

		template< class T >
		std::vector< std::shared_ptr<T> > GetComponents() const;

		template< class T >
		void GetComponents(std::vector<std::shared_ptr<T>> & out_components) const;

		template< class T >
		std::vector< std::shared_ptr<T> > GetComponentsInChildren();

		template< class T >
		void GetComponentsInChildren(std::vector< std::shared_ptr<T> > & out_components);


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

//#include "GameObject.hpp"


#endif // Component_hpp
