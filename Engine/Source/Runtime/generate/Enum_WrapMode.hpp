#pragma once

#include <ReflectEnum.hpp>
#include <WrapMode.hpp>

namespace FishEngine
{


/**************************************************
* FishEngine::WrapMode
**************************************************/

// enum count
template<>
constexpr int EnumCount<FishEngine::WrapMode>() { return 5; }

// string array
static const char* WrapModeStrings[] =
{
    "Once",
	"Loop",
	"PingPong",
	"Default",
	"ClampForever"
};

// cstring array
template<>
inline constexpr const char** EnumToCStringArray<FishEngine::WrapMode>()
{
    return WrapModeStrings;
}

// index to enum
template<>
inline FishEngine::WrapMode ToEnum<FishEngine::WrapMode>(const int index)
{
    switch (index) {
    case 0: return FishEngine::WrapMode::Once; break;
	case 1: return FishEngine::WrapMode::Loop; break;
	case 2: return FishEngine::WrapMode::PingPong; break;
	case 3: return FishEngine::WrapMode::Default; break;
	case 4: return FishEngine::WrapMode::ClampForever; break;
	
    default: abort(); break;
    }
}

// enum to index
template<>
inline int EnumToIndex<FishEngine::WrapMode>(FishEngine::WrapMode e)
{
    switch (e) {
    case FishEngine::WrapMode::Once: return 0; break;
	case FishEngine::WrapMode::Loop: return 1; break;
	case FishEngine::WrapMode::PingPong: return 2; break;
	case FishEngine::WrapMode::Default: return 3; break;
	case FishEngine::WrapMode::ClampForever: return 4; break;
	
    default: abort(); break;
    }
}

// string to enum
template<>
inline FishEngine::WrapMode ToEnum<FishEngine::WrapMode>(const std::string& s)
{
    if (s == "Once") return FishEngine::WrapMode::Once;
	if (s == "Loop") return FishEngine::WrapMode::Loop;
	if (s == "PingPong") return FishEngine::WrapMode::PingPong;
	if (s == "Default") return FishEngine::WrapMode::Default;
	if (s == "ClampForever") return FishEngine::WrapMode::ClampForever;
	
    abort();
}


} // namespace FishEngine
