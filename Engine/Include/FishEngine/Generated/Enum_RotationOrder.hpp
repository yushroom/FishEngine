#pragma once

#include <ReflectEnum.hpp>
#include <Quaternion.hpp>

namespace FishEngine
{


/**************************************************
* FishEngine::RotationOrder
**************************************************/

// enum count
template<>
constexpr int EnumCount<FishEngine::RotationOrder>() { return 6; }

// string array
static const char* RotationOrderStrings[] =
{
    "ZXY",
	"XYZ",
	"YZX",
	"XZY",
	"YXZ",
	"ZYX"
};

// cstring array
template<>
inline constexpr const char** EnumToCStringArray<FishEngine::RotationOrder>()
{
    return RotationOrderStrings;
}

// index to enum
template<>
inline FishEngine::RotationOrder ToEnum<FishEngine::RotationOrder>(const int index)
{
    switch (index) {
    case 0: return FishEngine::RotationOrder::ZXY; break;
	case 1: return FishEngine::RotationOrder::XYZ; break;
	case 2: return FishEngine::RotationOrder::YZX; break;
	case 3: return FishEngine::RotationOrder::XZY; break;
	case 4: return FishEngine::RotationOrder::YXZ; break;
	case 5: return FishEngine::RotationOrder::ZYX; break;
	
    default: abort(); break;
    }
}

// enum to index
template<>
inline int EnumToIndex<FishEngine::RotationOrder>(FishEngine::RotationOrder e)
{
    switch (e) {
    case FishEngine::RotationOrder::ZXY: return 0; break;
	case FishEngine::RotationOrder::XYZ: return 1; break;
	case FishEngine::RotationOrder::YZX: return 2; break;
	case FishEngine::RotationOrder::XZY: return 3; break;
	case FishEngine::RotationOrder::YXZ: return 4; break;
	case FishEngine::RotationOrder::ZYX: return 5; break;
	
    default: abort(); break;
    }
}

// string to enum
template<>
inline FishEngine::RotationOrder ToEnum<FishEngine::RotationOrder>(const std::string& s)
{
    if (s == "ZXY") return FishEngine::RotationOrder::ZXY;
	if (s == "XYZ") return FishEngine::RotationOrder::XYZ;
	if (s == "YZX") return FishEngine::RotationOrder::YZX;
	if (s == "XZY") return FishEngine::RotationOrder::XZY;
	if (s == "YXZ") return FishEngine::RotationOrder::YXZ;
	if (s == "ZYX") return FishEngine::RotationOrder::ZYX;
	
    abort();
}


} // namespace FishEngine
