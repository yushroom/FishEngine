#pragma once

#include <ReflectEnum.hpp>
#include <ShaderProperty.hpp>

namespace FishEngine
{


/**************************************************
* FishEngine::ZWrite
**************************************************/

// enum count
template<>
constexpr int EnumCount<FishEngine::ZWrite>() { return 2; }

// string array
static const char* ZWriteStrings[] =
{
    "On",
	"Off"
};

// cstring array
template<>
inline constexpr const char** EnumToCStringArray<FishEngine::ZWrite>()
{
    return ZWriteStrings;
}

// index to enum
template<>
inline FishEngine::ZWrite ToEnum<FishEngine::ZWrite>(const int index)
{
    switch (index) {
    case 0: return FishEngine::ZWrite::On; break;
	case 1: return FishEngine::ZWrite::Off; break;
	
    default: abort(); break;
    }
}

// enum to index
template<>
inline int EnumToIndex<FishEngine::ZWrite>(FishEngine::ZWrite e)
{
    switch (e) {
    case FishEngine::ZWrite::On: return 0; break;
	case FishEngine::ZWrite::Off: return 1; break;
	
    default: abort(); break;
    }
}

// string to enum
template<>
inline FishEngine::ZWrite ToEnum<FishEngine::ZWrite>(const std::string& s)
{
    if (s == "On") return FishEngine::ZWrite::On;
	if (s == "Off") return FishEngine::ZWrite::Off;
	
    abort();
}


} // namespace FishEngine
