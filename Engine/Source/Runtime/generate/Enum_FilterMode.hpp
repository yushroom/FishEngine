#pragma once

#include <ReflectEnum.hpp>
#include <TextureProperty.hpp>

namespace FishEngine
{


/**************************************************
* FishEngine::FilterMode
**************************************************/

// enum count
template<>
constexpr int EnumCount<FishEngine::FilterMode>() { return 3; }

// string array
static const char* FilterModeStrings[] =
{
    "Point",
	"Bilinear",
	"Trilinear"
};

// cstring array
template<>
inline constexpr const char** EnumToCStringArray<FishEngine::FilterMode>()
{
    return FilterModeStrings;
}

// index to enum
template<>
inline FishEngine::FilterMode ToEnum<FishEngine::FilterMode>(const int index)
{
    switch (index) {
    case 0: return FishEngine::FilterMode::Point; break;
	case 1: return FishEngine::FilterMode::Bilinear; break;
	case 2: return FishEngine::FilterMode::Trilinear; break;
	
    default: abort(); break;
    }
}

// enum to index
template<>
inline int EnumToIndex<FishEngine::FilterMode>(FishEngine::FilterMode e)
{
    switch (e) {
    case FishEngine::FilterMode::Point: return 0; break;
	case FishEngine::FilterMode::Bilinear: return 1; break;
	case FishEngine::FilterMode::Trilinear: return 2; break;
	
    default: abort(); break;
    }
}

// string to enum
template<>
inline FishEngine::FilterMode ToEnum<FishEngine::FilterMode>(const std::string& s)
{
    if (s == "Point") return FishEngine::FilterMode::Point;
	if (s == "Bilinear") return FishEngine::FilterMode::Bilinear;
	if (s == "Trilinear") return FishEngine::FilterMode::Trilinear;
	
    abort();
}


} // namespace FishEngine
