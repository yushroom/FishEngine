#pragma once

#include <ReflectEnum.hpp>
#include <Renderer.hpp>

namespace FishEngine
{


/**************************************************
* FishEngine::ShadowCastingMode
**************************************************/

// enum count
template<>
constexpr int EnumCount<FishEngine::ShadowCastingMode>() { return 4; }

// string array
static const char* ShadowCastingModeStrings[] =
{
    "Off",
	"On",
	"TwoSided",
	"ShdowsOnly"
};

// cstring array
template<>
inline constexpr const char** EnumToCStringArray<FishEngine::ShadowCastingMode>()
{
    return ShadowCastingModeStrings;
}

// index to enum
template<>
inline FishEngine::ShadowCastingMode ToEnum<FishEngine::ShadowCastingMode>(const int index)
{
    switch (index) {
    case 0: return FishEngine::ShadowCastingMode::Off; break;
	case 1: return FishEngine::ShadowCastingMode::On; break;
	case 2: return FishEngine::ShadowCastingMode::TwoSided; break;
	case 3: return FishEngine::ShadowCastingMode::ShdowsOnly; break;
	
    default: abort(); break;
    }
}

// enum to index
template<>
inline int EnumToIndex<FishEngine::ShadowCastingMode>(FishEngine::ShadowCastingMode e)
{
    switch (e) {
    case FishEngine::ShadowCastingMode::Off: return 0; break;
	case FishEngine::ShadowCastingMode::On: return 1; break;
	case FishEngine::ShadowCastingMode::TwoSided: return 2; break;
	case FishEngine::ShadowCastingMode::ShdowsOnly: return 3; break;
	
    default: abort(); break;
    }
}

// string to enum
template<>
inline FishEngine::ShadowCastingMode ToEnum<FishEngine::ShadowCastingMode>(const std::string& s)
{
    if (s == "Off") return FishEngine::ShadowCastingMode::Off;
	if (s == "On") return FishEngine::ShadowCastingMode::On;
	if (s == "TwoSided") return FishEngine::ShadowCastingMode::TwoSided;
	if (s == "ShdowsOnly") return FishEngine::ShadowCastingMode::ShdowsOnly;
	
    abort();
}


} // namespace FishEngine
