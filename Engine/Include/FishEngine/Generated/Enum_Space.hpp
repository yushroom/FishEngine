#pragma once

#include <ReflectEnum.hpp>
#include <Transform.hpp>

namespace FishEngine
{


/**************************************************
* FishEngine::Space
**************************************************/

// enum count
template<>
constexpr int EnumCount<FishEngine::Space>() { return 2; }

// string array
static const char* SpaceStrings[] =
{
    "World",
	"Self"
};

// cstring array
template<>
inline constexpr const char** EnumToCStringArray<FishEngine::Space>()
{
    return SpaceStrings;
}

// index to enum
template<>
inline FishEngine::Space ToEnum<FishEngine::Space>(const int index)
{
    switch (index) {
    case 0: return FishEngine::Space::World; break;
	case 1: return FishEngine::Space::Self; break;
	
    default: abort(); break;
    }
}

// enum to index
template<>
inline int EnumToIndex<FishEngine::Space>(FishEngine::Space e)
{
    switch (e) {
    case FishEngine::Space::World: return 0; break;
	case FishEngine::Space::Self: return 1; break;
	
    default: abort(); break;
    }
}

// string to enum
template<>
inline FishEngine::Space ToEnum<FishEngine::Space>(const std::string& s)
{
    if (s == "World") return FishEngine::Space::World;
	if (s == "Self") return FishEngine::Space::Self;
	
    abort();
}


} // namespace FishEngine
