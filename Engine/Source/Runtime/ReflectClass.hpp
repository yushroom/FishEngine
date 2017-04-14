#pragma once

# if defined(__REFLECTION_PARSER__)
#   define Meta(...) __attribute__((annotate(#__VA_ARGS__)))
# else
#   define Meta(...)
# endif

#include "FishEngine.hpp"
#include "Attribute.hpp"

namespace FishEngine
{
	//FE_EXPORT bool IsDerivedFrom(const std::string& derivedClassName, const std::string& baseClassName);
	
	FE_EXPORT bool IsDerivedFrom(int derivedClassID, int baseClassID);


	//template<typename BaseClass>
	//bool IsSubClassOf(const std::string& derivedClassName)
	//{
	//	return IsDerivedFrom(derivedClassName, BaseClass::StaticClassName());
	//}
	
	template<typename BaseClass>
	bool IsSubClassOf(int derivedClassID)
	{
		return IsDerivedFrom(derivedClassID, ClassID<BaseClass>());
	}
	
	//inline bool IsBehaviour(const std::string& derivedClassName)
	//{
	//	return IsDerivedFrom(derivedClassName, "Behaviour");
	//}
	
	inline bool IsBehaviour(int derivedClassID)
	{
		return IsSubClassOf<Behaviour>(derivedClassID);
	}

	//inline bool IsScript(const std::string& derivedClassName)
	//{
	//	return IsDerivedFrom(derivedClassName, "Script");
	//}
	
	inline bool IsScript(int derivedClassID)
	{
		return IsSubClassOf<Script>(derivedClassID);
	}

	//inline bool IsComponent(const std::string& derivedClassName)
	//{
	//	return IsDerivedFrom(derivedClassName, "Component");
	//}
	
	inline bool IsComponent(int derivedClassID)
	{
		return IsSubClassOf<Component>(derivedClassID);
	}

	inline bool IsGameObject(int classid)
	{
		return classid == ClassID<GameObject>();
	}
}
