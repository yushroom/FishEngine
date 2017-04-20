#pragma once

#include <ReflectEnum.hpp>
#include <SceneViewEditor.hpp>

namespace FishEngine
{


/**************************************************
* FishEditor::TransformPivot
**************************************************/

// enum count
template<>
constexpr int EnumCount<FishEditor::TransformPivot>() { return 2; }

// string array
static const char* TransformPivotStrings[] =
{
    "Pivot",
	"Center"
};

// cstring array
template<>
inline constexpr const char** EnumToCStringArray<FishEditor::TransformPivot>()
{
    return TransformPivotStrings;
}

// index to enum
template<>
inline FishEditor::TransformPivot ToEnum<FishEditor::TransformPivot>(const int index)
{
    switch (index) {
    case 0: return FishEditor::TransformPivot::Pivot; break;
	case 1: return FishEditor::TransformPivot::Center; break;
	
    default: abort(); break;
    }
}

// enum to index
template<>
inline int EnumToIndex<FishEditor::TransformPivot>(FishEditor::TransformPivot e)
{
    switch (e) {
    case FishEditor::TransformPivot::Pivot: return 0; break;
	case FishEditor::TransformPivot::Center: return 1; break;
	
    default: abort(); break;
    }
}

// string to enum
template<>
inline FishEditor::TransformPivot ToEnum<FishEditor::TransformPivot>(const std::string& s)
{
    if (s == "Pivot") return FishEditor::TransformPivot::Pivot;
	if (s == "Center") return FishEditor::TransformPivot::Center;
	
    abort();
}


} // namespace FishEngine
