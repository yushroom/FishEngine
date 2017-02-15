#pragma once

#include <ReflectEnum.hpp>
#include <TextureImporterProperties.hpp>

namespace FishEngine
{


/**************************************************
* FishEditor::TextureImporterAlphaSource
**************************************************/

// enum count
template<>
constexpr int EnumCount<FishEditor::TextureImporterAlphaSource>() { return 3; }

// string array
static const char* TextureImporterAlphaSourceStrings[] =
{
    "None",
	"FromInput",
	"FromGrayScale"
};

// cstring array
template<>
inline constexpr const char** EnumToCStringArray<FishEditor::TextureImporterAlphaSource>()
{
    return TextureImporterAlphaSourceStrings;
}

// index to enum
template<>
inline FishEditor::TextureImporterAlphaSource ToEnum<FishEditor::TextureImporterAlphaSource>(const int index)
{
    switch (index) {
    case 0: return FishEditor::TextureImporterAlphaSource::None; break;
	case 1: return FishEditor::TextureImporterAlphaSource::FromInput; break;
	case 2: return FishEditor::TextureImporterAlphaSource::FromGrayScale; break;
	
    default: abort(); break;
    }
}

// enum to index
template<>
inline int EnumToIndex<FishEditor::TextureImporterAlphaSource>(FishEditor::TextureImporterAlphaSource e)
{
    switch (e) {
    case FishEditor::TextureImporterAlphaSource::None: return 0; break;
	case FishEditor::TextureImporterAlphaSource::FromInput: return 1; break;
	case FishEditor::TextureImporterAlphaSource::FromGrayScale: return 2; break;
	
    default: abort(); break;
    }
}

// string to enum
template<>
inline FishEditor::TextureImporterAlphaSource ToEnum<FishEditor::TextureImporterAlphaSource>(const std::string& s)
{
    if (s == "None") return FishEditor::TextureImporterAlphaSource::None;
	if (s == "FromInput") return FishEditor::TextureImporterAlphaSource::FromInput;
	if (s == "FromGrayScale") return FishEditor::TextureImporterAlphaSource::FromGrayScale;
	
    abort();
}


} // namespace FishEngine
