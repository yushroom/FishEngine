#pragma once

#include <ReflectEnum.hpp>
#include <RenderTexture.hpp>

namespace FishEngine
{


/**************************************************
* FishEngine::RenderTextureFormat
**************************************************/

// enum count
template<>
constexpr int EnumCount<FishEngine::RenderTextureFormat>() { return 3; }

// string array
static const char* RenderTextureFormatStrings[] =
{
    "ARGB32",
	"Dpeth",
	"Shadowmap"
};

// cstring array
template<>
inline constexpr const char** EnumToCStringArray<FishEngine::RenderTextureFormat>()
{
    return RenderTextureFormatStrings;
}

// index to enum
template<>
inline FishEngine::RenderTextureFormat ToEnum<FishEngine::RenderTextureFormat>(const int index)
{
    switch (index) {
    case 0: return FishEngine::RenderTextureFormat::ARGB32; break;
	case 1: return FishEngine::RenderTextureFormat::Dpeth; break;
	case 2: return FishEngine::RenderTextureFormat::Shadowmap; break;
	
    default: abort(); break;
    }
}

// enum to index
template<>
inline int EnumToIndex<FishEngine::RenderTextureFormat>(FishEngine::RenderTextureFormat e)
{
    switch (e) {
    case FishEngine::RenderTextureFormat::ARGB32: return 0; break;
	case FishEngine::RenderTextureFormat::Dpeth: return 1; break;
	case FishEngine::RenderTextureFormat::Shadowmap: return 2; break;
	
    default: abort(); break;
    }
}

// string to enum
template<>
inline FishEngine::RenderTextureFormat ToEnum<FishEngine::RenderTextureFormat>(const std::string& s)
{
    if (s == "ARGB32") return FishEngine::RenderTextureFormat::ARGB32;
	if (s == "Dpeth") return FishEngine::RenderTextureFormat::Dpeth;
	if (s == "Shadowmap") return FishEngine::RenderTextureFormat::Shadowmap;
	
    abort();
}


} // namespace FishEngine
