#pragma once

#include <FishEngine/ReflectEnum.hpp>
#include <SceneViewEditor.hpp>

namespace FishEngine
{


/**************************************************
* FishEditor::TransformToolType
**************************************************/

// enum count
template<>
constexpr int EnumCount<FishEditor::TransformToolType>() { return 4; }

// string array
static const char* TransformToolTypeStrings[] =
{
    "None",
	"Translate",
	"Rotate",
	"Scale"
};

// cstring array
template<>
inline constexpr const char** EnumToCStringArray<FishEditor::TransformToolType>()
{
    return TransformToolTypeStrings;
}

// index to enum
template<>
inline FishEditor::TransformToolType ToEnum<FishEditor::TransformToolType>(const int index)
{
    switch (index) {
    case 0: return FishEditor::TransformToolType::None; break;
	case 1: return FishEditor::TransformToolType::Translate; break;
	case 2: return FishEditor::TransformToolType::Rotate; break;
	case 3: return FishEditor::TransformToolType::Scale; break;
	
    default: abort(); break;
    }
}

// enum to index
template<>
inline int EnumToIndex<FishEditor::TransformToolType>(FishEditor::TransformToolType e)
{
    switch (e) {
    case FishEditor::TransformToolType::None: return 0; break;
	case FishEditor::TransformToolType::Translate: return 1; break;
	case FishEditor::TransformToolType::Rotate: return 2; break;
	case FishEditor::TransformToolType::Scale: return 3; break;
	
    default: abort(); break;
    }
}

// string to enum
template<>
inline FishEditor::TransformToolType ToEnum<FishEditor::TransformToolType>(const std::string& s)
{
    if (s == "None") return FishEditor::TransformToolType::None;
	if (s == "Translate") return FishEditor::TransformToolType::Translate;
	if (s == "Rotate") return FishEditor::TransformToolType::Rotate;
	if (s == "Scale") return FishEditor::TransformToolType::Scale;
	
    abort();
}


} // namespace FishEngine
