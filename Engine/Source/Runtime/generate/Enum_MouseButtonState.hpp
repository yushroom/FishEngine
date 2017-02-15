#pragma once

#include <ReflectEnum.hpp>
#include <Input.hpp>

namespace FishEngine
{


/**************************************************
* FishEngine::MouseButtonState
**************************************************/

// enum count
template<>
constexpr int EnumCount<FishEngine::MouseButtonState>() { return 4; }

// string array
static const char* MouseButtonStateStrings[] =
{
    "None",
	"Down",
	"Held",
	"Up"
};

// cstring array
template<>
inline constexpr const char** EnumToCStringArray<FishEngine::MouseButtonState>()
{
    return MouseButtonStateStrings;
}

// index to enum
template<>
inline FishEngine::MouseButtonState ToEnum<FishEngine::MouseButtonState>(const int index)
{
    switch (index) {
    case 0: return FishEngine::MouseButtonState::None; break;
	case 1: return FishEngine::MouseButtonState::Down; break;
	case 2: return FishEngine::MouseButtonState::Held; break;
	case 3: return FishEngine::MouseButtonState::Up; break;
	
    default: abort(); break;
    }
}

// enum to index
template<>
inline int EnumToIndex<FishEngine::MouseButtonState>(FishEngine::MouseButtonState e)
{
    switch (e) {
    case FishEngine::MouseButtonState::None: return 0; break;
	case FishEngine::MouseButtonState::Down: return 1; break;
	case FishEngine::MouseButtonState::Held: return 2; break;
	case FishEngine::MouseButtonState::Up: return 3; break;
	
    default: abort(); break;
    }
}

// string to enum
template<>
inline FishEngine::MouseButtonState ToEnum<FishEngine::MouseButtonState>(const std::string& s)
{
    if (s == "None") return FishEngine::MouseButtonState::None;
	if (s == "Down") return FishEngine::MouseButtonState::Down;
	if (s == "Held") return FishEngine::MouseButtonState::Held;
	if (s == "Up") return FishEngine::MouseButtonState::Up;
	
    abort();
}


} // namespace FishEngine
