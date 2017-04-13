#pragma once

#include <ReflectEnum.hpp>
#include <UIHeaderState.hpp>

namespace FishEngine
{


/**************************************************
* FishEditor::UIHeaderState
**************************************************/

// enum count
template<>
constexpr int EnumCount<FishEditor::UIHeaderState>() { return 3; }

// string array
static const char* UIHeaderStateStrings[] =
{
    "none",
	"enabledChanged",
	"menuButtonClicked"
};

// cstring array
template<>
inline constexpr const char** EnumToCStringArray<FishEditor::UIHeaderState>()
{
    return UIHeaderStateStrings;
}

// index to enum
template<>
inline FishEditor::UIHeaderState ToEnum<FishEditor::UIHeaderState>(const int index)
{
    switch (index) {
    case 0: return FishEditor::UIHeaderState::none; break;
	case 1: return FishEditor::UIHeaderState::enabledChanged; break;
	case 2: return FishEditor::UIHeaderState::menuButtonClicked; break;
	
    default: abort(); break;
    }
}

// enum to index
template<>
inline int EnumToIndex<FishEditor::UIHeaderState>(FishEditor::UIHeaderState e)
{
    switch (e) {
    case FishEditor::UIHeaderState::none: return 0; break;
	case FishEditor::UIHeaderState::enabledChanged: return 1; break;
	case FishEditor::UIHeaderState::menuButtonClicked: return 2; break;
	
    default: abort(); break;
    }
}

// string to enum
template<>
inline FishEditor::UIHeaderState ToEnum<FishEditor::UIHeaderState>(const std::string& s)
{
    if (s == "none") return FishEditor::UIHeaderState::none;
	if (s == "enabledChanged") return FishEditor::UIHeaderState::enabledChanged;
	if (s == "menuButtonClicked") return FishEditor::UIHeaderState::menuButtonClicked;
	
    abort();
}


} // namespace FishEngine
