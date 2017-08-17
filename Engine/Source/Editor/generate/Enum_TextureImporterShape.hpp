#pragma once

#include <FishEngine/ReflectEnum.hpp>
#include <TextureImporterProperties.hpp>

namespace FishEngine
{


/**************************************************
* FishEditor::TextureImporterShape
**************************************************/

// enum count
template<>
constexpr int EnumCount<FishEditor::TextureImporterShape>() { return 2; }

// string array
static const char* TextureImporterShapeStrings[] =
{
    "Texture2D",
	"TextureCube"
};

// cstring array
template<>
inline constexpr const char** EnumToCStringArray<FishEditor::TextureImporterShape>()
{
    return TextureImporterShapeStrings;
}

// index to enum
template<>
inline FishEditor::TextureImporterShape ToEnum<FishEditor::TextureImporterShape>(const int index)
{
    switch (index) {
    case 0: return FishEditor::TextureImporterShape::Texture2D; break;
	case 1: return FishEditor::TextureImporterShape::TextureCube; break;
	
    default: abort(); break;
    }
}

// enum to index
template<>
inline int EnumToIndex<FishEditor::TextureImporterShape>(FishEditor::TextureImporterShape e)
{
    switch (e) {
    case FishEditor::TextureImporterShape::Texture2D: return 0; break;
	case FishEditor::TextureImporterShape::TextureCube: return 1; break;
	
    default: abort(); break;
    }
}

// string to enum
template<>
inline FishEditor::TextureImporterShape ToEnum<FishEditor::TextureImporterShape>(const std::string& s)
{
    if (s == "Texture2D") return FishEditor::TextureImporterShape::Texture2D;
	if (s == "TextureCube") return FishEditor::TextureImporterShape::TextureCube;
	
    abort();
}


} // namespace FishEngine
