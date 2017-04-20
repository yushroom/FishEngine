#pragma once

#include <ReflectEnum.hpp>
#include <AudioDataLoadState.hpp>

namespace FishEngine
{


/**************************************************
* FishEngine::AudioDataLoadState
**************************************************/

// enum count
template<>
constexpr int EnumCount<FishEngine::AudioDataLoadState>() { return 4; }

// string array
static const char* AudioDataLoadStateStrings[] =
{
    "Unloaded",
	"Loading",
	"Loaded",
	"Failed"
};

// cstring array
template<>
inline constexpr const char** EnumToCStringArray<FishEngine::AudioDataLoadState>()
{
    return AudioDataLoadStateStrings;
}

// index to enum
template<>
inline FishEngine::AudioDataLoadState ToEnum<FishEngine::AudioDataLoadState>(const int index)
{
    switch (index) {
    case 0: return FishEngine::AudioDataLoadState::Unloaded; break;
	case 1: return FishEngine::AudioDataLoadState::Loading; break;
	case 2: return FishEngine::AudioDataLoadState::Loaded; break;
	case 3: return FishEngine::AudioDataLoadState::Failed; break;
	
    default: abort(); break;
    }
}

// enum to index
template<>
inline int EnumToIndex<FishEngine::AudioDataLoadState>(FishEngine::AudioDataLoadState e)
{
    switch (e) {
    case FishEngine::AudioDataLoadState::Unloaded: return 0; break;
	case FishEngine::AudioDataLoadState::Loading: return 1; break;
	case FishEngine::AudioDataLoadState::Loaded: return 2; break;
	case FishEngine::AudioDataLoadState::Failed: return 3; break;
	
    default: abort(); break;
    }
}

// string to enum
template<>
inline FishEngine::AudioDataLoadState ToEnum<FishEngine::AudioDataLoadState>(const std::string& s)
{
    if (s == "Unloaded") return FishEngine::AudioDataLoadState::Unloaded;
	if (s == "Loading") return FishEngine::AudioDataLoadState::Loading;
	if (s == "Loaded") return FishEngine::AudioDataLoadState::Loaded;
	if (s == "Failed") return FishEngine::AudioDataLoadState::Failed;
	
    abort();
}


} // namespace FishEngine
