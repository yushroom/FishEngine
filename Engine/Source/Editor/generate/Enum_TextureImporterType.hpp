#pragma once

#include <ReflectEnum.hpp>
#include <TextureImporterProperties.hpp>

namespace FishEngine
{


/**************************************************
* FishEditor::TextureImporterType
**************************************************/

// enum count
template<>
constexpr int EnumCount<FishEditor::TextureImporterType>() { return 8; }

// string array
static const char* TextureImporterTypeStrings[] =
{
    "Default",
	"NormalMap",
	"GUI",
	"Sprite",
	"Cursor",
	"Cookie",
	"Lightmap",
	"SingleChannel"
};

// cstring array
template<>
inline constexpr const char** EnumToCStringArray<FishEditor::TextureImporterType>()
{
    return TextureImporterTypeStrings;
}

// index to enum
template<>
inline FishEditor::TextureImporterType ToEnum<FishEditor::TextureImporterType>(const int index)
{
    switch (index) {
    case 0: return FishEditor::TextureImporterType::Default; break;
	case 1: return FishEditor::TextureImporterType::NormalMap; break;
	case 2: return FishEditor::TextureImporterType::GUI; break;
	case 3: return FishEditor::TextureImporterType::Sprite; break;
	case 4: return FishEditor::TextureImporterType::Cursor; break;
	case 5: return FishEditor::TextureImporterType::Cookie; break;
	case 6: return FishEditor::TextureImporterType::Lightmap; break;
	case 7: return FishEditor::TextureImporterType::SingleChannel; break;
	
    default: abort(); break;
    }
}

// enum to index
template<>
inline int EnumToIndex<FishEditor::TextureImporterType>(FishEditor::TextureImporterType e)
{
    switch (e) {
    case FishEditor::TextureImporterType::Default: return 0; break;
	case FishEditor::TextureImporterType::NormalMap: return 1; break;
	case FishEditor::TextureImporterType::GUI: return 2; break;
	case FishEditor::TextureImporterType::Sprite: return 3; break;
	case FishEditor::TextureImporterType::Cursor: return 4; break;
	case FishEditor::TextureImporterType::Cookie: return 5; break;
	case FishEditor::TextureImporterType::Lightmap: return 6; break;
	case FishEditor::TextureImporterType::SingleChannel: return 7; break;
	
    default: abort(); break;
    }
}

// string to enum
template<>
inline FishEditor::TextureImporterType ToEnum<FishEditor::TextureImporterType>(const std::string& s)
{
    if (s == "Default") return FishEditor::TextureImporterType::Default;
	if (s == "NormalMap") return FishEditor::TextureImporterType::NormalMap;
	if (s == "GUI") return FishEditor::TextureImporterType::GUI;
	if (s == "Sprite") return FishEditor::TextureImporterType::Sprite;
	if (s == "Cursor") return FishEditor::TextureImporterType::Cursor;
	if (s == "Cookie") return FishEditor::TextureImporterType::Cookie;
	if (s == "Lightmap") return FishEditor::TextureImporterType::Lightmap;
	if (s == "SingleChannel") return FishEditor::TextureImporterType::SingleChannel;
	
    abort();
}


} // namespace FishEngine
