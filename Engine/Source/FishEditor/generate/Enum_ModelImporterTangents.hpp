#pragma once

#include <FishEngine/ReflectEnum.hpp>
#include "../ModelImporter.hpp"

namespace FishEngine
{


/**************************************************
* FishEditor::ModelImporterTangents
**************************************************/

// enum count
template<>
constexpr int EnumCount<FishEditor::ModelImporterTangents>() { return 3; }

// string array
static const char* ModelImporterTangentsStrings[] =
{
    "Import",
	"Calculate",
	"None"
};

// cstring array
template<>
inline constexpr const char** EnumToCStringArray<FishEditor::ModelImporterTangents>()
{
    return ModelImporterTangentsStrings;
}

// index to enum
template<>
inline FishEditor::ModelImporterTangents ToEnum<FishEditor::ModelImporterTangents>(const int index)
{
    switch (index) {
    case 0: return FishEditor::ModelImporterTangents::Import; break;
	case 1: return FishEditor::ModelImporterTangents::Calculate; break;
	case 2: return FishEditor::ModelImporterTangents::None; break;
	
    default: abort(); break;
    }
}

// enum to index
template<>
inline int EnumToIndex<FishEditor::ModelImporterTangents>(FishEditor::ModelImporterTangents e)
{
    switch (e) {
    case FishEditor::ModelImporterTangents::Import: return 0; break;
	case FishEditor::ModelImporterTangents::Calculate: return 1; break;
	case FishEditor::ModelImporterTangents::None: return 2; break;
	
    default: abort(); break;
    }
}

// string to enum
template<>
inline FishEditor::ModelImporterTangents ToEnum<FishEditor::ModelImporterTangents>(const std::string& s)
{
    if (s == "Import") return FishEditor::ModelImporterTangents::Import;
	if (s == "Calculate") return FishEditor::ModelImporterTangents::Calculate;
	if (s == "None") return FishEditor::ModelImporterTangents::None;
	
    abort();
}


} // namespace FishEngine
