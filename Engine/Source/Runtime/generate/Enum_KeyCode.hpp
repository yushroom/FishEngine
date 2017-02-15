#pragma once

#include <ReflectEnum.hpp>
#include <Input.hpp>

namespace FishEngine
{


/**************************************************
* FishEngine::KeyCode
**************************************************/

// enum count
template<>
constexpr int EnumCount<FishEngine::KeyCode>() { return 44; }

// string array
static const char* KeyCodeStrings[] =
{
    "Alpha0",
	"Alpha1",
	"Alpha2",
	"Alpha3",
	"Alpha4",
	"Alpha5",
	"Alpha6",
	"Alpha7",
	"Alpha8",
	"Alpha9",
	"A",
	"B",
	"C",
	"D",
	"E",
	"F",
	"G",
	"H",
	"I",
	"J",
	"K",
	"L",
	"M",
	"N",
	"O",
	"P",
	"Q",
	"R",
	"S",
	"T",
	"U",
	"V",
	"W",
	"X",
	"Y",
	"Z",
	"LeftControl",
	"RightControl",
	"LeftShift",
	"RightShift",
	"LeftAlt",
	"RightAlt",
	"LeftSuper",
	"RightSuper"
};

// cstring array
template<>
inline constexpr const char** EnumToCStringArray<FishEngine::KeyCode>()
{
    return KeyCodeStrings;
}

// index to enum
template<>
inline FishEngine::KeyCode ToEnum<FishEngine::KeyCode>(const int index)
{
    switch (index) {
    case 0: return FishEngine::KeyCode::Alpha0; break;
	case 1: return FishEngine::KeyCode::Alpha1; break;
	case 2: return FishEngine::KeyCode::Alpha2; break;
	case 3: return FishEngine::KeyCode::Alpha3; break;
	case 4: return FishEngine::KeyCode::Alpha4; break;
	case 5: return FishEngine::KeyCode::Alpha5; break;
	case 6: return FishEngine::KeyCode::Alpha6; break;
	case 7: return FishEngine::KeyCode::Alpha7; break;
	case 8: return FishEngine::KeyCode::Alpha8; break;
	case 9: return FishEngine::KeyCode::Alpha9; break;
	case 10: return FishEngine::KeyCode::A; break;
	case 11: return FishEngine::KeyCode::B; break;
	case 12: return FishEngine::KeyCode::C; break;
	case 13: return FishEngine::KeyCode::D; break;
	case 14: return FishEngine::KeyCode::E; break;
	case 15: return FishEngine::KeyCode::F; break;
	case 16: return FishEngine::KeyCode::G; break;
	case 17: return FishEngine::KeyCode::H; break;
	case 18: return FishEngine::KeyCode::I; break;
	case 19: return FishEngine::KeyCode::J; break;
	case 20: return FishEngine::KeyCode::K; break;
	case 21: return FishEngine::KeyCode::L; break;
	case 22: return FishEngine::KeyCode::M; break;
	case 23: return FishEngine::KeyCode::N; break;
	case 24: return FishEngine::KeyCode::O; break;
	case 25: return FishEngine::KeyCode::P; break;
	case 26: return FishEngine::KeyCode::Q; break;
	case 27: return FishEngine::KeyCode::R; break;
	case 28: return FishEngine::KeyCode::S; break;
	case 29: return FishEngine::KeyCode::T; break;
	case 30: return FishEngine::KeyCode::U; break;
	case 31: return FishEngine::KeyCode::V; break;
	case 32: return FishEngine::KeyCode::W; break;
	case 33: return FishEngine::KeyCode::X; break;
	case 34: return FishEngine::KeyCode::Y; break;
	case 35: return FishEngine::KeyCode::Z; break;
	case 36: return FishEngine::KeyCode::LeftControl; break;
	case 37: return FishEngine::KeyCode::RightControl; break;
	case 38: return FishEngine::KeyCode::LeftShift; break;
	case 39: return FishEngine::KeyCode::RightShift; break;
	case 40: return FishEngine::KeyCode::LeftAlt; break;
	case 41: return FishEngine::KeyCode::RightAlt; break;
	case 42: return FishEngine::KeyCode::LeftSuper; break;
	case 43: return FishEngine::KeyCode::RightSuper; break;
	
    default: abort(); break;
    }
}

// enum to index
template<>
inline int EnumToIndex<FishEngine::KeyCode>(FishEngine::KeyCode e)
{
    switch (e) {
    case FishEngine::KeyCode::Alpha0: return 0; break;
	case FishEngine::KeyCode::Alpha1: return 1; break;
	case FishEngine::KeyCode::Alpha2: return 2; break;
	case FishEngine::KeyCode::Alpha3: return 3; break;
	case FishEngine::KeyCode::Alpha4: return 4; break;
	case FishEngine::KeyCode::Alpha5: return 5; break;
	case FishEngine::KeyCode::Alpha6: return 6; break;
	case FishEngine::KeyCode::Alpha7: return 7; break;
	case FishEngine::KeyCode::Alpha8: return 8; break;
	case FishEngine::KeyCode::Alpha9: return 9; break;
	case FishEngine::KeyCode::A: return 10; break;
	case FishEngine::KeyCode::B: return 11; break;
	case FishEngine::KeyCode::C: return 12; break;
	case FishEngine::KeyCode::D: return 13; break;
	case FishEngine::KeyCode::E: return 14; break;
	case FishEngine::KeyCode::F: return 15; break;
	case FishEngine::KeyCode::G: return 16; break;
	case FishEngine::KeyCode::H: return 17; break;
	case FishEngine::KeyCode::I: return 18; break;
	case FishEngine::KeyCode::J: return 19; break;
	case FishEngine::KeyCode::K: return 20; break;
	case FishEngine::KeyCode::L: return 21; break;
	case FishEngine::KeyCode::M: return 22; break;
	case FishEngine::KeyCode::N: return 23; break;
	case FishEngine::KeyCode::O: return 24; break;
	case FishEngine::KeyCode::P: return 25; break;
	case FishEngine::KeyCode::Q: return 26; break;
	case FishEngine::KeyCode::R: return 27; break;
	case FishEngine::KeyCode::S: return 28; break;
	case FishEngine::KeyCode::T: return 29; break;
	case FishEngine::KeyCode::U: return 30; break;
	case FishEngine::KeyCode::V: return 31; break;
	case FishEngine::KeyCode::W: return 32; break;
	case FishEngine::KeyCode::X: return 33; break;
	case FishEngine::KeyCode::Y: return 34; break;
	case FishEngine::KeyCode::Z: return 35; break;
	case FishEngine::KeyCode::LeftControl: return 36; break;
	case FishEngine::KeyCode::RightControl: return 37; break;
	case FishEngine::KeyCode::LeftShift: return 38; break;
	case FishEngine::KeyCode::RightShift: return 39; break;
	case FishEngine::KeyCode::LeftAlt: return 40; break;
	case FishEngine::KeyCode::RightAlt: return 41; break;
	case FishEngine::KeyCode::LeftSuper: return 42; break;
	case FishEngine::KeyCode::RightSuper: return 43; break;
	
    default: abort(); break;
    }
}

// string to enum
template<>
inline FishEngine::KeyCode ToEnum<FishEngine::KeyCode>(const std::string& s)
{
    if (s == "Alpha0") return FishEngine::KeyCode::Alpha0;
	if (s == "Alpha1") return FishEngine::KeyCode::Alpha1;
	if (s == "Alpha2") return FishEngine::KeyCode::Alpha2;
	if (s == "Alpha3") return FishEngine::KeyCode::Alpha3;
	if (s == "Alpha4") return FishEngine::KeyCode::Alpha4;
	if (s == "Alpha5") return FishEngine::KeyCode::Alpha5;
	if (s == "Alpha6") return FishEngine::KeyCode::Alpha6;
	if (s == "Alpha7") return FishEngine::KeyCode::Alpha7;
	if (s == "Alpha8") return FishEngine::KeyCode::Alpha8;
	if (s == "Alpha9") return FishEngine::KeyCode::Alpha9;
	if (s == "A") return FishEngine::KeyCode::A;
	if (s == "B") return FishEngine::KeyCode::B;
	if (s == "C") return FishEngine::KeyCode::C;
	if (s == "D") return FishEngine::KeyCode::D;
	if (s == "E") return FishEngine::KeyCode::E;
	if (s == "F") return FishEngine::KeyCode::F;
	if (s == "G") return FishEngine::KeyCode::G;
	if (s == "H") return FishEngine::KeyCode::H;
	if (s == "I") return FishEngine::KeyCode::I;
	if (s == "J") return FishEngine::KeyCode::J;
	if (s == "K") return FishEngine::KeyCode::K;
	if (s == "L") return FishEngine::KeyCode::L;
	if (s == "M") return FishEngine::KeyCode::M;
	if (s == "N") return FishEngine::KeyCode::N;
	if (s == "O") return FishEngine::KeyCode::O;
	if (s == "P") return FishEngine::KeyCode::P;
	if (s == "Q") return FishEngine::KeyCode::Q;
	if (s == "R") return FishEngine::KeyCode::R;
	if (s == "S") return FishEngine::KeyCode::S;
	if (s == "T") return FishEngine::KeyCode::T;
	if (s == "U") return FishEngine::KeyCode::U;
	if (s == "V") return FishEngine::KeyCode::V;
	if (s == "W") return FishEngine::KeyCode::W;
	if (s == "X") return FishEngine::KeyCode::X;
	if (s == "Y") return FishEngine::KeyCode::Y;
	if (s == "Z") return FishEngine::KeyCode::Z;
	if (s == "LeftControl") return FishEngine::KeyCode::LeftControl;
	if (s == "RightControl") return FishEngine::KeyCode::RightControl;
	if (s == "LeftShift") return FishEngine::KeyCode::LeftShift;
	if (s == "RightShift") return FishEngine::KeyCode::RightShift;
	if (s == "LeftAlt") return FishEngine::KeyCode::LeftAlt;
	if (s == "RightAlt") return FishEngine::KeyCode::RightAlt;
	if (s == "LeftSuper") return FishEngine::KeyCode::LeftSuper;
	if (s == "RightSuper") return FishEngine::KeyCode::RightSuper;
	
    abort();
}


} // namespace FishEngine
