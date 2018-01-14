#pragma once

#include <FishEngine/ReflectEnum.hpp>
#include <SceneViewEditor.hpp>

namespace FishEngine
{


/**************************************************
* FishEditor::TransformSpace
**************************************************/

// enum count
template<>
constexpr int EnumCount<FishEditor::TransformSpace>() { return 2; }

// string array
static const char* TransformSpaceStrings[] =
{
    "Global",
	"Local"
};

// cstring array
template<>
inline constexpr const char** EnumToCStringArray<FishEditor::TransformSpace>()
{
    return TransformSpaceStrings;
}

// index to enum
template<>
inline FishEditor::TransformSpace ToEnum<FishEditor::TransformSpace>(const int index)
{
    switch (index) {
    case 0: return FishEditor::TransformSpace::Global; break;
	case 1: return FishEditor::TransformSpace::Local; break;
	
    default: abort(); break;
    }
}

// enum to index
template<>
inline int EnumToIndex<FishEditor::TransformSpace>(FishEditor::TransformSpace e)
{
    switch (e) {
    case FishEditor::TransformSpace::Global: return 0; break;
	case FishEditor::TransformSpace::Local: return 1; break;
	
    default: abort(); break;
    }
}

// string to enum
template<>
inline FishEditor::TransformSpace ToEnum<FishEditor::TransformSpace>(const std::string& s)
{
    if (s == "Global") return FishEditor::TransformSpace::Global;
	if (s == "Local") return FishEditor::TransformSpace::Local;
	
    abort();
}


} // namespace FishEngine
