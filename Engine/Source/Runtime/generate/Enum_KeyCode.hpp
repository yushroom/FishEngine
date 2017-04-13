#pragma once

#include <ReflectEnum.hpp>
#include <KeyCode.hpp>

namespace FishEngine
{


/**************************************************
* FishEngine::KeyCode
**************************************************/

// enum count
template<>
constexpr int EnumCount<FishEngine::KeyCode>() { return 321; }

// string array
static const char* KeyCodeStrings[] =
{
    "None",
	"Backspace",
	"Delete",
	"Tab",
	"Clear",
	"Return",
	"Pause",
	"Escape",
	"Space",
	"Keypad0",
	"Keypad1",
	"Keypad2",
	"Keypad3",
	"Keypad4",
	"Keypad5",
	"Keypad6",
	"Keypad7",
	"Keypad8",
	"Keypad9",
	"KeypadPeriod",
	"KeypadDivide",
	"KeypadMultiply",
	"KeypadMinus",
	"KeypadPlus",
	"KeypadEnter",
	"KeypadEquals",
	"UpArrow",
	"DownArrow",
	"RightArrow",
	"LeftArrow",
	"Insert",
	"Home",
	"End",
	"PageUp",
	"PageDown",
	"F1",
	"F2",
	"F3",
	"F4",
	"F5",
	"F6",
	"F7",
	"F8",
	"F9",
	"F10",
	"F11",
	"F12",
	"F13",
	"F14",
	"F15",
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
	"Exclaim",
	"DoubleQuote",
	"Hash",
	"Dollar",
	"Ampersand",
	"Quote",
	"LeftParen",
	"RightParen",
	"Asterisk",
	"Plus",
	"Comma",
	"Minus",
	"Period",
	"Slash",
	"Colon",
	"Semicolon",
	"Less",
	"Equals",
	"Greater",
	"Question",
	"At",
	"LeftBracket",
	"Backslash",
	"RightBracket",
	"Caret",
	"Underscore",
	"BackQuote",
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
	"Numlock",
	"CapsLock",
	"ScrollLock",
	"RightShift",
	"LeftShift",
	"RightControl",
	"LeftControl",
	"RightAlt",
	"LeftAlt",
	"LeftCommand",
	"LeftApple",
	"LeftWindows",
	"RightCommand",
	"RightApple",
	"RightWindows",
	"AltGr",
	"Help",
	"Print",
	"SysReq",
	"Break",
	"Menu",
	"Mouse0",
	"Mouse1",
	"Mouse2",
	"Mouse3",
	"Mouse4",
	"Mouse5",
	"Mouse6",
	"JoystickButton0",
	"JoystickButton1",
	"JoystickButton2",
	"JoystickButton3",
	"JoystickButton4",
	"JoystickButton5",
	"JoystickButton6",
	"JoystickButton7",
	"JoystickButton8",
	"JoystickButton9",
	"JoystickButton10",
	"JoystickButton11",
	"JoystickButton12",
	"JoystickButton13",
	"JoystickButton14",
	"JoystickButton15",
	"JoystickButton16",
	"JoystickButton17",
	"JoystickButton18",
	"JoystickButton19",
	"Joystick1Button0",
	"Joystick1Button1",
	"Joystick1Button2",
	"Joystick1Button3",
	"Joystick1Button4",
	"Joystick1Button5",
	"Joystick1Button6",
	"Joystick1Button7",
	"Joystick1Button8",
	"Joystick1Button9",
	"Joystick1Button10",
	"Joystick1Button11",
	"Joystick1Button12",
	"Joystick1Button13",
	"Joystick1Button14",
	"Joystick1Button15",
	"Joystick1Button16",
	"Joystick1Button17",
	"Joystick1Button18",
	"Joystick1Button19",
	"Joystick2Button0",
	"Joystick2Button1",
	"Joystick2Button2",
	"Joystick2Button3",
	"Joystick2Button4",
	"Joystick2Button5",
	"Joystick2Button6",
	"Joystick2Button7",
	"Joystick2Button8",
	"Joystick2Button9",
	"Joystick2Button10",
	"Joystick2Button11",
	"Joystick2Button12",
	"Joystick2Button13",
	"Joystick2Button14",
	"Joystick2Button15",
	"Joystick2Button16",
	"Joystick2Button17",
	"Joystick2Button18",
	"Joystick2Button19",
	"Joystick3Button0",
	"Joystick3Button1",
	"Joystick3Button2",
	"Joystick3Button3",
	"Joystick3Button4",
	"Joystick3Button5",
	"Joystick3Button6",
	"Joystick3Button7",
	"Joystick3Button8",
	"Joystick3Button9",
	"Joystick3Button10",
	"Joystick3Button11",
	"Joystick3Button12",
	"Joystick3Button13",
	"Joystick3Button14",
	"Joystick3Button15",
	"Joystick3Button16",
	"Joystick3Button17",
	"Joystick3Button18",
	"Joystick3Button19",
	"Joystick4Button0",
	"Joystick4Button1",
	"Joystick4Button2",
	"Joystick4Button3",
	"Joystick4Button4",
	"Joystick4Button5",
	"Joystick4Button6",
	"Joystick4Button7",
	"Joystick4Button8",
	"Joystick4Button9",
	"Joystick4Button10",
	"Joystick4Button11",
	"Joystick4Button12",
	"Joystick4Button13",
	"Joystick4Button14",
	"Joystick4Button15",
	"Joystick4Button16",
	"Joystick4Button17",
	"Joystick4Button18",
	"Joystick4Button19",
	"Joystick5Button0",
	"Joystick5Button1",
	"Joystick5Button2",
	"Joystick5Button3",
	"Joystick5Button4",
	"Joystick5Button5",
	"Joystick5Button6",
	"Joystick5Button7",
	"Joystick5Button8",
	"Joystick5Button9",
	"Joystick5Button10",
	"Joystick5Button11",
	"Joystick5Button12",
	"Joystick5Button13",
	"Joystick5Button14",
	"Joystick5Button15",
	"Joystick5Button16",
	"Joystick5Button17",
	"Joystick5Button18",
	"Joystick5Button19",
	"Joystick6Button0",
	"Joystick6Button1",
	"Joystick6Button2",
	"Joystick6Button3",
	"Joystick6Button4",
	"Joystick6Button5",
	"Joystick6Button6",
	"Joystick6Button7",
	"Joystick6Button8",
	"Joystick6Button9",
	"Joystick6Button10",
	"Joystick6Button11",
	"Joystick6Button12",
	"Joystick6Button13",
	"Joystick6Button14",
	"Joystick6Button15",
	"Joystick6Button16",
	"Joystick6Button17",
	"Joystick6Button18",
	"Joystick6Button19",
	"Joystick7Button0",
	"Joystick7Button1",
	"Joystick7Button2",
	"Joystick7Button3",
	"Joystick7Button4",
	"Joystick7Button5",
	"Joystick7Button6",
	"Joystick7Button7",
	"Joystick7Button8",
	"Joystick7Button9",
	"Joystick7Button10",
	"Joystick7Button11",
	"Joystick7Button12",
	"Joystick7Button13",
	"Joystick7Button14",
	"Joystick7Button15",
	"Joystick7Button16",
	"Joystick7Button17",
	"Joystick7Button18",
	"Joystick7Button19",
	"Joystick8Button0",
	"Joystick8Button1",
	"Joystick8Button2",
	"Joystick8Button3",
	"Joystick8Button4",
	"Joystick8Button5",
	"Joystick8Button6",
	"Joystick8Button7",
	"Joystick8Button8",
	"Joystick8Button9",
	"Joystick8Button10",
	"Joystick8Button11",
	"Joystick8Button12",
	"Joystick8Button13",
	"Joystick8Button14",
	"Joystick8Button15",
	"Joystick8Button16",
	"Joystick8Button17",
	"Joystick8Button18",
	"Joystick8Button19"
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
    case 0: return FishEngine::KeyCode::None; break;
	case 1: return FishEngine::KeyCode::Backspace; break;
	case 2: return FishEngine::KeyCode::Delete; break;
	case 3: return FishEngine::KeyCode::Tab; break;
	case 4: return FishEngine::KeyCode::Clear; break;
	case 5: return FishEngine::KeyCode::Return; break;
	case 6: return FishEngine::KeyCode::Pause; break;
	case 7: return FishEngine::KeyCode::Escape; break;
	case 8: return FishEngine::KeyCode::Space; break;
	case 9: return FishEngine::KeyCode::Keypad0; break;
	case 10: return FishEngine::KeyCode::Keypad1; break;
	case 11: return FishEngine::KeyCode::Keypad2; break;
	case 12: return FishEngine::KeyCode::Keypad3; break;
	case 13: return FishEngine::KeyCode::Keypad4; break;
	case 14: return FishEngine::KeyCode::Keypad5; break;
	case 15: return FishEngine::KeyCode::Keypad6; break;
	case 16: return FishEngine::KeyCode::Keypad7; break;
	case 17: return FishEngine::KeyCode::Keypad8; break;
	case 18: return FishEngine::KeyCode::Keypad9; break;
	case 19: return FishEngine::KeyCode::KeypadPeriod; break;
	case 20: return FishEngine::KeyCode::KeypadDivide; break;
	case 21: return FishEngine::KeyCode::KeypadMultiply; break;
	case 22: return FishEngine::KeyCode::KeypadMinus; break;
	case 23: return FishEngine::KeyCode::KeypadPlus; break;
	case 24: return FishEngine::KeyCode::KeypadEnter; break;
	case 25: return FishEngine::KeyCode::KeypadEquals; break;
	case 26: return FishEngine::KeyCode::UpArrow; break;
	case 27: return FishEngine::KeyCode::DownArrow; break;
	case 28: return FishEngine::KeyCode::RightArrow; break;
	case 29: return FishEngine::KeyCode::LeftArrow; break;
	case 30: return FishEngine::KeyCode::Insert; break;
	case 31: return FishEngine::KeyCode::Home; break;
	case 32: return FishEngine::KeyCode::End; break;
	case 33: return FishEngine::KeyCode::PageUp; break;
	case 34: return FishEngine::KeyCode::PageDown; break;
	case 35: return FishEngine::KeyCode::F1; break;
	case 36: return FishEngine::KeyCode::F2; break;
	case 37: return FishEngine::KeyCode::F3; break;
	case 38: return FishEngine::KeyCode::F4; break;
	case 39: return FishEngine::KeyCode::F5; break;
	case 40: return FishEngine::KeyCode::F6; break;
	case 41: return FishEngine::KeyCode::F7; break;
	case 42: return FishEngine::KeyCode::F8; break;
	case 43: return FishEngine::KeyCode::F9; break;
	case 44: return FishEngine::KeyCode::F10; break;
	case 45: return FishEngine::KeyCode::F11; break;
	case 46: return FishEngine::KeyCode::F12; break;
	case 47: return FishEngine::KeyCode::F13; break;
	case 48: return FishEngine::KeyCode::F14; break;
	case 49: return FishEngine::KeyCode::F15; break;
	case 50: return FishEngine::KeyCode::Alpha0; break;
	case 51: return FishEngine::KeyCode::Alpha1; break;
	case 52: return FishEngine::KeyCode::Alpha2; break;
	case 53: return FishEngine::KeyCode::Alpha3; break;
	case 54: return FishEngine::KeyCode::Alpha4; break;
	case 55: return FishEngine::KeyCode::Alpha5; break;
	case 56: return FishEngine::KeyCode::Alpha6; break;
	case 57: return FishEngine::KeyCode::Alpha7; break;
	case 58: return FishEngine::KeyCode::Alpha8; break;
	case 59: return FishEngine::KeyCode::Alpha9; break;
	case 60: return FishEngine::KeyCode::Exclaim; break;
	case 61: return FishEngine::KeyCode::DoubleQuote; break;
	case 62: return FishEngine::KeyCode::Hash; break;
	case 63: return FishEngine::KeyCode::Dollar; break;
	case 64: return FishEngine::KeyCode::Ampersand; break;
	case 65: return FishEngine::KeyCode::Quote; break;
	case 66: return FishEngine::KeyCode::LeftParen; break;
	case 67: return FishEngine::KeyCode::RightParen; break;
	case 68: return FishEngine::KeyCode::Asterisk; break;
	case 69: return FishEngine::KeyCode::Plus; break;
	case 70: return FishEngine::KeyCode::Comma; break;
	case 71: return FishEngine::KeyCode::Minus; break;
	case 72: return FishEngine::KeyCode::Period; break;
	case 73: return FishEngine::KeyCode::Slash; break;
	case 74: return FishEngine::KeyCode::Colon; break;
	case 75: return FishEngine::KeyCode::Semicolon; break;
	case 76: return FishEngine::KeyCode::Less; break;
	case 77: return FishEngine::KeyCode::Equals; break;
	case 78: return FishEngine::KeyCode::Greater; break;
	case 79: return FishEngine::KeyCode::Question; break;
	case 80: return FishEngine::KeyCode::At; break;
	case 81: return FishEngine::KeyCode::LeftBracket; break;
	case 82: return FishEngine::KeyCode::Backslash; break;
	case 83: return FishEngine::KeyCode::RightBracket; break;
	case 84: return FishEngine::KeyCode::Caret; break;
	case 85: return FishEngine::KeyCode::Underscore; break;
	case 86: return FishEngine::KeyCode::BackQuote; break;
	case 87: return FishEngine::KeyCode::A; break;
	case 88: return FishEngine::KeyCode::B; break;
	case 89: return FishEngine::KeyCode::C; break;
	case 90: return FishEngine::KeyCode::D; break;
	case 91: return FishEngine::KeyCode::E; break;
	case 92: return FishEngine::KeyCode::F; break;
	case 93: return FishEngine::KeyCode::G; break;
	case 94: return FishEngine::KeyCode::H; break;
	case 95: return FishEngine::KeyCode::I; break;
	case 96: return FishEngine::KeyCode::J; break;
	case 97: return FishEngine::KeyCode::K; break;
	case 98: return FishEngine::KeyCode::L; break;
	case 99: return FishEngine::KeyCode::M; break;
	case 100: return FishEngine::KeyCode::N; break;
	case 101: return FishEngine::KeyCode::O; break;
	case 102: return FishEngine::KeyCode::P; break;
	case 103: return FishEngine::KeyCode::Q; break;
	case 104: return FishEngine::KeyCode::R; break;
	case 105: return FishEngine::KeyCode::S; break;
	case 106: return FishEngine::KeyCode::T; break;
	case 107: return FishEngine::KeyCode::U; break;
	case 108: return FishEngine::KeyCode::V; break;
	case 109: return FishEngine::KeyCode::W; break;
	case 110: return FishEngine::KeyCode::X; break;
	case 111: return FishEngine::KeyCode::Y; break;
	case 112: return FishEngine::KeyCode::Z; break;
	case 113: return FishEngine::KeyCode::Numlock; break;
	case 114: return FishEngine::KeyCode::CapsLock; break;
	case 115: return FishEngine::KeyCode::ScrollLock; break;
	case 116: return FishEngine::KeyCode::RightShift; break;
	case 117: return FishEngine::KeyCode::LeftShift; break;
	case 118: return FishEngine::KeyCode::RightControl; break;
	case 119: return FishEngine::KeyCode::LeftControl; break;
	case 120: return FishEngine::KeyCode::RightAlt; break;
	case 121: return FishEngine::KeyCode::LeftAlt; break;
	case 122: return FishEngine::KeyCode::LeftCommand; break;
	case 123: return FishEngine::KeyCode::LeftApple; break;
	case 124: return FishEngine::KeyCode::LeftWindows; break;
	case 125: return FishEngine::KeyCode::RightCommand; break;
	case 126: return FishEngine::KeyCode::RightApple; break;
	case 127: return FishEngine::KeyCode::RightWindows; break;
	case 128: return FishEngine::KeyCode::AltGr; break;
	case 129: return FishEngine::KeyCode::Help; break;
	case 130: return FishEngine::KeyCode::Print; break;
	case 131: return FishEngine::KeyCode::SysReq; break;
	case 132: return FishEngine::KeyCode::Break; break;
	case 133: return FishEngine::KeyCode::Menu; break;
	case 134: return FishEngine::KeyCode::Mouse0; break;
	case 135: return FishEngine::KeyCode::Mouse1; break;
	case 136: return FishEngine::KeyCode::Mouse2; break;
	case 137: return FishEngine::KeyCode::Mouse3; break;
	case 138: return FishEngine::KeyCode::Mouse4; break;
	case 139: return FishEngine::KeyCode::Mouse5; break;
	case 140: return FishEngine::KeyCode::Mouse6; break;
	case 141: return FishEngine::KeyCode::JoystickButton0; break;
	case 142: return FishEngine::KeyCode::JoystickButton1; break;
	case 143: return FishEngine::KeyCode::JoystickButton2; break;
	case 144: return FishEngine::KeyCode::JoystickButton3; break;
	case 145: return FishEngine::KeyCode::JoystickButton4; break;
	case 146: return FishEngine::KeyCode::JoystickButton5; break;
	case 147: return FishEngine::KeyCode::JoystickButton6; break;
	case 148: return FishEngine::KeyCode::JoystickButton7; break;
	case 149: return FishEngine::KeyCode::JoystickButton8; break;
	case 150: return FishEngine::KeyCode::JoystickButton9; break;
	case 151: return FishEngine::KeyCode::JoystickButton10; break;
	case 152: return FishEngine::KeyCode::JoystickButton11; break;
	case 153: return FishEngine::KeyCode::JoystickButton12; break;
	case 154: return FishEngine::KeyCode::JoystickButton13; break;
	case 155: return FishEngine::KeyCode::JoystickButton14; break;
	case 156: return FishEngine::KeyCode::JoystickButton15; break;
	case 157: return FishEngine::KeyCode::JoystickButton16; break;
	case 158: return FishEngine::KeyCode::JoystickButton17; break;
	case 159: return FishEngine::KeyCode::JoystickButton18; break;
	case 160: return FishEngine::KeyCode::JoystickButton19; break;
	case 161: return FishEngine::KeyCode::Joystick1Button0; break;
	case 162: return FishEngine::KeyCode::Joystick1Button1; break;
	case 163: return FishEngine::KeyCode::Joystick1Button2; break;
	case 164: return FishEngine::KeyCode::Joystick1Button3; break;
	case 165: return FishEngine::KeyCode::Joystick1Button4; break;
	case 166: return FishEngine::KeyCode::Joystick1Button5; break;
	case 167: return FishEngine::KeyCode::Joystick1Button6; break;
	case 168: return FishEngine::KeyCode::Joystick1Button7; break;
	case 169: return FishEngine::KeyCode::Joystick1Button8; break;
	case 170: return FishEngine::KeyCode::Joystick1Button9; break;
	case 171: return FishEngine::KeyCode::Joystick1Button10; break;
	case 172: return FishEngine::KeyCode::Joystick1Button11; break;
	case 173: return FishEngine::KeyCode::Joystick1Button12; break;
	case 174: return FishEngine::KeyCode::Joystick1Button13; break;
	case 175: return FishEngine::KeyCode::Joystick1Button14; break;
	case 176: return FishEngine::KeyCode::Joystick1Button15; break;
	case 177: return FishEngine::KeyCode::Joystick1Button16; break;
	case 178: return FishEngine::KeyCode::Joystick1Button17; break;
	case 179: return FishEngine::KeyCode::Joystick1Button18; break;
	case 180: return FishEngine::KeyCode::Joystick1Button19; break;
	case 181: return FishEngine::KeyCode::Joystick2Button0; break;
	case 182: return FishEngine::KeyCode::Joystick2Button1; break;
	case 183: return FishEngine::KeyCode::Joystick2Button2; break;
	case 184: return FishEngine::KeyCode::Joystick2Button3; break;
	case 185: return FishEngine::KeyCode::Joystick2Button4; break;
	case 186: return FishEngine::KeyCode::Joystick2Button5; break;
	case 187: return FishEngine::KeyCode::Joystick2Button6; break;
	case 188: return FishEngine::KeyCode::Joystick2Button7; break;
	case 189: return FishEngine::KeyCode::Joystick2Button8; break;
	case 190: return FishEngine::KeyCode::Joystick2Button9; break;
	case 191: return FishEngine::KeyCode::Joystick2Button10; break;
	case 192: return FishEngine::KeyCode::Joystick2Button11; break;
	case 193: return FishEngine::KeyCode::Joystick2Button12; break;
	case 194: return FishEngine::KeyCode::Joystick2Button13; break;
	case 195: return FishEngine::KeyCode::Joystick2Button14; break;
	case 196: return FishEngine::KeyCode::Joystick2Button15; break;
	case 197: return FishEngine::KeyCode::Joystick2Button16; break;
	case 198: return FishEngine::KeyCode::Joystick2Button17; break;
	case 199: return FishEngine::KeyCode::Joystick2Button18; break;
	case 200: return FishEngine::KeyCode::Joystick2Button19; break;
	case 201: return FishEngine::KeyCode::Joystick3Button0; break;
	case 202: return FishEngine::KeyCode::Joystick3Button1; break;
	case 203: return FishEngine::KeyCode::Joystick3Button2; break;
	case 204: return FishEngine::KeyCode::Joystick3Button3; break;
	case 205: return FishEngine::KeyCode::Joystick3Button4; break;
	case 206: return FishEngine::KeyCode::Joystick3Button5; break;
	case 207: return FishEngine::KeyCode::Joystick3Button6; break;
	case 208: return FishEngine::KeyCode::Joystick3Button7; break;
	case 209: return FishEngine::KeyCode::Joystick3Button8; break;
	case 210: return FishEngine::KeyCode::Joystick3Button9; break;
	case 211: return FishEngine::KeyCode::Joystick3Button10; break;
	case 212: return FishEngine::KeyCode::Joystick3Button11; break;
	case 213: return FishEngine::KeyCode::Joystick3Button12; break;
	case 214: return FishEngine::KeyCode::Joystick3Button13; break;
	case 215: return FishEngine::KeyCode::Joystick3Button14; break;
	case 216: return FishEngine::KeyCode::Joystick3Button15; break;
	case 217: return FishEngine::KeyCode::Joystick3Button16; break;
	case 218: return FishEngine::KeyCode::Joystick3Button17; break;
	case 219: return FishEngine::KeyCode::Joystick3Button18; break;
	case 220: return FishEngine::KeyCode::Joystick3Button19; break;
	case 221: return FishEngine::KeyCode::Joystick4Button0; break;
	case 222: return FishEngine::KeyCode::Joystick4Button1; break;
	case 223: return FishEngine::KeyCode::Joystick4Button2; break;
	case 224: return FishEngine::KeyCode::Joystick4Button3; break;
	case 225: return FishEngine::KeyCode::Joystick4Button4; break;
	case 226: return FishEngine::KeyCode::Joystick4Button5; break;
	case 227: return FishEngine::KeyCode::Joystick4Button6; break;
	case 228: return FishEngine::KeyCode::Joystick4Button7; break;
	case 229: return FishEngine::KeyCode::Joystick4Button8; break;
	case 230: return FishEngine::KeyCode::Joystick4Button9; break;
	case 231: return FishEngine::KeyCode::Joystick4Button10; break;
	case 232: return FishEngine::KeyCode::Joystick4Button11; break;
	case 233: return FishEngine::KeyCode::Joystick4Button12; break;
	case 234: return FishEngine::KeyCode::Joystick4Button13; break;
	case 235: return FishEngine::KeyCode::Joystick4Button14; break;
	case 236: return FishEngine::KeyCode::Joystick4Button15; break;
	case 237: return FishEngine::KeyCode::Joystick4Button16; break;
	case 238: return FishEngine::KeyCode::Joystick4Button17; break;
	case 239: return FishEngine::KeyCode::Joystick4Button18; break;
	case 240: return FishEngine::KeyCode::Joystick4Button19; break;
	case 241: return FishEngine::KeyCode::Joystick5Button0; break;
	case 242: return FishEngine::KeyCode::Joystick5Button1; break;
	case 243: return FishEngine::KeyCode::Joystick5Button2; break;
	case 244: return FishEngine::KeyCode::Joystick5Button3; break;
	case 245: return FishEngine::KeyCode::Joystick5Button4; break;
	case 246: return FishEngine::KeyCode::Joystick5Button5; break;
	case 247: return FishEngine::KeyCode::Joystick5Button6; break;
	case 248: return FishEngine::KeyCode::Joystick5Button7; break;
	case 249: return FishEngine::KeyCode::Joystick5Button8; break;
	case 250: return FishEngine::KeyCode::Joystick5Button9; break;
	case 251: return FishEngine::KeyCode::Joystick5Button10; break;
	case 252: return FishEngine::KeyCode::Joystick5Button11; break;
	case 253: return FishEngine::KeyCode::Joystick5Button12; break;
	case 254: return FishEngine::KeyCode::Joystick5Button13; break;
	case 255: return FishEngine::KeyCode::Joystick5Button14; break;
	case 256: return FishEngine::KeyCode::Joystick5Button15; break;
	case 257: return FishEngine::KeyCode::Joystick5Button16; break;
	case 258: return FishEngine::KeyCode::Joystick5Button17; break;
	case 259: return FishEngine::KeyCode::Joystick5Button18; break;
	case 260: return FishEngine::KeyCode::Joystick5Button19; break;
	case 261: return FishEngine::KeyCode::Joystick6Button0; break;
	case 262: return FishEngine::KeyCode::Joystick6Button1; break;
	case 263: return FishEngine::KeyCode::Joystick6Button2; break;
	case 264: return FishEngine::KeyCode::Joystick6Button3; break;
	case 265: return FishEngine::KeyCode::Joystick6Button4; break;
	case 266: return FishEngine::KeyCode::Joystick6Button5; break;
	case 267: return FishEngine::KeyCode::Joystick6Button6; break;
	case 268: return FishEngine::KeyCode::Joystick6Button7; break;
	case 269: return FishEngine::KeyCode::Joystick6Button8; break;
	case 270: return FishEngine::KeyCode::Joystick6Button9; break;
	case 271: return FishEngine::KeyCode::Joystick6Button10; break;
	case 272: return FishEngine::KeyCode::Joystick6Button11; break;
	case 273: return FishEngine::KeyCode::Joystick6Button12; break;
	case 274: return FishEngine::KeyCode::Joystick6Button13; break;
	case 275: return FishEngine::KeyCode::Joystick6Button14; break;
	case 276: return FishEngine::KeyCode::Joystick6Button15; break;
	case 277: return FishEngine::KeyCode::Joystick6Button16; break;
	case 278: return FishEngine::KeyCode::Joystick6Button17; break;
	case 279: return FishEngine::KeyCode::Joystick6Button18; break;
	case 280: return FishEngine::KeyCode::Joystick6Button19; break;
	case 281: return FishEngine::KeyCode::Joystick7Button0; break;
	case 282: return FishEngine::KeyCode::Joystick7Button1; break;
	case 283: return FishEngine::KeyCode::Joystick7Button2; break;
	case 284: return FishEngine::KeyCode::Joystick7Button3; break;
	case 285: return FishEngine::KeyCode::Joystick7Button4; break;
	case 286: return FishEngine::KeyCode::Joystick7Button5; break;
	case 287: return FishEngine::KeyCode::Joystick7Button6; break;
	case 288: return FishEngine::KeyCode::Joystick7Button7; break;
	case 289: return FishEngine::KeyCode::Joystick7Button8; break;
	case 290: return FishEngine::KeyCode::Joystick7Button9; break;
	case 291: return FishEngine::KeyCode::Joystick7Button10; break;
	case 292: return FishEngine::KeyCode::Joystick7Button11; break;
	case 293: return FishEngine::KeyCode::Joystick7Button12; break;
	case 294: return FishEngine::KeyCode::Joystick7Button13; break;
	case 295: return FishEngine::KeyCode::Joystick7Button14; break;
	case 296: return FishEngine::KeyCode::Joystick7Button15; break;
	case 297: return FishEngine::KeyCode::Joystick7Button16; break;
	case 298: return FishEngine::KeyCode::Joystick7Button17; break;
	case 299: return FishEngine::KeyCode::Joystick7Button18; break;
	case 300: return FishEngine::KeyCode::Joystick7Button19; break;
	case 301: return FishEngine::KeyCode::Joystick8Button0; break;
	case 302: return FishEngine::KeyCode::Joystick8Button1; break;
	case 303: return FishEngine::KeyCode::Joystick8Button2; break;
	case 304: return FishEngine::KeyCode::Joystick8Button3; break;
	case 305: return FishEngine::KeyCode::Joystick8Button4; break;
	case 306: return FishEngine::KeyCode::Joystick8Button5; break;
	case 307: return FishEngine::KeyCode::Joystick8Button6; break;
	case 308: return FishEngine::KeyCode::Joystick8Button7; break;
	case 309: return FishEngine::KeyCode::Joystick8Button8; break;
	case 310: return FishEngine::KeyCode::Joystick8Button9; break;
	case 311: return FishEngine::KeyCode::Joystick8Button10; break;
	case 312: return FishEngine::KeyCode::Joystick8Button11; break;
	case 313: return FishEngine::KeyCode::Joystick8Button12; break;
	case 314: return FishEngine::KeyCode::Joystick8Button13; break;
	case 315: return FishEngine::KeyCode::Joystick8Button14; break;
	case 316: return FishEngine::KeyCode::Joystick8Button15; break;
	case 317: return FishEngine::KeyCode::Joystick8Button16; break;
	case 318: return FishEngine::KeyCode::Joystick8Button17; break;
	case 319: return FishEngine::KeyCode::Joystick8Button18; break;
	case 320: return FishEngine::KeyCode::Joystick8Button19; break;
	
    default: abort(); break;
    }
}

