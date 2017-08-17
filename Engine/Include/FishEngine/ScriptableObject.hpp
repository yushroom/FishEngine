#pragma once

#include "Object.hpp"

namespace FishEngine
{

	// A class you can derive from if you want to create objects that don't need to be attached to game objects.
	// This is most useful for assets which are only meant to store data.
	// To make it easy to create ScriptableObject instances that are bound to assets in your project, see CreateAssetMenuAttribute.
	class Meta(NonSerializable) ScriptableObject : public Object
	{
	public:
		//InjectClassName(ScriptableObject);

		ScriptableObject() = default;
		virtual ~ScriptableObject() = default;
		
		template<class T>
		static std::shared_ptr<T> CraeteInstance()
		{
			return std::make_shared<T>();
		}

		// This function is called when the ScriptableObject script is started.
		virtual void Awake() { }

		// This function is called when the scriptable object will be destroyed.
		virtual void OnDestroy() { }

		// This function is called when the scriptable object goes out of scope.
		virtual void OnDisable() { }

		// This function is called when the object is loaded.
		virtual void OnEnable() { }
	};
}
