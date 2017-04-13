#pragma once

#include <ReflectEnum.hpp>
#include <Debug.hpp>

namespace FishEngine
{


/**************************************************
* FishEngine::LogChannel
**************************************************/

// enum count
template<>
constexpr int EnumCount<FishEngine::LogChannel>() { return 3; }

// string array
static const char* LogChannelStrings[] =
{
    "Info",
	"Warning",
	"Error"
};

// cstring array
template<>
inline constexpr const char** EnumToCStringArray<FishEngine::LogChannel>()
{
    return LogChannelStrings;
}

// index to enum
template<>
inline FishEngine::LogChannel ToEnum<FishEngine::LogChannel>(const int index)
{
    switch (index) {
    case 0: return FishEngine::LogChannel::Info; break;
	case 1: return FishEngine::LogChannel::Warning; break;
	case 2: return FishEngine::LogChannel::Error; break;
	
    default: abort(); break;
    }
}

// enum to index
template<>
inline int EnumToIndex<FishEngine::LogChannel>(FishEngine::LogChannel e)
{
    switch (e) {
    case FishEngine::LogChannel::Info: return 0; break;
	case FishEngine::LogChannel::Warning: return 1; break;
	case FishEngine::LogChannel::Error: return 2; break;
	
    default: abort(); break;
    }
}

// string to enum
template<>
inline FishEngine::LogChannel ToEnum<FishEngine::LogChannel>(const std::string& s)
{
    if (s == "Info") return FishEngine::LogChannel::Info;
	if (s == "Warning") return FishEngine::LogChannel::Warning;
	if (s == "Error") return FishEngine::LogChannel::Error;
	
    abort();
}


} // namespace FishEngine