// enum to index
template<>
inline int EnumToIndex<FishEngine::KeyCode>(FishEngine::KeyCode e)
{
    switch (e) {
    case FishEngine::KeyCode::None: return 0; break;
	case FishEngine::KeyCode::Backspace: return 1; break;
	case FishEngine::KeyCode::Delete: return 2; break;
	case FishEngine::KeyCode::Tab: return 3; break;
	case FishEngine::KeyCode::Clear: return 4; break;
	case FishEngine::KeyCode::Return: return 5; break;
	case FishEngine::KeyCode::Pause: return 6; break;
	case FishEngine::KeyCode::Escape: return 7; break;
	case FishEngine::KeyCode::Space: return 8; break;
	case FishEngine::KeyCode::Keypad0: return 9; break;
	case FishEngine::KeyCode::Keypad1: return 10; break;
	case FishEngine::KeyCode::Keypad2: return 11; break;
	case FishEngine::KeyCode::Keypad3: return 12; break;
	case FishEngine::KeyCode::Keypad4: return 13; break;
	case FishEngine::KeyCode::Keypad5: return 14; break;
	case FishEngine::KeyCode::Keypad6: return 15; break;
	case FishEngine::KeyCode::Keypad7: return 16; break;
	case FishEngine::KeyCode::Keypad8: return 17; break;
	case FishEngine::KeyCode::Keypad9: return 18; break;
	case FishEngine::KeyCode::KeypadPeriod: return 19; break;
	case FishEngine::KeyCode::KeypadDivide: return 20; break;
	case FishEngine::KeyCode::KeypadMultiply: return 21; break;
	case FishEngine::KeyCode::KeypadMinus: return 22; break;
	case FishEngine::KeyCode::KeypadPlus: return 23; break;
	case FishEngine::KeyCode::KeypadEnter: return 24; break;
	case FishEngine::KeyCode::KeypadEquals: return 25; break;
	case FishEngine::KeyCode::UpArrow: return 26; break;
	case FishEngine::KeyCode::DownArrow: return 27; break;
	case FishEngine::KeyCode::RightArrow: return 28; break;
	case FishEngine::KeyCode::LeftArrow: return 29; break;
	case FishEngine::KeyCode::Insert: return 30; break;
	case FishEngine::KeyCode::Home: return 31; break;
	case FishEngine::KeyCode::End: return 32; break;
	case FishEngine::KeyCode::PageUp: return 33; break;
	case FishEngine::KeyCode::PageDown: return 34; break;
	case FishEngine::KeyCode::F1: return 35; break;
	case FishEngine::KeyCode::F2: return 36; break;
	case FishEngine::KeyCode::F3: return 37; break;
	case FishEngine::KeyCode::F4: return 38; break;
	case FishEngine::KeyCode::F5: return 39; break;
	case FishEngine::KeyCode::F6: return 40; break;
	case FishEngine::KeyCode::F7: return 41; break;
	case FishEngine::KeyCode::F8: return 42; break;
	case FishEngine::KeyCode::F9: return 43; break;
	case FishEngine::KeyCode::F10: return 44; break;
	case FishEngine::KeyCode::F11: return 45; break;
	case FishEngine::KeyCode::F12: return 46; break;
	case FishEngine::KeyCode::F13: return 47; break;
	case FishEngine::KeyCode::F14: return 48; break;
	case FishEngine::KeyCode::F15: return 49; break;
	case FishEngine::KeyCode::Alpha0: return 50; break;
	case FishEngine::KeyCode::Alpha1: return 51; break;
	case FishEngine::KeyCode::Alpha2: return 52; break;
	case FishEngine::KeyCode::Alpha3: return 53; break;
	case FishEngine::KeyCode::Alpha4: return 54; break;
	case FishEngine::KeyCode::Alpha5: return 55; break;
	case FishEngine::KeyCode::Alpha6: return 56; break;
	case FishEngine::KeyCode::Alpha7: return 57; break;
	case FishEngine::KeyCode::Alpha8: return 58; break;
	case FishEngine::KeyCode::Alpha9: return 59; break;
	case FishEngine::KeyCode::Exclaim: return 60; break;
	case FishEngine::KeyCode::DoubleQuote: return 61; break;
	case FishEngine::KeyCode::Hash: return 62; break;
	case FishEngine::KeyCode::Dollar: return 63; break;
	case FishEngine::KeyCode::Ampersand: return 64; break;
	case FishEngine::KeyCode::Quote: return 65; break;
	case FishEngine::KeyCode::LeftParen: return 66; break;
	case FishEngine::KeyCode::RightParen: return 67; break;
	case FishEngine::KeyCode::Asterisk: return 68; break;
	case FishEngine::KeyCode::Plus: return 69; break;
	case FishEngine::KeyCode::Comma: return 70; break;
	case FishEngine::KeyCode::Minus: return 71; break;
	case FishEngine::KeyCode::Period: return 72; break;
	case FishEngine::KeyCode::Slash: return 73; break;
	case FishEngine::KeyCode::Colon: return 74; break;
	case FishEngine::KeyCode::Semicolon: return 75; break;
	case FishEngine::KeyCode::Less: return 76; break;
	case FishEngine::KeyCode::Equals: return 77; break;
	case FishEngine::KeyCode::Greater: return 78; break;
	case FishEngine::KeyCode::Question: return 79; break;
	case FishEngine::KeyCode::At: return 80; break;
	case FishEngine::KeyCode::LeftBracket: return 81; break;
	case FishEngine::KeyCode::Backslash: return 82; break;
	case FishEngine::KeyCode::RightBracket: return 83; break;
	case FishEngine::KeyCode::Caret: return 84; break;
	case FishEngine::KeyCode::Underscore: return 85; break;
	case FishEngine::KeyCode::BackQuote: return 86; break;
	case FishEngine::KeyCode::A: return 87; break;
	case FishEngine::KeyCode::B: return 88; break;
	case FishEngine::KeyCode::C: return 89; break;
	case FishEngine::KeyCode::D: return 90; break;
	case FishEngine::KeyCode::E: return 91; break;
	case FishEngine::KeyCode::F: return 92; break;
	case FishEngine::KeyCode::G: return 93; break;
	case FishEngine::KeyCode::H: return 94; break;
	case FishEngine::KeyCode::I: return 95; break;
	case FishEngine::KeyCode::J: return 96; break;
	case FishEngine::KeyCode::K: return 97; break;
	case FishEngine::KeyCode::L: return 98; break;
	case FishEngine::KeyCode::M: return 99; break;
	case FishEngine::KeyCode::N: return 100; break;
	case FishEngine::KeyCode::O: return 101; break;
	case FishEngine::KeyCode::P: return 102; break;
	case FishEngine::KeyCode::Q: return 103; break;
	case FishEngine::KeyCode::R: return 104; break;
	case FishEngine::KeyCode::S: return 105; break;
	case FishEngine::KeyCode::T: return 106; break;
	case FishEngine::KeyCode::U: return 107; break;
	case FishEngine::KeyCode::V: return 108; break;
	case FishEngine::KeyCode::W: return 109; break;
	case FishEngine::KeyCode::X: return 110; break;
	case FishEngine::KeyCode::Y: return 111; break;
	case FishEngine::KeyCode::Z: return 112; break;
	case FishEngine::KeyCode::Numlock: return 113; break;
	case FishEngine::KeyCode::CapsLock: return 114; break;
	case FishEngine::KeyCode::ScrollLock: return 115; break;
	case FishEngine::KeyCode::RightShift: return 116; break;
	case FishEngine::KeyCode::LeftShift: return 117; break;
	case FishEngine::KeyCode::RightControl: return 118; break;
	case FishEngine::KeyCode::LeftControl: return 119; break;
	case FishEngine::KeyCode::RightAlt: return 120; break;
	case FishEngine::KeyCode::LeftAlt: return 121; break;
	case FishEngine::KeyCode::LeftCommand: return 122; break;
	case FishEngine::KeyCode::LeftApple: return 123; break;
	case FishEngine::KeyCode::LeftWindows: return 124; break;
	case FishEngine::KeyCode::RightCommand: return 125; break;
	case FishEngine::KeyCode::RightApple: return 126; break;
	case FishEngine::KeyCode::RightWindows: return 127; break;
	case FishEngine::KeyCode::AltGr: return 128; break;
	case FishEngine::KeyCode::Help: return 129; break;
	case FishEngine::KeyCode::Print: return 130; break;
	case FishEngine::KeyCode::SysReq: return 131; break;
	case FishEngine::KeyCode::Break: return 132; break;
	case FishEngine::KeyCode::Menu: return 133; break;
	case FishEngine::KeyCode::Mouse0: return 134; break;
	case FishEngine::KeyCode::Mouse1: return 135; break;
	case FishEngine::KeyCode::Mouse2: return 136; break;
	case FishEngine::KeyCode::Mouse3: return 137; break;
	case FishEngine::KeyCode::Mouse4: return 138; break;
	case FishEngine::KeyCode::Mouse5: return 139; break;
	case FishEngine::KeyCode::Mouse6: return 140; break;
	case FishEngine::KeyCode::JoystickButton0: return 141; break;
	case FishEngine::KeyCode::JoystickButton1: return 142; break;
	case FishEngine::KeyCode::JoystickButton2: return 143; break;
	case FishEngine::KeyCode::JoystickButton3: return 144; break;
	case FishEngine::KeyCode::JoystickButton4: return 145; break;
	case FishEngine::KeyCode::JoystickButton5: return 146; break;
	case FishEngine::KeyCode::JoystickButton6: return 147; break;
	case FishEngine::KeyCode::JoystickButton7: return 148; break;
	case FishEngine::KeyCode::JoystickButton8: return 149; break;
	case FishEngine::KeyCode::JoystickButton9: return 150; break;
	case FishEngine::KeyCode::JoystickButton10: return 151; break;
	case FishEngine::KeyCode::JoystickButton11: return 152; break;
	case FishEngine::KeyCode::JoystickButton12: return 153; break;
	case FishEngine::KeyCode::JoystickButton13: return 154; break;
	case FishEngine::KeyCode::JoystickButton14: return 155; break;
	case FishEngine::KeyCode::JoystickButton15: return 156; break;
	case FishEngine::KeyCode::JoystickButton16: return 157; break;
	case FishEngine::KeyCode::JoystickButton17: return 158; break;
	case FishEngine::KeyCode::JoystickButton18: return 159; break;
	case FishEngine::KeyCode::JoystickButton19: return 160; break;
	case FishEngine::KeyCode::Joystick1Button0: return 161; break;
	case FishEngine::KeyCode::Joystick1Button1: return 162; break;
	case FishEngine::KeyCode::Joystick1Button2: return 163; break;
	case FishEngine::KeyCode::Joystick1Button3: return 164; break;
	case FishEngine::KeyCode::Joystick1Button4: return 165; break;
	case FishEngine::KeyCode::Joystick1Button5: return 166; break;
	case FishEngine::KeyCode::Joystick1Button6: return 167; break;
	case FishEngine::KeyCode::Joystick1Button7: return 168; break;
	case FishEngine::KeyCode::Joystick1Button8: return 169; break;
	case FishEngine::KeyCode::Joystick1Button9: return 170; break;
	case FishEngine::KeyCode::Joystick1Button10: return 171; break;
	case FishEngine::KeyCode::Joystick1Button11: return 172; break;
	case FishEngine::KeyCode::Joystick1Button12: return 173; break;
	case FishEngine::KeyCode::Joystick1Button13: return 174; break;
	case FishEngine::KeyCode::Joystick1Button14: return 175; break;
	case FishEngine::KeyCode::Joystick1Button15: return 176; break;
	case FishEngine::KeyCode::Joystick1Button16: return 177; break;
	case FishEngine::KeyCode::Joystick1Button17: return 178; break;
	case FishEngine::KeyCode::Joystick1Button18: return 179; break;
	case FishEngine::KeyCode::Joystick1Button19: return 180; break;
	case FishEngine::KeyCode::Joystick2Button0: return 181; break;
	case FishEngine::KeyCode::Joystick2Button1: return 182; break;
	case FishEngine::KeyCode::Joystick2Button2: return 183; break;
	case FishEngine::KeyCode::Joystick2Button3: return 184; break;
	case FishEngine::KeyCode::Joystick2Button4: return 185; break;
	case FishEngine::KeyCode::Joystick2Button5: return 186; break;
	case FishEngine::KeyCode::Joystick2Button6: return 187; break;
	case FishEngine::KeyCode::Joystick2Button7: return 188; break;
	case FishEngine::KeyCode::Joystick2Button8: return 189; break;
	case FishEngine::KeyCode::Joystick2Button9: return 190; break;
	case FishEngine::KeyCode::Joystick2Button10: return 191; break;
	case FishEngine::KeyCode::Joystick2Button11: return 192; break;
	case FishEngine::KeyCode::Joystick2Button12: return 193; break;
	case FishEngine::KeyCode::Joystick2Button13: return 194; break;
	case FishEngine::KeyCode::Joystick2Button14: return 195; break;
	case FishEngine::KeyCode::Joystick2Button15: return 196; break;
	case FishEngine::KeyCode::Joystick2Button16: return 197; break;
	case FishEngine::KeyCode::Joystick2Button17: return 198; break;
	case FishEngine::KeyCode::Joystick2Button18: return 199; break;
	case FishEngine::KeyCode::Joystick2Button19: return 200; break;
	case FishEngine::KeyCode::Joystick3Button0: return 201; break;
	case FishEngine::KeyCode::Joystick3Button1: return 202; break;
	case FishEngine::KeyCode::Joystick3Button2: return 203; break;
	case FishEngine::KeyCode::Joystick3Button3: return 204; break;
	case FishEngine::KeyCode::Joystick3Button4: return 205; break;
	case FishEngine::KeyCode::Joystick3Button5: return 206; break;
	case FishEngine::KeyCode::Joystick3Button6: return 207; break;
	case FishEngine::KeyCode::Joystick3Button7: return 208; break;
	case FishEngine::KeyCode::Joystick3Button8: return 209; break;
	case FishEngine::KeyCode::Joystick3Button9: return 210; break;
	case FishEngine::KeyCode::Joystick3Button10: return 211; break;
	case FishEngine::KeyCode::Joystick3Button11: return 212; break;
	case FishEngine::KeyCode::Joystick3Button12: return 213; break;
	case FishEngine::KeyCode::Joystick3Button13: return 214; break;
	case FishEngine::KeyCode::Joystick3Button14: return 215; break;
	case FishEngine::KeyCode::Joystick3Button15: return 216; break;
	case FishEngine::KeyCode::Joystick3Button16: return 217; break;
	case FishEngine::KeyCode::Joystick3Button17: return 218; break;
	case FishEngine::KeyCode::Joystick3Button18: return 219; break;
	case FishEngine::KeyCode::Joystick3Button19: return 220; break;
	case FishEngine::KeyCode::Joystick4Button0: return 221; break;
	case FishEngine::KeyCode::Joystick4Button1: return 222; break;
	case FishEngine::KeyCode::Joystick4Button2: return 223; break;
	case FishEngine::KeyCode::Joystick4Button3: return 224; break;
	case FishEngine::KeyCode::Joystick4Button4: return 225; break;
	case FishEngine::KeyCode::Joystick4Button5: return 226; break;
	case FishEngine::KeyCode::Joystick4Button6: return 227; break;
	case FishEngine::KeyCode::Joystick4Button7: return 228; break;
	case FishEngine::KeyCode::Joystick4Button8: return 229; break;
	case FishEngine::KeyCode::Joystick4Button9: return 230; break;
	case FishEngine::KeyCode::Joystick4Button10: return 231; break;
	case FishEngine::KeyCode::Joystick4Button11: return 232; break;
	case FishEngine::KeyCode::Joystick4Button12: return 233; break;
	case FishEngine::KeyCode::Joystick4Button13: return 234; break;
	case FishEngine::KeyCode::Joystick4Button14: return 235; break;
	case FishEngine::KeyCode::Joystick4Button15: return 236; break;
	case FishEngine::KeyCode::Joystick4Button16: return 237; break;
	case FishEngine::KeyCode::Joystick4Button17: return 238; break;
	case FishEngine::KeyCode::Joystick4Button18: return 239; break;
	case FishEngine::KeyCode::Joystick4Button19: return 240; break;
	case FishEngine::KeyCode::Joystick5Button0: return 241; break;
	case FishEngine::KeyCode::Joystick5Button1: return 242; break;
	case FishEngine::KeyCode::Joystick5Button2: return 243; break;
	case FishEngine::KeyCode::Joystick5Button3: return 244; break;
	case FishEngine::KeyCode::Joystick5Button4: return 245; break;
	case FishEngine::KeyCode::Joystick5Button5: return 246; break;
	case FishEngine::KeyCode::Joystick5Button6: return 247; break;
	case FishEngine::KeyCode::Joystick5Button7: return 248; break;
	case FishEngine::KeyCode::Joystick5Button8: return 249; break;
	case FishEngine::KeyCode::Joystick5Button9: return 250; break;
	case FishEngine::KeyCode::Joystick5Button10: return 251; break;
	case FishEngine::KeyCode::Joystick5Button11: return 252; break;
	case FishEngine::KeyCode::Joystick5Button12: return 253; break;
	case FishEngine::KeyCode::Joystick5Button13: return 254; break;
	case FishEngine::KeyCode::Joystick5Button14: return 255; break;
	case FishEngine::KeyCode::Joystick5Button15: return 256; break;
	case FishEngine::KeyCode::Joystick5Button16: return 257; break;
	case FishEngine::KeyCode::Joystick5Button17: return 258; break;
	case FishEngine::KeyCode::Joystick5Button18: return 259; break;
	case FishEngine::KeyCode::Joystick5Button19: return 260; break;
	case FishEngine::KeyCode::Joystick6Button0: return 261; break;
	case FishEngine::KeyCode::Joystick6Button1: return 262; break;
	case FishEngine::KeyCode::Joystick6Button2: return 263; break;
	case FishEngine::KeyCode::Joystick6Button3: return 264; break;
	case FishEngine::KeyCode::Joystick6Button4: return 265; break;
	case FishEngine::KeyCode::Joystick6Button5: return 266; break;
	case FishEngine::KeyCode::Joystick6Button6: return 267; break;
	case FishEngine::KeyCode::Joystick6Button7: return 268; break;
	case FishEngine::KeyCode::Joystick6Button8: return 269; break;
	case FishEngine::KeyCode::Joystick6Button9: return 270; break;
	case FishEngine::KeyCode::Joystick6Button10: return 271; break;
	case FishEngine::KeyCode::Joystick6Button11: return 272; break;
	case FishEngine::KeyCode::Joystick6Button12: return 273; break;
	case FishEngine::KeyCode::Joystick6Button13: return 274; break;
	case FishEngine::KeyCode::Joystick6Button14: return 275; break;
	case FishEngine::KeyCode::Joystick6Button15: return 276; break;
	case FishEngine::KeyCode::Joystick6Button16: return 277; break;
	case FishEngine::KeyCode::Joystick6Button17: return 278; break;
	case FishEngine::KeyCode::Joystick6Button18: return 279; break;
	case FishEngine::KeyCode::Joystick6Button19: return 280; break;
	case FishEngine::KeyCode::Joystick7Button0: return 281; break;
	case FishEngine::KeyCode::Joystick7Button1: return 282; break;
	case FishEngine::KeyCode::Joystick7Button2: return 283; break;
	case FishEngine::KeyCode::Joystick7Button3: return 284; break;
	case FishEngine::KeyCode::Joystick7Button4: return 285; break;
	case FishEngine::KeyCode::Joystick7Button5: return 286; break;
	case FishEngine::KeyCode::Joystick7Button6: return 287; break;
	case FishEngine::KeyCode::Joystick7Button7: return 288; break;
	case FishEngine::KeyCode::Joystick7Button8: return 289; break;
	case FishEngine::KeyCode::Joystick7Button9: return 290; break;
	case FishEngine::KeyCode::Joystick7Button10: return 291; break;
	case FishEngine::KeyCode::Joystick7Button11: return 292; break;
	case FishEngine::KeyCode::Joystick7Button12: return 293; break;
	case FishEngine::KeyCode::Joystick7Button13: return 294; break;
	case FishEngine::KeyCode::Joystick7Button14: return 295; break;
	case FishEngine::KeyCode::Joystick7Button15: return 296; break;
	case FishEngine::KeyCode::Joystick7Button16: return 297; break;
	case FishEngine::KeyCode::Joystick7Button17: return 298; break;
	case FishEngine::KeyCode::Joystick7Button18: return 299; break;
	case FishEngine::KeyCode::Joystick7Button19: return 300; break;
	case FishEngine::KeyCode::Joystick8Button0: return 301; break;
	case FishEngine::KeyCode::Joystick8Button1: return 302; break;
	case FishEngine::KeyCode::Joystick8Button2: return 303; break;
	case FishEngine::KeyCode::Joystick8Button3: return 304; break;
	case FishEngine::KeyCode::Joystick8Button4: return 305; break;
	case FishEngine::KeyCode::Joystick8Button5: return 306; break;
	case FishEngine::KeyCode::Joystick8Button6: return 307; break;
	case FishEngine::KeyCode::Joystick8Button7: return 308; break;
	case FishEngine::KeyCode::Joystick8Button8: return 309; break;
	case FishEngine::KeyCode::Joystick8Button9: return 310; break;
	case FishEngine::KeyCode::Joystick8Button10: return 311; break;
	case FishEngine::KeyCode::Joystick8Button11: return 312; break;
	case FishEngine::KeyCode::Joystick8Button12: return 313; break;
	case FishEngine::KeyCode::Joystick8Button13: return 314; break;
	case FishEngine::KeyCode::Joystick8Button14: return 315; break;
	case FishEngine::KeyCode::Joystick8Button15: return 316; break;
	case FishEngine::KeyCode::Joystick8Button16: return 317; break;
	case FishEngine::KeyCode::Joystick8Button17: return 318; break;
	case FishEngine::KeyCode::Joystick8Button18: return 319; break;
	case FishEngine::KeyCode::Joystick8Button19: return 320; break;
	
    default: abort(); break;
    }
}

