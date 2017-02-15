#pragma once

#include <ReflectEnum.hpp>
#include <ModelImporter.hpp>

namespace FishEngine
{


/**************************************************
* FishEditor::ModelImporterMeshCompression
**************************************************/

// enum count
template<>
constexpr int EnumCount<FishEditor::ModelImporterMeshCompression>() { return 4; }

// string array
static const char* ModelImporterMeshCompressionStrings[] =
{
    "Off",
	"Low",
	"Medium",
	"High"
};

// cstring array
template<>
inline constexpr const char** EnumToCStringArray<FishEditor::ModelImporterMeshCompression>()
{
    return ModelImporterMeshCompressionStrings;
}

// index to enum
template<>
inline FishEditor::ModelImporterMeshCompression ToEnum<FishEditor::ModelImporterMeshCompression>(const int index)
{
    switch (index) {
    case 0: return FishEditor::ModelImporterMeshCompression::Off; break;
	case 1: return FishEditor::ModelImporterMeshCompression::Low; break;
	case 2: return FishEditor::ModelImporterMeshCompression::Medium; break;
	case 3: return FishEditor::ModelImporterMeshCompression::High; break;
	
    default: abort(); break;
    }
}

// enum to index
template<>
inline int EnumToIndex<FishEditor::ModelImporterMeshCompression>(FishEditor::ModelImporterMeshCompression e)
{
    switch (e) {
    case FishEditor::ModelImporterMeshCompression::Off: return 0; break;
	case FishEditor::ModelImporterMeshCompression::Low: return 1; break;
	case FishEditor::ModelImporterMeshCompression::Medium: return 2; break;
	case FishEditor::ModelImporterMeshCompression::High: return 3; break;
	
    default: abort(); break;
    }
}

// string to enum
template<>
inline FishEditor::ModelImporterMeshCompression ToEnum<FishEditor::ModelImporterMeshCompression>(const std::string& s)
{
    if (s == "Off") return FishEditor::ModelImporterMeshCompression::Off;
	if (s == "Low") return FishEditor::ModelImporterMeshCompression::Low;
	if (s == "Medium") return FishEditor::ModelImporterMeshCompression::Medium;
	if (s == "High") return FishEditor::ModelImporterMeshCompression::High;
	
    abort();
}


} // namespace FishEngine
