#pragma once

#include <ReflectEnum.hpp>
#include <QualitySettings.hpp>

namespace FishEngine
{


/**************************************************
* FishEngine::ShadowResolution
**************************************************/

// enum count
template<>
constexpr int EnumCount<FishEngine::ShadowResolution>() { return 4; }

// string array
static const char* ShadowResolutionStrings[] =
{
    "Low",
	"Medium",
	"High",
	"VeryHigh"
};

// cstring array
template<>
inline constexpr const char** EnumToCStringArray<FishEngine::ShadowResolution>()
{
    return ShadowResolutionStrings;
}

// index to enum
template<>
inline FishEngine::ShadowResolution ToEnum<FishEngine::ShadowResolution>(const int index)
{
    switch (index) {
    case 0: return FishEngine::ShadowResolution::Low; break;
	case 1: return FishEngine::ShadowResolution::Medium; break;
	case 2: return FishEngine::ShadowResolution::High; break;
	case 3: return FishEngine::ShadowResolution::VeryHigh; break;
	
    default: abort(); break;
    }
}

// enum to index
template<>
inline int EnumToIndex<FishEngine::ShadowResolution>(FishEngine::ShadowResolution e)
{
    switch (e) {
    case FishEngine::ShadowResolution::Low: return 0; break;
	case FishEngine::ShadowResolution::Medium: return 1; break;
	case FishEngine::ShadowResolution::High: return 2; break;
	case FishEngine::ShadowResolution::VeryHigh: return 3; break;
	
    default: abort(); break;
    }
}

// string to enum
template<>
inline FishEngine::ShadowResolution ToEnum<FishEngine::ShadowResolution>(const std::string& s)
{
    if (s == "Low") return FishEngine::ShadowResolution::Low;
	if (s == "Medium") return FishEngine::ShadowResolution::Medium;
	if (s == "High") return FishEngine::ShadowResolution::High;
	if (s == "VeryHigh") return FishEngine::ShadowResolution::VeryHigh;
	
    abort();
}


} // namespace FishEngine