// string to enum
template<>
inline FishEngine::KeyCode ToEnum<FishEngine::KeyCode>(const std::string& s)
{
    if (s == "None") return FishEngine::KeyCode::None;
	if (s == "Backspace") return FishEngine::KeyCode::Backspace;
	if (s == "Delete") return FishEngine::KeyCode::Delete;
	if (s == "Tab") return FishEngine::KeyCode::Tab;
	if (s == "Clear") return FishEngine::KeyCode::Clear;
	if (s == "Return") return FishEngine::KeyCode::Return;
	if (s == "Pause") return FishEngine::KeyCode::Pause;
	if (s == "Escape") return FishEngine::KeyCode::Escape;
	if (s == "Space") return FishEngine::KeyCode::Space;
	if (s == "Keypad0") return FishEngine::KeyCode::Keypad0;
	if (s == "Keypad1") return FishEngine::KeyCode::Keypad1;
	if (s == "Keypad2") return FishEngine::KeyCode::Keypad2;
	if (s == "Keypad3") return FishEngine::KeyCode::Keypad3;
	if (s == "Keypad4") return FishEngine::KeyCode::Keypad4;
	if (s == "Keypad5") return FishEngine::KeyCode::Keypad5;
	if (s == "Keypad6") return FishEngine::KeyCode::Keypad6;
	if (s == "Keypad7") return FishEngine::KeyCode::Keypad7;
	if (s == "Keypad8") return FishEngine::KeyCode::Keypad8;
	if (s == "Keypad9") return FishEngine::KeyCode::Keypad9;
	if (s == "KeypadPeriod") return FishEngine::KeyCode::KeypadPeriod;
	if (s == "KeypadDivide") return FishEngine::KeyCode::KeypadDivide;
	if (s == "KeypadMultiply") return FishEngine::KeyCode::KeypadMultiply;
	if (s == "KeypadMinus") return FishEngine::KeyCode::KeypadMinus;
	if (s == "KeypadPlus") return FishEngine::KeyCode::KeypadPlus;
	if (s == "KeypadEnter") return FishEngine::KeyCode::KeypadEnter;
	if (s == "KeypadEquals") return FishEngine::KeyCode::KeypadEquals;
	if (s == "UpArrow") return FishEngine::KeyCode::UpArrow;
	if (s == "DownArrow") return FishEngine::KeyCode::DownArrow;
	if (s == "RightArrow") return FishEngine::KeyCode::RightArrow;
	if (s == "LeftArrow") return FishEngine::KeyCode::LeftArrow;
	if (s == "Insert") return FishEngine::KeyCode::Insert;
	if (s == "Home") return FishEngine::KeyCode::Home;
	if (s == "End") return FishEngine::KeyCode::End;
	if (s == "PageUp") return FishEngine::KeyCode::PageUp;
	if (s == "PageDown") return FishEngine::KeyCode::PageDown;
	if (s == "F1") return FishEngine::KeyCode::F1;
	if (s == "F2") return FishEngine::KeyCode::F2;
	if (s == "F3") return FishEngine::KeyCode::F3;
	if (s == "F4") return FishEngine::KeyCode::F4;
	if (s == "F5") return FishEngine::KeyCode::F5;
	if (s == "F6") return FishEngine::KeyCode::F6;
	if (s == "F7") return FishEngine::KeyCode::F7;
	if (s == "F8") return FishEngine::KeyCode::F8;
	if (s == "F9") return FishEngine::KeyCode::F9;
	if (s == "F10") return FishEngine::KeyCode::F10;
	if (s == "F11") return FishEngine::KeyCode::F11;
	if (s == "F12") return FishEngine::KeyCode::F12;
	if (s == "F13") return FishEngine::KeyCode::F13;
	if (s == "F14") return FishEngine::KeyCode::F14;
	if (s == "F15") return FishEngine::KeyCode::F15;
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
	if (s == "Exclaim") return FishEngine::KeyCode::Exclaim;
	if (s == "DoubleQuote") return FishEngine::KeyCode::DoubleQuote;
	if (s == "Hash") return FishEngine::KeyCode::Hash;
	if (s == "Dollar") return FishEngine::KeyCode::Dollar;
	if (s == "Ampersand") return FishEngine::KeyCode::Ampersand;
	if (s == "Quote") return FishEngine::KeyCode::Quote;
	if (s == "LeftParen") return FishEngine::KeyCode::LeftParen;
	if (s == "RightParen") return FishEngine::KeyCode::RightParen;
	if (s == "Asterisk") return FishEngine::KeyCode::Asterisk;
	if (s == "Plus") return FishEngine::KeyCode::Plus;
	if (s == "Comma") return FishEngine::KeyCode::Comma;
	if (s == "Minus") return FishEngine::KeyCode::Minus;
	if (s == "Period") return FishEngine::KeyCode::Period;
	if (s == "Slash") return FishEngine::KeyCode::Slash;
	if (s == "Colon") return FishEngine::KeyCode::Colon;
	if (s == "Semicolon") return FishEngine::KeyCode::Semicolon;
	if (s == "Less") return FishEngine::KeyCode::Less;
	if (s == "Equals") return FishEngine::KeyCode::Equals;
	if (s == "Greater") return FishEngine::KeyCode::Greater;
	if (s == "Question") return FishEngine::KeyCode::Question;
	if (s == "At") return FishEngine::KeyCode::At;
	if (s == "LeftBracket") return FishEngine::KeyCode::LeftBracket;
	if (s == "Backslash") return FishEngine::KeyCode::Backslash;
	if (s == "RightBracket") return FishEngine::KeyCode::RightBracket;
	if (s == "Caret") return FishEngine::KeyCode::Caret;
	if (s == "Underscore") return FishEngine::KeyCode::Underscore;
	if (s == "BackQuote") return FishEngine::KeyCode::BackQuote;
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
	if (s == "Numlock") return FishEngine::KeyCode::Numlock;
	if (s == "CapsLock") return FishEngine::KeyCode::CapsLock;
	if (s == "ScrollLock") return FishEngine::KeyCode::ScrollLock;
	if (s == "RightShift") return FishEngine::KeyCode::RightShift;
	if (s == "LeftShift") return FishEngine::KeyCode::LeftShift;
	if (s == "RightControl") return FishEngine::KeyCode::RightControl;
	if (s == "LeftControl") return FishEngine::KeyCode::LeftControl;
	if (s == "RightAlt") return FishEngine::KeyCode::RightAlt;
	if (s == "LeftAlt") return FishEngine::KeyCode::LeftAlt;
	if (s == "LeftCommand") return FishEngine::KeyCode::LeftCommand;
	if (s == "LeftApple") return FishEngine::KeyCode::LeftApple;
	if (s == "LeftWindows") return FishEngine::KeyCode::LeftWindows;
	if (s == "RightCommand") return FishEngine::KeyCode::RightCommand;
	if (s == "RightApple") return FishEngine::KeyCode::RightApple;
	if (s == "RightWindows") return FishEngine::KeyCode::RightWindows;
	if (s == "AltGr") return FishEngine::KeyCode::AltGr;
	if (s == "Help") return FishEngine::KeyCode::Help;
	if (s == "Print") return FishEngine::KeyCode::Print;
	if (s == "SysReq") return FishEngine::KeyCode::SysReq;
	if (s == "Break") return FishEngine::KeyCode::Break;
	if (s == "Menu") return FishEngine::KeyCode::Menu;
	if (s == "Mouse0") return FishEngine::KeyCode::Mouse0;
	if (s == "Mouse1") return FishEngine::KeyCode::Mouse1;
	if (s == "Mouse2") return FishEngine::KeyCode::Mouse2;
	if (s == "Mouse3") return FishEngine::KeyCode::Mouse3;
	if (s == "Mouse4") return FishEngine::KeyCode::Mouse4;
	if (s == "Mouse5") return FishEngine::KeyCode::Mouse5;
	if (s == "Mouse6") return FishEngine::KeyCode::Mouse6;
	if (s == "JoystickButton0") return FishEngine::KeyCode::JoystickButton0;
	if (s == "JoystickButton1") return FishEngine::KeyCode::JoystickButton1;
	if (s == "JoystickButton2") return FishEngine::KeyCode::JoystickButton2;
	if (s == "JoystickButton3") return FishEngine::KeyCode::JoystickButton3;
	if (s == "JoystickButton4") return FishEngine::KeyCode::JoystickButton4;
	if (s == "JoystickButton5") return FishEngine::KeyCode::JoystickButton5;
	if (s == "JoystickButton6") return FishEngine::KeyCode::JoystickButton6;
	if (s == "JoystickButton7") return FishEngine::KeyCode::JoystickButton7;
	if (s == "JoystickButton8") return FishEngine::KeyCode::JoystickButton8;
	if (s == "JoystickButton9") return FishEngine::KeyCode::JoystickButton9;
	if (s == "JoystickButton10") return FishEngine::KeyCode::JoystickButton10;
	if (s == "JoystickButton11") return FishEngine::KeyCode::JoystickButton11;
	if (s == "JoystickButton12") return FishEngine::KeyCode::JoystickButton12;
	if (s == "JoystickButton13") return FishEngine::KeyCode::JoystickButton13;
	if (s == "JoystickButton14") return FishEngine::KeyCode::JoystickButton14;
	if (s == "JoystickButton15") return FishEngine::KeyCode::JoystickButton15;
	if (s == "JoystickButton16") return FishEngine::KeyCode::JoystickButton16;
	if (s == "JoystickButton17") return FishEngine::KeyCode::JoystickButton17;
	if (s == "JoystickButton18") return FishEngine::KeyCode::JoystickButton18;
	if (s == "JoystickButton19") return FishEngine::KeyCode::JoystickButton19;
	if (s == "Joystick1Button0") return FishEngine::KeyCode::Joystick1Button0;
	if (s == "Joystick1Button1") return FishEngine::KeyCode::Joystick1Button1;
	if (s == "Joystick1Button2") return FishEngine::KeyCode::Joystick1Button2;
	if (s == "Joystick1Button3") return FishEngine::KeyCode::Joystick1Button3;
	if (s == "Joystick1Button4") return FishEngine::KeyCode::Joystick1Button4;
	if (s == "Joystick1Button5") return FishEngine::KeyCode::Joystick1Button5;
	if (s == "Joystick1Button6") return FishEngine::KeyCode::Joystick1Button6;
	if (s == "Joystick1Button7") return FishEngine::KeyCode::Joystick1Button7;
	if (s == "Joystick1Button8") return FishEngine::KeyCode::Joystick1Button8;
	if (s == "Joystick1Button9") return FishEngine::KeyCode::Joystick1Button9;
	if (s == "Joystick1Button10") return FishEngine::KeyCode::Joystick1Button10;
	if (s == "Joystick1Button11") return FishEngine::KeyCode::Joystick1Button11;
	if (s == "Joystick1Button12") return FishEngine::KeyCode::Joystick1Button12;
	if (s == "Joystick1Button13") return FishEngine::KeyCode::Joystick1Button13;
	if (s == "Joystick1Button14") return FishEngine::KeyCode::Joystick1Button14;
	if (s == "Joystick1Button15") return FishEngine::KeyCode::Joystick1Button15;
	if (s == "Joystick1Button16") return FishEngine::KeyCode::Joystick1Button16;
	if (s == "Joystick1Button17") return FishEngine::KeyCode::Joystick1Button17;
	if (s == "Joystick1Button18") return FishEngine::KeyCode::Joystick1Button18;
	if (s == "Joystick1Button19") return FishEngine::KeyCode::Joystick1Button19;
	if (s == "Joystick2Button0") return FishEngine::KeyCode::Joystick2Button0;
	if (s == "Joystick2Button1") return FishEngine::KeyCode::Joystick2Button1;
	if (s == "Joystick2Button2") return FishEngine::KeyCode::Joystick2Button2;
	if (s == "Joystick2Button3") return FishEngine::KeyCode::Joystick2Button3;
	if (s == "Joystick2Button4") return FishEngine::KeyCode::Joystick2Button4;
	if (s == "Joystick2Button5") return FishEngine::KeyCode::Joystick2Button5;
	if (s == "Joystick2Button6") return FishEngine::KeyCode::Joystick2Button6;
	if (s == "Joystick2Button7") return FishEngine::KeyCode::Joystick2Button7;
	if (s == "Joystick2Button8") return FishEngine::KeyCode::Joystick2Button8;
	if (s == "Joystick2Button9") return FishEngine::KeyCode::Joystick2Button9;
	if (s == "Joystick2Button10") return FishEngine::KeyCode::Joystick2Button10;
	if (s == "Joystick2Button11") return FishEngine::KeyCode::Joystick2Button11;
	if (s == "Joystick2Button12") return FishEngine::KeyCode::Joystick2Button12;
	if (s == "Joystick2Button13") return FishEngine::KeyCode::Joystick2Button13;
	if (s == "Joystick2Button14") return FishEngine::KeyCode::Joystick2Button14;
	if (s == "Joystick2Button15") return FishEngine::KeyCode::Joystick2Button15;
	if (s == "Joystick2Button16") return FishEngine::KeyCode::Joystick2Button16;
	if (s == "Joystick2Button17") return FishEngine::KeyCode::Joystick2Button17;
	if (s == "Joystick2Button18") return FishEngine::KeyCode::Joystick2Button18;
	if (s == "Joystick2Button19") return FishEngine::KeyCode::Joystick2Button19;
	if (s == "Joystick3Button0") return FishEngine::KeyCode::Joystick3Button0;
	if (s == "Joystick3Button1") return FishEngine::KeyCode::Joystick3Button1;
	if (s == "Joystick3Button2") return FishEngine::KeyCode::Joystick3Button2;
	if (s == "Joystick3Button3") return FishEngine::KeyCode::Joystick3Button3;
	if (s == "Joystick3Button4") return FishEngine::KeyCode::Joystick3Button4;
	if (s == "Joystick3Button5") return FishEngine::KeyCode::Joystick3Button5;
	if (s == "Joystick3Button6") return FishEngine::KeyCode::Joystick3Button6;
	if (s == "Joystick3Button7") return FishEngine::KeyCode::Joystick3Button7;
	if (s == "Joystick3Button8") return FishEngine::KeyCode::Joystick3Button8;
	if (s == "Joystick3Button9") return FishEngine::KeyCode::Joystick3Button9;
	if (s == "Joystick3Button10") return FishEngine::KeyCode::Joystick3Button10;
	if (s == "Joystick3Button11") return FishEngine::KeyCode::Joystick3Button11;
	if (s == "Joystick3Button12") return FishEngine::KeyCode::Joystick3Button12;
	if (s == "Joystick3Button13") return FishEngine::KeyCode::Joystick3Button13;
	if (s == "Joystick3Button14") return FishEngine::KeyCode::Joystick3Button14;
	if (s == "Joystick3Button15") return FishEngine::KeyCode::Joystick3Button15;
	if (s == "Joystick3Button16") return FishEngine::KeyCode::Joystick3Button16;
	if (s == "Joystick3Button17") return FishEngine::KeyCode::Joystick3Button17;
	if (s == "Joystick3Button18") return FishEngine::KeyCode::Joystick3Button18;
	if (s == "Joystick3Button19") return FishEngine::KeyCode::Joystick3Button19;
	if (s == "Joystick4Button0") return FishEngine::KeyCode::Joystick4Button0;
	if (s == "Joystick4Button1") return FishEngine::KeyCode::Joystick4Button1;
	if (s == "Joystick4Button2") return FishEngine::KeyCode::Joystick4Button2;
	if (s == "Joystick4Button3") return FishEngine::KeyCode::Joystick4Button3;
	if (s == "Joystick4Button4") return FishEngine::KeyCode::Joystick4Button4;
	if (s == "Joystick4Button5") return FishEngine::KeyCode::Joystick4Button5;
	if (s == "Joystick4Button6") return FishEngine::KeyCode::Joystick4Button6;
	if (s == "Joystick4Button7") return FishEngine::KeyCode::Joystick4Button7;
	if (s == "Joystick4Button8") return FishEngine::KeyCode::Joystick4Button8;
	if (s == "Joystick4Button9") return FishEngine::KeyCode::Joystick4Button9;
	if (s == "Joystick4Button10") return FishEngine::KeyCode::Joystick4Button10;
	if (s == "Joystick4Button11") return FishEngine::KeyCode::Joystick4Button11;
	if (s == "Joystick4Button12") return FishEngine::KeyCode::Joystick4Button12;
	if (s == "Joystick4Button13") return FishEngine::KeyCode::Joystick4Button13;
	if (s == "Joystick4Button14") return FishEngine::KeyCode::Joystick4Button14;
	if (s == "Joystick4Button15") return FishEngine::KeyCode::Joystick4Button15;
	if (s == "Joystick4Button16") return FishEngine::KeyCode::Joystick4Button16;
	if (s == "Joystick4Button17") return FishEngine::KeyCode::Joystick4Button17;
	if (s == "Joystick4Button18") return FishEngine::KeyCode::Joystick4Button18;
	if (s == "Joystick4Button19") return FishEngine::KeyCode::Joystick4Button19;
	if (s == "Joystick5Button0") return FishEngine::KeyCode::Joystick5Button0;
	if (s == "Joystick5Button1") return FishEngine::KeyCode::Joystick5Button1;
	if (s == "Joystick5Button2") return FishEngine::KeyCode::Joystick5Button2;
	if (s == "Joystick5Button3") return FishEngine::KeyCode::Joystick5Button3;
	if (s == "Joystick5Button4") return FishEngine::KeyCode::Joystick5Button4;
	if (s == "Joystick5Button5") return FishEngine::KeyCode::Joystick5Button5;
	if (s == "Joystick5Button6") return FishEngine::KeyCode::Joystick5Button6;
	if (s == "Joystick5Button7") return FishEngine::KeyCode::Joystick5Button7;
	if (s == "Joystick5Button8") return FishEngine::KeyCode::Joystick5Button8;
	if (s == "Joystick5Button9") return FishEngine::KeyCode::Joystick5Button9;
	if (s == "Joystick5Button10") return FishEngine::KeyCode::Joystick5Button10;
	if (s == "Joystick5Button11") return FishEngine::KeyCode::Joystick5Button11;
	if (s == "Joystick5Button12") return FishEngine::KeyCode::Joystick5Button12;
	if (s == "Joystick5Button13") return FishEngine::KeyCode::Joystick5Button13;
	if (s == "Joystick5Button14") return FishEngine::KeyCode::Joystick5Button14;
	if (s == "Joystick5Button15") return FishEngine::KeyCode::Joystick5Button15;
	if (s == "Joystick5Button16") return FishEngine::KeyCode::Joystick5Button16;
	if (s == "Joystick5Button17") return FishEngine::KeyCode::Joystick5Button17;
	if (s == "Joystick5Button18") return FishEngine::KeyCode::Joystick5Button18;
	if (s == "Joystick5Button19") return FishEngine::KeyCode::Joystick5Button19;
	if (s == "Joystick6Button0") return FishEngine::KeyCode::Joystick6Button0;
	if (s == "Joystick6Button1") return FishEngine::KeyCode::Joystick6Button1;
	if (s == "Joystick6Button2") return FishEngine::KeyCode::Joystick6Button2;
	if (s == "Joystick6Button3") return FishEngine::KeyCode::Joystick6Button3;
	if (s == "Joystick6Button4") return FishEngine::KeyCode::Joystick6Button4;
	if (s == "Joystick6Button5") return FishEngine::KeyCode::Joystick6Button5;
	if (s == "Joystick6Button6") return FishEngine::KeyCode::Joystick6Button6;
	if (s == "Joystick6Button7") return FishEngine::KeyCode::Joystick6Button7;
	if (s == "Joystick6Button8") return FishEngine::KeyCode::Joystick6Button8;
	if (s == "Joystick6Button9") return FishEngine::KeyCode::Joystick6Button9;
	if (s == "Joystick6Button10") return FishEngine::KeyCode::Joystick6Button10;
	if (s == "Joystick6Button11") return FishEngine::KeyCode::Joystick6Button11;
	if (s == "Joystick6Button12") return FishEngine::KeyCode::Joystick6Button12;
	if (s == "Joystick6Button13") return FishEngine::KeyCode::Joystick6Button13;
	if (s == "Joystick6Button14") return FishEngine::KeyCode::Joystick6Button14;
	if (s == "Joystick6Button15") return FishEngine::KeyCode::Joystick6Button15;
	if (s == "Joystick6Button16") return FishEngine::KeyCode::Joystick6Button16;
	if (s == "Joystick6Button17") return FishEngine::KeyCode::Joystick6Button17;
	if (s == "Joystick6Button18") return FishEngine::KeyCode::Joystick6Button18;
	if (s == "Joystick6Button19") return FishEngine::KeyCode::Joystick6Button19;
	if (s == "Joystick7Button0") return FishEngine::KeyCode::Joystick7Button0;
	if (s == "Joystick7Button1") return FishEngine::KeyCode::Joystick7Button1;
	if (s == "Joystick7Button2") return FishEngine::KeyCode::Joystick7Button2;
	if (s == "Joystick7Button3") return FishEngine::KeyCode::Joystick7Button3;
	if (s == "Joystick7Button4") return FishEngine::KeyCode::Joystick7Button4;
	if (s == "Joystick7Button5") return FishEngine::KeyCode::Joystick7Button5;
	if (s == "Joystick7Button6") return FishEngine::KeyCode::Joystick7Button6;
	if (s == "Joystick7Button7") return FishEngine::KeyCode::Joystick7Button7;
	if (s == "Joystick7Button8") return FishEngine::KeyCode::Joystick7Button8;
	if (s == "Joystick7Button9") return FishEngine::KeyCode::Joystick7Button9;
	if (s == "Joystick7Button10") return FishEngine::KeyCode::Joystick7Button10;
	if (s == "Joystick7Button11") return FishEngine::KeyCode::Joystick7Button11;
	if (s == "Joystick7Button12") return FishEngine::KeyCode::Joystick7Button12;
	if (s == "Joystick7Button13") return FishEngine::KeyCode::Joystick7Button13;
	if (s == "Joystick7Button14") return FishEngine::KeyCode::Joystick7Button14;
	if (s == "Joystick7Button15") return FishEngine::KeyCode::Joystick7Button15;
	if (s == "Joystick7Button16") return FishEngine::KeyCode::Joystick7Button16;
	if (s == "Joystick7Button17") return FishEngine::KeyCode::Joystick7Button17;
	if (s == "Joystick7Button18") return FishEngine::KeyCode::Joystick7Button18;
	if (s == "Joystick7Button19") return FishEngine::KeyCode::Joystick7Button19;
	if (s == "Joystick8Button0") return FishEngine::KeyCode::Joystick8Button0;
	if (s == "Joystick8Button1") return FishEngine::KeyCode::Joystick8Button1;
	if (s == "Joystick8Button2") return FishEngine::KeyCode::Joystick8Button2;
	if (s == "Joystick8Button3") return FishEngine::KeyCode::Joystick8Button3;
	if (s == "Joystick8Button4") return FishEngine::KeyCode::Joystick8Button4;
	if (s == "Joystick8Button5") return FishEngine::KeyCode::Joystick8Button5;
	if (s == "Joystick8Button6") return FishEngine::KeyCode::Joystick8Button6;
	if (s == "Joystick8Button7") return FishEngine::KeyCode::Joystick8Button7;
	if (s == "Joystick8Button8") return FishEngine::KeyCode::Joystick8Button8;
	if (s == "Joystick8Button9") return FishEngine::KeyCode::Joystick8Button9;
	if (s == "Joystick8Button10") return FishEngine::KeyCode::Joystick8Button10;
	if (s == "Joystick8Button11") return FishEngine::KeyCode::Joystick8Button11;
	if (s == "Joystick8Button12") return FishEngine::KeyCode::Joystick8Button12;
	if (s == "Joystick8Button13") return FishEngine::KeyCode::Joystick8Button13;
	if (s == "Joystick8Button14") return FishEngine::KeyCode::Joystick8Button14;
	if (s == "Joystick8Button15") return FishEngine::KeyCode::Joystick8Button15;
	if (s == "Joystick8Button16") return FishEngine::KeyCode::Joystick8Button16;
	if (s == "Joystick8Button17") return FishEngine::KeyCode::Joystick8Button17;
	if (s == "Joystick8Button18") return FishEngine::KeyCode::Joystick8Button18;
	if (s == "Joystick8Button19") return FishEngine::KeyCode::Joystick8Button19;
	
    abort();
}


} // namespace FishEngine
