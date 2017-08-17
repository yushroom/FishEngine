#pragma once

#include "../ReflectEnum.hpp"
#include "../ShaderProperty.hpp"

namespace FishEngine
{


/**************************************************
* FishEngine::Cullface
**************************************************/

// enum count
template<>
constexpr int EnumCount<FishEngine::Cullface>() { return 3; }

// string array
static const char* CullfaceStrings[] =
{
    "Back",
	"Front",
	"Off"
};

// cstring array
template<>
inline constexpr const char** EnumToCStringArray<FishEngine::Cullface>()
{
    return CullfaceStrings;
}

// index to enum
template<>
inline FishEngine::Cullface ToEnum<FishEngine::Cullface>(const int index)
{
    switch (index) {
    case 0: return FishEngine::Cullface::Back; break;
	case 1: return FishEngine::Cullface::Front; break;
	case 2: return FishEngine::Cullface::Off; break;
	
    default: abort(); break;
    }
}

// enum to index
template<>
inline int EnumToIndex<FishEngine::Cullface>(FishEngine::Cullface e)
{
    switch (e) {
    case FishEngine::Cullface::Back: return 0; break;
	case FishEngine::Cullface::Front: return 1; break;
	case FishEngine::Cullface::Off: return 2; break;
	
    default: abort(); break;
    }
}

// string to enum
template<>
inline FishEngine::Cullface ToEnum<FishEngine::Cullface>(const std::string& s)
{
    if (s == "Back") return FishEngine::Cullface::Back;
	if (s == "Front") return FishEngine::Cullface::Front;
	if (s == "Off") return FishEngine::Cullface::Off;
	
    abort();
}


} // namespace FishEngine
