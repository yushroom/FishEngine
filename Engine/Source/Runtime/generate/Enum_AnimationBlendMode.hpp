#pragma once

#include <ReflectEnum.hpp>
#include <AnimationBlendMode.hpp>

namespace FishEngine
{


/**************************************************
* FishEngine::AnimationBlendMode
**************************************************/

// enum count
template<>
constexpr int EnumCount<FishEngine::AnimationBlendMode>() { return 2; }

// string array
static const char* AnimationBlendModeStrings[] =
{
    "Blend",
	"Additive"
};

// cstring array
template<>
inline constexpr const char** EnumToCStringArray<FishEngine::AnimationBlendMode>()
{
    return AnimationBlendModeStrings;
}

// index to enum
template<>
inline FishEngine::AnimationBlendMode ToEnum<FishEngine::AnimationBlendMode>(const int index)
{
    switch (index) {
    case 0: return FishEngine::AnimationBlendMode::Blend; break;
	case 1: return FishEngine::AnimationBlendMode::Additive; break;
	
    default: abort(); break;
    }
}

// enum to index
template<>
inline int EnumToIndex<FishEngine::AnimationBlendMode>(FishEngine::AnimationBlendMode e)
{
    switch (e) {
    case FishEngine::AnimationBlendMode::Blend: return 0; break;
	case FishEngine::AnimationBlendMode::Additive: return 1; break;
	
    default: abort(); break;
    }
}

// string to enum
template<>
inline FishEngine::AnimationBlendMode ToEnum<FishEngine::AnimationBlendMode>(const std::string& s)
{
    if (s == "Blend") return FishEngine::AnimationBlendMode::Blend;
	if (s == "Additive") return FishEngine::AnimationBlendMode::Additive;
	
    abort();
}


} // namespace FishEngine
