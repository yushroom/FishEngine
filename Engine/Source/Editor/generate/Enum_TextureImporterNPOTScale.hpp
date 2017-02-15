#pragma once

#include <ReflectEnum.hpp>
#include <TextureImporterProperties.hpp>

namespace FishEngine
{


/**************************************************
* FishEditor::TextureImporterNPOTScale
**************************************************/

// enum count
template<>
constexpr int EnumCount<FishEditor::TextureImporterNPOTScale>() { return 4; }

// string array
static const char* TextureImporterNPOTScaleStrings[] =
{
    "None",
	"ToNearest",
	"ToLarger",
	"ToSmaller"
};

// cstring array
template<>
inline constexpr const char** EnumToCStringArray<FishEditor::TextureImporterNPOTScale>()
{
    return TextureImporterNPOTScaleStrings;
}

// index to enum
template<>
inline FishEditor::TextureImporterNPOTScale ToEnum<FishEditor::TextureImporterNPOTScale>(const int index)
{
    switch (index) {
    case 0: return FishEditor::TextureImporterNPOTScale::None; break;
	case 1: return FishEditor::TextureImporterNPOTScale::ToNearest; break;
	case 2: return FishEditor::TextureImporterNPOTScale::ToLarger; break;
	case 3: return FishEditor::TextureImporterNPOTScale::ToSmaller; break;
	
    default: abort(); break;
    }
}

// enum to index
template<>
inline int EnumToIndex<FishEditor::TextureImporterNPOTScale>(FishEditor::TextureImporterNPOTScale e)
{
    switch (e) {
    case FishEditor::TextureImporterNPOTScale::None: return 0; break;
	case FishEditor::TextureImporterNPOTScale::ToNearest: return 1; break;
	case FishEditor::TextureImporterNPOTScale::ToLarger: return 2; break;
	case FishEditor::TextureImporterNPOTScale::ToSmaller: return 3; break;
	
    default: abort(); break;
    }
}

// string to enum
template<>
inline FishEditor::TextureImporterNPOTScale ToEnum<FishEditor::TextureImporterNPOTScale>(const std::string& s)
{
    if (s == "None") return FishEditor::TextureImporterNPOTScale::None;
	if (s == "ToNearest") return FishEditor::TextureImporterNPOTScale::ToNearest;
	if (s == "ToLarger") return FishEditor::TextureImporterNPOTScale::ToLarger;
	if (s == "ToSmaller") return FishEditor::TextureImporterNPOTScale::ToSmaller;
	
    abort();
}


} // namespace FishEngine
