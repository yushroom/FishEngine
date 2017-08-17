#pragma once

#include <FishEngine/ReflectEnum.hpp>
#include <ModelImporter.hpp>

namespace FishEngine
{


/**************************************************
* FishEditor::ModelImporterNormals
**************************************************/

// enum count
template<>
constexpr int EnumCount<FishEditor::ModelImporterNormals>() { return 3; }

// string array
static const char* ModelImporterNormalsStrings[] =
{
    "Import",
	"Calculate",
	"None"
};

// cstring array
template<>
inline constexpr const char** EnumToCStringArray<FishEditor::ModelImporterNormals>()
{
    return ModelImporterNormalsStrings;
}

// index to enum
template<>
inline FishEditor::ModelImporterNormals ToEnum<FishEditor::ModelImporterNormals>(const int index)
{
    switch (index) {
    case 0: return FishEditor::ModelImporterNormals::Import; break;
	case 1: return FishEditor::ModelImporterNormals::Calculate; break;
	case 2: return FishEditor::ModelImporterNormals::None; break;
	
    default: abort(); break;
    }
}

// enum to index
template<>
inline int EnumToIndex<FishEditor::ModelImporterNormals>(FishEditor::ModelImporterNormals e)
{
    switch (e) {
    case FishEditor::ModelImporterNormals::Import: return 0; break;
	case FishEditor::ModelImporterNormals::Calculate: return 1; break;
	case FishEditor::ModelImporterNormals::None: return 2; break;
	
    default: abort(); break;
    }
}

// string to enum
template<>
inline FishEditor::ModelImporterNormals ToEnum<FishEditor::ModelImporterNormals>(const std::string& s)
{
    if (s == "Import") return FishEditor::ModelImporterNormals::Import;
	if (s == "Calculate") return FishEditor::ModelImporterNormals::Calculate;
	if (s == "None") return FishEditor::ModelImporterNormals::None;
	
    abort();
}


} // namespace FishEngine
