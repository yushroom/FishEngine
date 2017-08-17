#pragma once

#include <ReflectEnum.hpp>
#include <TextureProperty.hpp>

namespace FishEngine
{


/**************************************************
* FishEngine::CubemapFace
**************************************************/

// enum count
template<>
constexpr int EnumCount<FishEngine::CubemapFace>() { return 7; }

// string array
static const char* CubemapFaceStrings[] =
{
    "Unknown",
	"PositiveX",
	"NegativeX",
	"PositiveY",
	"NegativeY",
	"PositiveZ",
	"NegativeZ"
};

// cstring array
template<>
inline constexpr const char** EnumToCStringArray<FishEngine::CubemapFace>()
{
    return CubemapFaceStrings;
}

// index to enum
template<>
inline FishEngine::CubemapFace ToEnum<FishEngine::CubemapFace>(const int index)
{
    switch (index) {
    case 0: return FishEngine::CubemapFace::Unknown; break;
	case 1: return FishEngine::CubemapFace::PositiveX; break;
	case 2: return FishEngine::CubemapFace::NegativeX; break;
	case 3: return FishEngine::CubemapFace::PositiveY; break;
	case 4: return FishEngine::CubemapFace::NegativeY; break;
	case 5: return FishEngine::CubemapFace::PositiveZ; break;
	case 6: return FishEngine::CubemapFace::NegativeZ; break;
	
    default: abort(); break;
    }
}

// enum to index
template<>
inline int EnumToIndex<FishEngine::CubemapFace>(FishEngine::CubemapFace e)
{
    switch (e) {
    case FishEngine::CubemapFace::Unknown: return 0; break;
	case FishEngine::CubemapFace::PositiveX: return 1; break;
	case FishEngine::CubemapFace::NegativeX: return 2; break;
	case FishEngine::CubemapFace::PositiveY: return 3; break;
	case FishEngine::CubemapFace::NegativeY: return 4; break;
	case FishEngine::CubemapFace::PositiveZ: return 5; break;
	case FishEngine::CubemapFace::NegativeZ: return 6; break;
	
    default: abort(); break;
    }
}

// string to enum
template<>
inline FishEngine::CubemapFace ToEnum<FishEngine::CubemapFace>(const std::string& s)
{
    if (s == "Unknown") return FishEngine::CubemapFace::Unknown;
	if (s == "PositiveX") return FishEngine::CubemapFace::PositiveX;
	if (s == "NegativeX") return FishEngine::CubemapFace::NegativeX;
	if (s == "PositiveY") return FishEngine::CubemapFace::PositiveY;
	if (s == "NegativeY") return FishEngine::CubemapFace::NegativeY;
	if (s == "PositiveZ") return FishEngine::CubemapFace::PositiveZ;
	if (s == "NegativeZ") return FishEngine::CubemapFace::NegativeZ;
	
    abort();
}


} // namespace FishEngine
