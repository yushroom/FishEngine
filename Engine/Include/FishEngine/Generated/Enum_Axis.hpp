#pragma once

#include <ReflectEnum.hpp>
#include <Input.hpp>

namespace FishEngine
{


/**************************************************
* FishEngine::Axis
**************************************************/

// enum count
template<>
constexpr int EnumCount<FishEngine::Axis>() { return 12; }

// string array
static const char* AxisStrings[] =
{
    "Vertical",
	"Horizontal",
	"Fire1",
	"Fire2",
	"Fire3",
	"Jump",
	"MouseX",
	"MouseY",
	"MouseScrollWheel",
	"WindowShakeX",
	"WindwoShakeY",
	"AxisCount"
};

// cstring array
template<>
inline constexpr const char** EnumToCStringArray<FishEngine::Axis>()
{
    return AxisStrings;
}

// index to enum
template<>
inline FishEngine::Axis ToEnum<FishEngine::Axis>(const int index)
{
    switch (index) {
    case 0: return FishEngine::Axis::Vertical; break;
	case 1: return FishEngine::Axis::Horizontal; break;
	case 2: return FishEngine::Axis::Fire1; break;
	case 3: return FishEngine::Axis::Fire2; break;
	case 4: return FishEngine::Axis::Fire3; break;
	case 5: return FishEngine::Axis::Jump; break;
	case 6: return FishEngine::Axis::MouseX; break;
	case 7: return FishEngine::Axis::MouseY; break;
	case 8: return FishEngine::Axis::MouseScrollWheel; break;
	case 9: return FishEngine::Axis::WindowShakeX; break;
	case 10: return FishEngine::Axis::WindwoShakeY; break;
	case 11: return FishEngine::Axis::AxisCount; break;
	
    default: abort(); break;
    }
}

// enum to index
template<>
inline int EnumToIndex<FishEngine::Axis>(FishEngine::Axis e)
{
    switch (e) {
    case FishEngine::Axis::Vertical: return 0; break;
	case FishEngine::Axis::Horizontal: return 1; break;
	case FishEngine::Axis::Fire1: return 2; break;
	case FishEngine::Axis::Fire2: return 3; break;
	case FishEngine::Axis::Fire3: return 4; break;
	case FishEngine::Axis::Jump: return 5; break;
	case FishEngine::Axis::MouseX: return 6; break;
	case FishEngine::Axis::MouseY: return 7; break;
	case FishEngine::Axis::MouseScrollWheel: return 8; break;
	case FishEngine::Axis::WindowShakeX: return 9; break;
	case FishEngine::Axis::WindwoShakeY: return 10; break;
	case FishEngine::Axis::AxisCount: return 11; break;
	
    default: abort(); break;
    }
}

// string to enum
template<>
inline FishEngine::Axis ToEnum<FishEngine::Axis>(const std::string& s)
{
    if (s == "Vertical") return FishEngine::Axis::Vertical;
	if (s == "Horizontal") return FishEngine::Axis::Horizontal;
	if (s == "Fire1") return FishEngine::Axis::Fire1;
	if (s == "Fire2") return FishEngine::Axis::Fire2;
	if (s == "Fire3") return FishEngine::Axis::Fire3;
	if (s == "Jump") return FishEngine::Axis::Jump;
	if (s == "MouseX") return FishEngine::Axis::MouseX;
	if (s == "MouseY") return FishEngine::Axis::MouseY;
	if (s == "MouseScrollWheel") return FishEngine::Axis::MouseScrollWheel;
	if (s == "WindowShakeX") return FishEngine::Axis::WindowShakeX;
	if (s == "WindwoShakeY") return FishEngine::Axis::WindwoShakeY;
	if (s == "AxisCount") return FishEngine::Axis::AxisCount;
	
    abort();
}


} // namespace FishEngine
