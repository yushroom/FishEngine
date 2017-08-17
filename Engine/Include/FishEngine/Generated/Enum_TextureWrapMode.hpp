#pragma once

#include <FishEngine/ReflectEnum.hpp>
#include <FishEngine/TextureProperty.hpp>

namespace FishEngine
{


/**************************************************
* FishEngine::TextureWrapMode
**************************************************/

// enum count
template<>
constexpr int EnumCount<FishEngine::TextureWrapMode>() { return 2; }

// string array
static const char* TextureWrapModeStrings[] =
{
    "Repeat",
	"Clamp"
};

// cstring array
template<>
inline constexpr const char** EnumToCStringArray<FishEngine::TextureWrapMode>()
{
    return TextureWrapModeStrings;
}

// index to enum
template<>
inline FishEngine::TextureWrapMode ToEnum<FishEngine::TextureWrapMode>(const int index)
{
    switch (index) {
    case 0: return FishEngine::TextureWrapMode::Repeat; break;
	case 1: return FishEngine::TextureWrapMode::Clamp; break;
	
    default: abort(); break;
    }
}

// enum to index
template<>
inline int EnumToIndex<FishEngine::TextureWrapMode>(FishEngine::TextureWrapMode e)
{
    switch (e) {
    case FishEngine::TextureWrapMode::Repeat: return 0; break;
	case FishEngine::TextureWrapMode::Clamp: return 1; break;
	
    default: abort(); break;
    }
}

// string to enum
template<>
inline FishEngine::TextureWrapMode ToEnum<FishEngine::TextureWrapMode>(const std::string& s)
{
    if (s == "Repeat") return FishEngine::TextureWrapMode::Repeat;
	if (s == "Clamp") return FishEngine::TextureWrapMode::Clamp;
	
    abort();
}


} // namespace FishEngine
