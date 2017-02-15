#pragma once

#include <ReflectEnum.hpp>
#include <ModelImporter.hpp>

namespace FishEngine
{


/**************************************************
* FishEditor::ModelImporterMaterialSearch
**************************************************/

// enum count
template<>
constexpr int EnumCount<FishEditor::ModelImporterMaterialSearch>() { return 3; }

// string array
static const char* ModelImporterMaterialSearchStrings[] =
{
    "Local",
	"RecursiveUp",
	"Everywhere"
};

// cstring array
template<>
inline constexpr const char** EnumToCStringArray<FishEditor::ModelImporterMaterialSearch>()
{
    return ModelImporterMaterialSearchStrings;
}

// index to enum
template<>
inline FishEditor::ModelImporterMaterialSearch ToEnum<FishEditor::ModelImporterMaterialSearch>(const int index)
{
    switch (index) {
    case 0: return FishEditor::ModelImporterMaterialSearch::Local; break;
	case 1: return FishEditor::ModelImporterMaterialSearch::RecursiveUp; break;
	case 2: return FishEditor::ModelImporterMaterialSearch::Everywhere; break;
	
    default: abort(); break;
    }
}

// enum to index
template<>
inline int EnumToIndex<FishEditor::ModelImporterMaterialSearch>(FishEditor::ModelImporterMaterialSearch e)
{
    switch (e) {
    case FishEditor::ModelImporterMaterialSearch::Local: return 0; break;
	case FishEditor::ModelImporterMaterialSearch::RecursiveUp: return 1; break;
	case FishEditor::ModelImporterMaterialSearch::Everywhere: return 2; break;
	
    default: abort(); break;
    }
}

// string to enum
template<>
inline FishEditor::ModelImporterMaterialSearch ToEnum<FishEditor::ModelImporterMaterialSearch>(const std::string& s)
{
    if (s == "Local") return FishEditor::ModelImporterMaterialSearch::Local;
	if (s == "RecursiveUp") return FishEditor::ModelImporterMaterialSearch::RecursiveUp;
	if (s == "Everywhere") return FishEditor::ModelImporterMaterialSearch::Everywhere;
	
    abort();
}


} // namespace FishEngine
