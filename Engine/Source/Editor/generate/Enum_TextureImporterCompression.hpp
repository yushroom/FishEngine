#pragma once

#include <ReflectEnum.hpp>
#include <TextureImporterProperties.hpp>

namespace FishEngine
{


/**************************************************
* FishEditor::TextureImporterCompression
**************************************************/

// enum count
template<>
constexpr int EnumCount<FishEditor::TextureImporterCompression>() { return 4; }

// string array
static const char* TextureImporterCompressionStrings[] =
{
    "Uncompressed",
	"Compressed",
	"CompressedHQ",
	"CompressedLQ"
};

// cstring array
template<>
inline constexpr const char** EnumToCStringArray<FishEditor::TextureImporterCompression>()
{
    return TextureImporterCompressionStrings;
}

// index to enum
template<>
inline FishEditor::TextureImporterCompression ToEnum<FishEditor::TextureImporterCompression>(const int index)
{
    switch (index) {
    case 0: return FishEditor::TextureImporterCompression::Uncompressed; break;
	case 1: return FishEditor::TextureImporterCompression::Compressed; break;
	case 2: return FishEditor::TextureImporterCompression::CompressedHQ; break;
	case 3: return FishEditor::TextureImporterCompression::CompressedLQ; break;
	
    default: abort(); break;
    }
}

// enum to index
template<>
inline int EnumToIndex<FishEditor::TextureImporterCompression>(FishEditor::TextureImporterCompression e)
{
    switch (e) {
    case FishEditor::TextureImporterCompression::Uncompressed: return 0; break;
	case FishEditor::TextureImporterCompression::Compressed: return 1; break;
	case FishEditor::TextureImporterCompression::CompressedHQ: return 2; break;
	case FishEditor::TextureImporterCompression::CompressedLQ: return 3; break;
	
    default: abort(); break;
    }
}

// string to enum
template<>
inline FishEditor::TextureImporterCompression ToEnum<FishEditor::TextureImporterCompression>(const std::string& s)
{
    if (s == "Uncompressed") return FishEditor::TextureImporterCompression::Uncompressed;
	if (s == "Compressed") return FishEditor::TextureImporterCompression::Compressed;
	if (s == "CompressedHQ") return FishEditor::TextureImporterCompression::CompressedHQ;
	if (s == "CompressedLQ") return FishEditor::TextureImporterCompression::CompressedLQ;
	
    abort();
}


} // namespace FishEngine
