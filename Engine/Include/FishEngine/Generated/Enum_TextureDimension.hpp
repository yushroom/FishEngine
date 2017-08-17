#pragma once

#include <ReflectEnum.hpp>
#include <TextureProperty.hpp>

namespace FishEngine
{


/**************************************************
* FishEngine::TextureDimension
**************************************************/

// enum count
template<>
constexpr int EnumCount<FishEngine::TextureDimension>() { return 7; }

// string array
static const char* TextureDimensionStrings[] =
{
    "Unknown",
	"None",
	"Tex2D",
	"Tex3D",
	"Cube",
	"Tex2DArray",
	"Any"
};

// cstring array
template<>
inline constexpr const char** EnumToCStringArray<FishEngine::TextureDimension>()
{
    return TextureDimensionStrings;
}

// index to enum
template<>
inline FishEngine::TextureDimension ToEnum<FishEngine::TextureDimension>(const int index)
{
    switch (index) {
    case 0: return FishEngine::TextureDimension::Unknown; break;
	case 1: return FishEngine::TextureDimension::None; break;
	case 2: return FishEngine::TextureDimension::Tex2D; break;
	case 3: return FishEngine::TextureDimension::Tex3D; break;
	case 4: return FishEngine::TextureDimension::Cube; break;
	case 5: return FishEngine::TextureDimension::Tex2DArray; break;
	case 6: return FishEngine::TextureDimension::Any; break;
	
    default: abort(); break;
    }
}

// enum to index
template<>
inline int EnumToIndex<FishEngine::TextureDimension>(FishEngine::TextureDimension e)
{
    switch (e) {
    case FishEngine::TextureDimension::Unknown: return 0; break;
	case FishEngine::TextureDimension::None: return 1; break;
	case FishEngine::TextureDimension::Tex2D: return 2; break;
	case FishEngine::TextureDimension::Tex3D: return 3; break;
	case FishEngine::TextureDimension::Cube: return 4; break;
	case FishEngine::TextureDimension::Tex2DArray: return 5; break;
	case FishEngine::TextureDimension::Any: return 6; break;
	
    default: abort(); break;
    }
}

// string to enum
template<>
inline FishEngine::TextureDimension ToEnum<FishEngine::TextureDimension>(const std::string& s)
{
    if (s == "Unknown") return FishEngine::TextureDimension::Unknown;
	if (s == "None") return FishEngine::TextureDimension::None;
	if (s == "Tex2D") return FishEngine::TextureDimension::Tex2D;
	if (s == "Tex3D") return FishEngine::TextureDimension::Tex3D;
	if (s == "Cube") return FishEngine::TextureDimension::Cube;
	if (s == "Tex2DArray") return FishEngine::TextureDimension::Tex2DArray;
	if (s == "Any") return FishEngine::TextureDimension::Any;
	
    abort();
}


} // namespace FishEngine
