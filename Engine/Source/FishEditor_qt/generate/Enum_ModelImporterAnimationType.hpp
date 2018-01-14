#pragma once

#include <FishEngine/ReflectEnum.hpp>
#include <ModelImporter.hpp>

namespace FishEngine
{


/**************************************************
* FishEditor::ModelImporterAnimationType
**************************************************/

// enum count
template<>
constexpr int EnumCount<FishEditor::ModelImporterAnimationType>() { return 3; }

// string array
static const char* ModelImporterAnimationTypeStrings[] =
{
    "None",
	"Generic",
	"Human"
};

// cstring array
template<>
inline constexpr const char** EnumToCStringArray<FishEditor::ModelImporterAnimationType>()
{
    return ModelImporterAnimationTypeStrings;
}

// index to enum
template<>
inline FishEditor::ModelImporterAnimationType ToEnum<FishEditor::ModelImporterAnimationType>(const int index)
{
    switch (index) {
    case 0: return FishEditor::ModelImporterAnimationType::None; break;
	case 1: return FishEditor::ModelImporterAnimationType::Generic; break;
	case 2: return FishEditor::ModelImporterAnimationType::Human; break;
	
    default: abort(); break;
    }
}

// enum to index
template<>
inline int EnumToIndex<FishEditor::ModelImporterAnimationType>(FishEditor::ModelImporterAnimationType e)
{
    switch (e) {
    case FishEditor::ModelImporterAnimationType::None: return 0; break;
	case FishEditor::ModelImporterAnimationType::Generic: return 1; break;
	case FishEditor::ModelImporterAnimationType::Human: return 2; break;
	
    default: abort(); break;
    }
}

// string to enum
template<>
inline FishEditor::ModelImporterAnimationType ToEnum<FishEditor::ModelImporterAnimationType>(const std::string& s)
{
    if (s == "None") return FishEditor::ModelImporterAnimationType::None;
	if (s == "Generic") return FishEditor::ModelImporterAnimationType::Generic;
	if (s == "Human") return FishEditor::ModelImporterAnimationType::Human;
	
    abort();
}


} // namespace FishEngine
