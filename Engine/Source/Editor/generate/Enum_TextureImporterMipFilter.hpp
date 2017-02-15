#pragma once

#include <ReflectEnum.hpp>
#include <TextureImporterProperties.hpp>

namespace FishEngine
{


/**************************************************
* FishEditor::TextureImporterMipFilter
**************************************************/

// enum count
template<>
constexpr int EnumCount<FishEditor::TextureImporterMipFilter>() { return 2; }

// string array
static const char* TextureImporterMipFilterStrings[] =
{
    "BoxFilter",
	"KaiserFilter"
};

// cstring array
template<>
inline constexpr const char** EnumToCStringArray<FishEditor::TextureImporterMipFilter>()
{
    return TextureImporterMipFilterStrings;
}

// index to enum
template<>
inline FishEditor::TextureImporterMipFilter ToEnum<FishEditor::TextureImporterMipFilter>(const int index)
{
    switch (index) {
    case 0: return FishEditor::TextureImporterMipFilter::BoxFilter; break;
	case 1: return FishEditor::TextureImporterMipFilter::KaiserFilter; break;
	
    default: abort(); break;
    }
}

// enum to index
template<>
inline int EnumToIndex<FishEditor::TextureImporterMipFilter>(FishEditor::TextureImporterMipFilter e)
{
    switch (e) {
    case FishEditor::TextureImporterMipFilter::BoxFilter: return 0; break;
	case FishEditor::TextureImporterMipFilter::KaiserFilter: return 1; break;
	
    default: abort(); break;
    }
}

// string to enum
template<>
inline FishEditor::TextureImporterMipFilter ToEnum<FishEditor::TextureImporterMipFilter>(const std::string& s)
{
    if (s == "BoxFilter") return FishEditor::TextureImporterMipFilter::BoxFilter;
	if (s == "KaiserFilter") return FishEditor::TextureImporterMipFilter::KaiserFilter;
	
    abort();
}


} // namespace FishEngine
