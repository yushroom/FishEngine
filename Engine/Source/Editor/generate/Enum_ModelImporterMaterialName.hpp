#pragma once

#include <ReflectEnum.hpp>
#include <ModelImporter.hpp>

namespace FishEngine
{


/**************************************************
* FishEditor::ModelImporterMaterialName
**************************************************/

// enum count
template<>
constexpr int EnumCount<FishEditor::ModelImporterMaterialName>() { return 3; }

// string array
static const char* ModelImporterMaterialNameStrings[] =
{
    "BasedOnTextureName",
	"BasedOnMaterialName",
	"BasedOnModelNameAndMaterialName"
};

// cstring array
template<>
inline constexpr const char** EnumToCStringArray<FishEditor::ModelImporterMaterialName>()
{
    return ModelImporterMaterialNameStrings;
}

// index to enum
template<>
inline FishEditor::ModelImporterMaterialName ToEnum<FishEditor::ModelImporterMaterialName>(const int index)
{
    switch (index) {
    case 0: return FishEditor::ModelImporterMaterialName::BasedOnTextureName; break;
	case 1: return FishEditor::ModelImporterMaterialName::BasedOnMaterialName; break;
	case 2: return FishEditor::ModelImporterMaterialName::BasedOnModelNameAndMaterialName; break;
	
    default: abort(); break;
    }
}

// enum to index
template<>
inline int EnumToIndex<FishEditor::ModelImporterMaterialName>(FishEditor::ModelImporterMaterialName e)
{
    switch (e) {
    case FishEditor::ModelImporterMaterialName::BasedOnTextureName: return 0; break;
	case FishEditor::ModelImporterMaterialName::BasedOnMaterialName: return 1; break;
	case FishEditor::ModelImporterMaterialName::BasedOnModelNameAndMaterialName: return 2; break;
	
    default: abort(); break;
    }
}

// string to enum
template<>
inline FishEditor::ModelImporterMaterialName ToEnum<FishEditor::ModelImporterMaterialName>(const std::string& s)
{
    if (s == "BasedOnTextureName") return FishEditor::ModelImporterMaterialName::BasedOnTextureName;
	if (s == "BasedOnMaterialName") return FishEditor::ModelImporterMaterialName::BasedOnMaterialName;
	if (s == "BasedOnModelNameAndMaterialName") return FishEditor::ModelImporterMaterialName::BasedOnModelNameAndMaterialName;
	
    abort();
}


} // namespace FishEngine
