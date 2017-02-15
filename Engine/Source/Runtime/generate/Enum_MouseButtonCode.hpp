#pragma once

#include <ReflectEnum.hpp>
#include <Input.hpp>

namespace FishEngine
{


/**************************************************
* FishEngine::MouseButtonCode
**************************************************/

// enum count
template<>
constexpr int EnumCount<FishEngine::MouseButtonCode>() { return 3; }

// string array
static const char* MouseButtonCodeStrings[] =
{
    "Left",
	"Right",
	"Middle"
};

// cstring array
template<>
inline constexpr const char** EnumToCStringArray<FishEngine::MouseButtonCode>()
{
    return MouseButtonCodeStrings;
}

// index to enum
template<>
inline FishEngine::MouseButtonCode ToEnum<FishEngine::MouseButtonCode>(const int index)
{
    switch (index) {
    case 0: return FishEngine::MouseButtonCode::Left; break;
	case 1: return FishEngine::MouseButtonCode::Right; break;
	case 2: return FishEngine::MouseButtonCode::Middle; break;
	
    default: abort(); break;
    }
}

// enum to index
template<>
inline int EnumToIndex<FishEngine::MouseButtonCode>(FishEngine::MouseButtonCode e)
{
    switch (e) {
    case FishEngine::MouseButtonCode::Left: return 0; break;
	case FishEngine::MouseButtonCode::Right: return 1; break;
	case FishEngine::MouseButtonCode::Middle: return 2; break;
	
    default: abort(); break;
    }
}

// string to enum
template<>
inline FishEngine::MouseButtonCode ToEnum<FishEngine::MouseButtonCode>(const std::string& s)
{
    if (s == "Left") return FishEngine::MouseButtonCode::Left;
	if (s == "Right") return FishEngine::MouseButtonCode::Right;
	if (s == "Middle") return FishEngine::MouseButtonCode::Middle;
	
    abort();
}


} // namespace FishEngine
