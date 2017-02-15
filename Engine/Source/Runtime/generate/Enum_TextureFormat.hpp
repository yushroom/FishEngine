#pragma once

#include <ReflectEnum.hpp>
#include <TextureProperty.hpp>

namespace FishEngine
{


/**************************************************
* FishEngine::TextureFormat
**************************************************/

// enum count
template<>
constexpr int EnumCount<FishEngine::TextureFormat>() { return 12; }

// string array
static const char* TextureFormatStrings[] =
{
    "RGB24",
	"RGBA32",
	"ARGB32",
	"R16",
	"DXT1",
	"DXT5",
	"R8",
	"R32",
	"RG16",
	"RG32",
	"RGHalf",
	"RGFloat"
};

// cstring array
template<>
inline constexpr const char** EnumToCStringArray<FishEngine::TextureFormat>()
{
    return TextureFormatStrings;
}

// index to enum
template<>
inline FishEngine::TextureFormat ToEnum<FishEngine::TextureFormat>(const int index)
{
    switch (index) {
    case 0: return FishEngine::TextureFormat::RGB24; break;
	case 1: return FishEngine::TextureFormat::RGBA32; break;
	case 2: return FishEngine::TextureFormat::ARGB32; break;
	case 3: return FishEngine::TextureFormat::R16; break;
	case 4: return FishEngine::TextureFormat::DXT1; break;
	case 5: return FishEngine::TextureFormat::DXT5; break;
	case 6: return FishEngine::TextureFormat::R8; break;
	case 7: return FishEngine::TextureFormat::R32; break;
	case 8: return FishEngine::TextureFormat::RG16; break;
	case 9: return FishEngine::TextureFormat::RG32; break;
	case 10: return FishEngine::TextureFormat::RGHalf; break;
	case 11: return FishEngine::TextureFormat::RGFloat; break;
	
    default: abort(); break;
    }
}

// enum to index
template<>
inline int EnumToIndex<FishEngine::TextureFormat>(FishEngine::TextureFormat e)
{
    switch (e) {
    case FishEngine::TextureFormat::RGB24: return 0; break;
	case FishEngine::TextureFormat::RGBA32: return 1; break;
	case FishEngine::TextureFormat::ARGB32: return 2; break;
	case FishEngine::TextureFormat::R16: return 3; break;
	case FishEngine::TextureFormat::DXT1: return 4; break;
	case FishEngine::TextureFormat::DXT5: return 5; break;
	case FishEngine::TextureFormat::R8: return 6; break;
	case FishEngine::TextureFormat::R32: return 7; break;
	case FishEngine::TextureFormat::RG16: return 8; break;
	case FishEngine::TextureFormat::RG32: return 9; break;
	case FishEngine::TextureFormat::RGHalf: return 10; break;
	case FishEngine::TextureFormat::RGFloat: return 11; break;
	
    default: abort(); break;
    }
}

// string to enum
template<>
inline FishEngine::TextureFormat ToEnum<FishEngine::TextureFormat>(const std::string& s)
{
    if (s == "RGB24") return FishEngine::TextureFormat::RGB24;
	if (s == "RGBA32") return FishEngine::TextureFormat::RGBA32;
	if (s == "ARGB32") return FishEngine::TextureFormat::ARGB32;
	if (s == "R16") return FishEngine::TextureFormat::R16;
	if (s == "DXT1") return FishEngine::TextureFormat::DXT1;
	if (s == "DXT5") return FishEngine::TextureFormat::DXT5;
	if (s == "R8") return FishEngine::TextureFormat::R8;
	if (s == "R32") return FishEngine::TextureFormat::R32;
	if (s == "RG16") return FishEngine::TextureFormat::RG16;
	if (s == "RG32") return FishEngine::TextureFormat::RG32;
	if (s == "RGHalf") return FishEngine::TextureFormat::RGHalf;
	if (s == "RGFloat") return FishEngine::TextureFormat::RGFloat;
	
    abort();
}


} // namespace FishEngine
