#pragma once

#include <ReflectEnum.hpp>
#include <Resources.hpp>

namespace FishEngine
{


/**************************************************
* FishEngine::SystemDirectoryType
**************************************************/

// enum count
template<>
constexpr int EnumCount<FishEngine::SystemDirectoryType>() { return 4; }

// string array
static const char* SystemDirectoryTypeStrings[] =
{
    "RootDir",
	"ShaderDir",
	"TextureDir",
	"FontDir"
};

// cstring array
template<>
inline constexpr const char** EnumToCStringArray<FishEngine::SystemDirectoryType>()
{
    return SystemDirectoryTypeStrings;
}

// index to enum
template<>
inline FishEngine::SystemDirectoryType ToEnum<FishEngine::SystemDirectoryType>(const int index)
{
    switch (index) {
    case 0: return FishEngine::SystemDirectoryType::RootDir; break;
	case 1: return FishEngine::SystemDirectoryType::ShaderDir; break;
	case 2: return FishEngine::SystemDirectoryType::TextureDir; break;
	case 3: return FishEngine::SystemDirectoryType::FontDir; break;
	
    default: abort(); break;
    }
}

// enum to index
template<>
inline int EnumToIndex<FishEngine::SystemDirectoryType>(FishEngine::SystemDirectoryType e)
{
    switch (e) {
    case FishEngine::SystemDirectoryType::RootDir: return 0; break;
	case FishEngine::SystemDirectoryType::ShaderDir: return 1; break;
	case FishEngine::SystemDirectoryType::TextureDir: return 2; break;
	case FishEngine::SystemDirectoryType::FontDir: return 3; break;
	
    default: abort(); break;
    }
}

// string to enum
template<>
inline FishEngine::SystemDirectoryType ToEnum<FishEngine::SystemDirectoryType>(const std::string& s)
{
    if (s == "RootDir") return FishEngine::SystemDirectoryType::RootDir;
	if (s == "ShaderDir") return FishEngine::SystemDirectoryType::ShaderDir;
	if (s == "TextureDir") return FishEngine::SystemDirectoryType::TextureDir;
	if (s == "FontDir") return FishEngine::SystemDirectoryType::FontDir;
	
    abort();
}


} // namespace FishEngine
