#pragma once

#include <ReflectEnum.hpp>
#include <Light.hpp>

namespace FishEngine
{


/**************************************************
* FishEngine::LightType
**************************************************/

// enum count
template<>
constexpr int EnumCount<FishEngine::LightType>() { return 4; }

// string array
static const char* LightTypeStrings[] =
{
    "Spot",
	"Directional",
	"Point",
	"Area"
};

// cstring array
template<>
inline constexpr const char** EnumToCStringArray<FishEngine::LightType>()
{
    return LightTypeStrings;
}

// index to enum
template<>
inline FishEngine::LightType ToEnum<FishEngine::LightType>(const int index)
{
    switch (index) {
    case 0: return FishEngine::LightType::Spot; break;
	case 1: return FishEngine::LightType::Directional; break;
	case 2: return FishEngine::LightType::Point; break;
	case 3: return FishEngine::LightType::Area; break;
	
    default: abort(); break;
    }
}

// enum to index
template<>
inline int EnumToIndex<FishEngine::LightType>(FishEngine::LightType e)
{
    switch (e) {
    case FishEngine::LightType::Spot: return 0; break;
	case FishEngine::LightType::Directional: return 1; break;
	case FishEngine::LightType::Point: return 2; break;
	case FishEngine::LightType::Area: return 3; break;
	
    default: abort(); break;
    }
}

// string to enum
template<>
inline FishEngine::LightType ToEnum<FishEngine::LightType>(const std::string& s)
{
    if (s == "Spot") return FishEngine::LightType::Spot;
	if (s == "Directional") return FishEngine::LightType::Directional;
	if (s == "Point") return FishEngine::LightType::Point;
	if (s == "Area") return FishEngine::LightType::Area;
	
    abort();
}


} // namespace FishEngine
