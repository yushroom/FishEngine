#pragma once

#include <FishEngine/ReflectEnum.hpp>
#include <SceneViewEditor.hpp>

namespace FishEngine
{


/**************************************************
* FishEditor::ShadingMode
**************************************************/

// enum count
template<>
constexpr int EnumCount<FishEditor::ShadingMode>() { return 3; }

// string array
static const char* ShadingModeStrings[] =
{
    "Shaded",
	"Wireframe",
	"ShadedWireframe"
};

// cstring array
template<>
inline constexpr const char** EnumToCStringArray<FishEditor::ShadingMode>()
{
    return ShadingModeStrings;
}

// index to enum
template<>
inline FishEditor::ShadingMode ToEnum<FishEditor::ShadingMode>(const int index)
{
    switch (index) {
    case 0: return FishEditor::ShadingMode::Shaded; break;
	case 1: return FishEditor::ShadingMode::Wireframe; break;
	case 2: return FishEditor::ShadingMode::ShadedWireframe; break;
	
    default: abort(); break;
    }
}

// enum to index
template<>
inline int EnumToIndex<FishEditor::ShadingMode>(FishEditor::ShadingMode e)
{
    switch (e) {
    case FishEditor::ShadingMode::Shaded: return 0; break;
	case FishEditor::ShadingMode::Wireframe: return 1; break;
	case FishEditor::ShadingMode::ShadedWireframe: return 2; break;
	
    default: abort(); break;
    }
}

// string to enum
template<>
inline FishEditor::ShadingMode ToEnum<FishEditor::ShadingMode>(const std::string& s)
{
    if (s == "Shaded") return FishEditor::ShadingMode::Shaded;
	if (s == "Wireframe") return FishEditor::ShadingMode::Wireframe;
	if (s == "ShadedWireframe") return FishEditor::ShadingMode::ShadedWireframe;
	
    abort();
}


} // namespace FishEngine
