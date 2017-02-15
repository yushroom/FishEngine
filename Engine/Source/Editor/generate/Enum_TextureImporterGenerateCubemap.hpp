#pragma once

#include <ReflectEnum.hpp>
#include <TextureImporterProperties.hpp>

namespace FishEngine
{


/**************************************************
* FishEditor::TextureImporterGenerateCubemap
**************************************************/

// enum count
template<>
constexpr int EnumCount<FishEditor::TextureImporterGenerateCubemap>() { return 4; }

// string array
static const char* TextureImporterGenerateCubemapStrings[] =
{
    "Spheremap",
	"Cylindrical",
	"FullCubemap",
	"AutoCubemap"
};

// cstring array
template<>
inline constexpr const char** EnumToCStringArray<FishEditor::TextureImporterGenerateCubemap>()
{
    return TextureImporterGenerateCubemapStrings;
}

// index to enum
template<>
inline FishEditor::TextureImporterGenerateCubemap ToEnum<FishEditor::TextureImporterGenerateCubemap>(const int index)
{
    switch (index) {
    case 0: return FishEditor::TextureImporterGenerateCubemap::Spheremap; break;
	case 1: return FishEditor::TextureImporterGenerateCubemap::Cylindrical; break;
	case 2: return FishEditor::TextureImporterGenerateCubemap::FullCubemap; break;
	case 3: return FishEditor::TextureImporterGenerateCubemap::AutoCubemap; break;
	
    default: abort(); break;
    }
}

// enum to index
template<>
inline int EnumToIndex<FishEditor::TextureImporterGenerateCubemap>(FishEditor::TextureImporterGenerateCubemap e)
{
    switch (e) {
    case FishEditor::TextureImporterGenerateCubemap::Spheremap: return 0; break;
	case FishEditor::TextureImporterGenerateCubemap::Cylindrical: return 1; break;
	case FishEditor::TextureImporterGenerateCubemap::FullCubemap: return 2; break;
	case FishEditor::TextureImporterGenerateCubemap::AutoCubemap: return 3; break;
	
    default: abort(); break;
    }
}

// string to enum
template<>
inline FishEditor::TextureImporterGenerateCubemap ToEnum<FishEditor::TextureImporterGenerateCubemap>(const std::string& s)
{
    if (s == "Spheremap") return FishEditor::TextureImporterGenerateCubemap::Spheremap;
	if (s == "Cylindrical") return FishEditor::TextureImporterGenerateCubemap::Cylindrical;
	if (s == "FullCubemap") return FishEditor::TextureImporterGenerateCubemap::FullCubemap;
	if (s == "AutoCubemap") return FishEditor::TextureImporterGenerateCubemap::AutoCubemap;
	
    abort();
}


} // namespace FishEngine
