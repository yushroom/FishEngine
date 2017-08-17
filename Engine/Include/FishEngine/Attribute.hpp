#pragma once

#ifndef __REFLECTION_PARSER__

namespace FishEngine
{

	// auto-completion by IDE
#if 1
	// base class for meta attributes
	class Attribute
	{
	public:
		Attribute() = delete;
	};

	// Prevents MonoBehaviour of same type (or subtype) to be added more than once to a GameObject.
	class DisallowMultipleComponent : public Attribute
	{

	};

	// Makes a variable not show up in the inspector but be serialized.
	class HideInInspector : public Attribute
	{

	};

	class Serializable : public Attribute
	{

	};

	class NonSerializable : public Attribute
	{

	};

	class ExecuteInEditMode : public Attribute
	{

	};

	class AddComponentMenu : public Attribute
	{

	};

	// The RequireComponent attribute automatically adds required components as dependencies.
	class RequireComponent : public Attribute
	{

	};

#else

	enum class Attribute
	{
		DonotReflect,

		//Prevents MonoBehaviour of same type(or subtype) to be added more than once to a GameObject.
		DisallowMultipleComponent,

		//Makes a variable not show up in the inspector but be serialized.
		HideInInspector,

		Serializable,

		NonSerializable,

		//Makes all instances of a script execute in edit mode.
		ExecuteInEditMode,

		// The AddComponentMenu attribute allows you to place a script anywhere in the "Component" menu, instead of just the "Component-&gt;Scripts" menu.
		AddComponentMenu,

		//The RequireComponent attribute automatically adds required components as dependencies.
		RequireComponent,
	};
#endif
}

#endif
