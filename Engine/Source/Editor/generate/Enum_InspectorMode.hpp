#pragma once

#include <FishEngine/ReflectEnum.hpp>
#include <InspectorMode.hpp>

namespace FishEngine
{


/**************************************************
* FishEditor::InspectorMode
**************************************************/

// enum count
template<>
constexpr int EnumCount<FishEditor::InspectorMode>() { return 3; }

// string array
static const char* InspectorModeStrings[] =
{
    "Normal",
	"Debug",
	"DebugInternal"
};

// cstring array
template<>
inline constexpr const char** EnumToCStringArray<FishEditor::InspectorMode>()
{
    return InspectorModeStrings;
}

// index to enum
template<>
inline FishEditor::InspectorMode ToEnum<FishEditor::InspectorMode>(const int index)
{
    switch (index) {
    case 0: return FishEditor::InspectorMode::Normal; break;
	case 1: return FishEditor::InspectorMode::Debug; break;
	case 2: return FishEditor::InspectorMode::DebugInternal; break;
	
    default: abort(); break;
    }
}

// enum to index
template<>
inline int EnumToIndex<FishEditor::InspectorMode>(FishEditor::InspectorMode e)
{
    switch (e) {
    case FishEditor::InspectorMode::Normal: return 0; break;
	case FishEditor::InspectorMode::Debug: return 1; break;
	case FishEditor::InspectorMode::DebugInternal: return 2; break;
	
    default: abort(); break;
    }
}

// string to enum
template<>
inline FishEditor::InspectorMode ToEnum<FishEditor::InspectorMode>(const std::string& s)
{
    if (s == "Normal") return FishEditor::InspectorMode::Normal;
	if (s == "Debug") return FishEditor::InspectorMode::Debug;
	if (s == "DebugInternal") return FishEditor::InspectorMode::DebugInternal;
	
    abort();
}


} // namespace FishEngine
