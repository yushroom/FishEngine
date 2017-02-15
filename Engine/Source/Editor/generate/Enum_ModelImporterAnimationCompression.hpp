#pragma once

#include <ReflectEnum.hpp>
#include <ModelImporter.hpp>

namespace FishEngine
{


/**************************************************
* FishEditor::ModelImporterAnimationCompression
**************************************************/

// enum count
template<>
constexpr int EnumCount<FishEditor::ModelImporterAnimationCompression>() { return 4; }

// string array
static const char* ModelImporterAnimationCompressionStrings[] =
{
    "Off",
	"KeyframeReduction",
	"KeyframeReductionAndCompression",
	"Optimal"
};

// cstring array
template<>
inline constexpr const char** EnumToCStringArray<FishEditor::ModelImporterAnimationCompression>()
{
    return ModelImporterAnimationCompressionStrings;
}

// index to enum
template<>
inline FishEditor::ModelImporterAnimationCompression ToEnum<FishEditor::ModelImporterAnimationCompression>(const int index)
{
    switch (index) {
    case 0: return FishEditor::ModelImporterAnimationCompression::Off; break;
	case 1: return FishEditor::ModelImporterAnimationCompression::KeyframeReduction; break;
	case 2: return FishEditor::ModelImporterAnimationCompression::KeyframeReductionAndCompression; break;
	case 3: return FishEditor::ModelImporterAnimationCompression::Optimal; break;
	
    default: abort(); break;
    }
}

// enum to index
template<>
inline int EnumToIndex<FishEditor::ModelImporterAnimationCompression>(FishEditor::ModelImporterAnimationCompression e)
{
    switch (e) {
    case FishEditor::ModelImporterAnimationCompression::Off: return 0; break;
	case FishEditor::ModelImporterAnimationCompression::KeyframeReduction: return 1; break;
	case FishEditor::ModelImporterAnimationCompression::KeyframeReductionAndCompression: return 2; break;
	case FishEditor::ModelImporterAnimationCompression::Optimal: return 3; break;
	
    default: abort(); break;
    }
}

// string to enum
template<>
inline FishEditor::ModelImporterAnimationCompression ToEnum<FishEditor::ModelImporterAnimationCompression>(const std::string& s)
{
    if (s == "Off") return FishEditor::ModelImporterAnimationCompression::Off;
	if (s == "KeyframeReduction") return FishEditor::ModelImporterAnimationCompression::KeyframeReduction;
	if (s == "KeyframeReductionAndCompression") return FishEditor::ModelImporterAnimationCompression::KeyframeReductionAndCompression;
	if (s == "Optimal") return FishEditor::ModelImporterAnimationCompression::Optimal;
	
    abort();
}


} // namespace FishEngine
