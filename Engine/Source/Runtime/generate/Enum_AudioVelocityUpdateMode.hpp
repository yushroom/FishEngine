#pragma once

#include <ReflectEnum.hpp>
#include <AudioVelocityUpdateMode.hpp>

namespace FishEngine
{


/**************************************************
* FishEngine::AudioVelocityUpdateMode
**************************************************/

// enum count
template<>
constexpr int EnumCount<FishEngine::AudioVelocityUpdateMode>() { return 3; }

// string array
static const char* AudioVelocityUpdateModeStrings[] =
{
    "Auto",
	"Fixed",
	"Dynamic"
};

// cstring array
template<>
inline constexpr const char** EnumToCStringArray<FishEngine::AudioVelocityUpdateMode>()
{
    return AudioVelocityUpdateModeStrings;
}

// index to enum
template<>
inline FishEngine::AudioVelocityUpdateMode ToEnum<FishEngine::AudioVelocityUpdateMode>(const int index)
{
    switch (index) {
    case 0: return FishEngine::AudioVelocityUpdateMode::Auto; break;
	case 1: return FishEngine::AudioVelocityUpdateMode::Fixed; break;
	case 2: return FishEngine::AudioVelocityUpdateMode::Dynamic; break;
	
    default: abort(); break;
    }
}

// enum to index
template<>
inline int EnumToIndex<FishEngine::AudioVelocityUpdateMode>(FishEngine::AudioVelocityUpdateMode e)
{
    switch (e) {
    case FishEngine::AudioVelocityUpdateMode::Auto: return 0; break;
	case FishEngine::AudioVelocityUpdateMode::Fixed: return 1; break;
	case FishEngine::AudioVelocityUpdateMode::Dynamic: return 2; break;
	
    default: abort(); break;
    }
}

// string to enum
template<>
inline FishEngine::AudioVelocityUpdateMode ToEnum<FishEngine::AudioVelocityUpdateMode>(const std::string& s)
{
    if (s == "Auto") return FishEngine::AudioVelocityUpdateMode::Auto;
	if (s == "Fixed") return FishEngine::AudioVelocityUpdateMode::Fixed;
	if (s == "Dynamic") return FishEngine::AudioVelocityUpdateMode::Dynamic;
	
    abort();
}


} // namespace FishEngine
