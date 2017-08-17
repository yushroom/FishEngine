#pragma once

#include <ReflectEnum.hpp>
#include <Input.hpp>

namespace FishEngine
{


/**************************************************
* FishEngine::KeyState
**************************************************/

// enum count
template<>
constexpr int EnumCount<FishEngine::KeyState>() { return 4; }

// string array
static const char* KeyStateStrings[] =
{
    "Up",
	"Down",
	"Held",
	"None"
};

// cstring array
template<>
inline constexpr const char** EnumToCStringArray<FishEngine::KeyState>()
{
    return KeyStateStrings;
}

// index to enum
template<>
inline FishEngine::KeyState ToEnum<FishEngine::KeyState>(const int index)
{
    switch (index) {
    case 0: return FishEngine::KeyState::Up; break;
	case 1: return FishEngine::KeyState::Down; break;
	case 2: return FishEngine::KeyState::Held; break;
	case 3: return FishEngine::KeyState::None; break;
	
    default: abort(); break;
    }
}

// enum to index
template<>
inline int EnumToIndex<FishEngine::KeyState>(FishEngine::KeyState e)
{
    switch (e) {
    case FishEngine::KeyState::Up: return 0; break;
	case FishEngine::KeyState::Down: return 1; break;
	case FishEngine::KeyState::Held: return 2; break;
	case FishEngine::KeyState::None: return 3; break;
	
    default: abort(); break;
    }
}

// string to enum
template<>
inline FishEngine::KeyState ToEnum<FishEngine::KeyState>(const std::string& s)
{
    if (s == "Up") return FishEngine::KeyState::Up;
	if (s == "Down") return FishEngine::KeyState::Down;
	if (s == "Held") return FishEngine::KeyState::Held;
	if (s == "None") return FishEngine::KeyState::None;
	
    abort();
}


} // namespace FishEngine
