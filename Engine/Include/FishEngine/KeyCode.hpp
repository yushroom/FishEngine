#pragma once

namespace FishEngine
{
	/// <summary>
	///   <para>Key codes returned by Event.keyCode. These map directly to a physical key on the keyboard.</para>
	/// </summary>
	enum class KeyCode
	{
		/// <summary>
		///   <para>Not assigned (never returned as the result of a keystroke).</para>
		/// </summary>
		None,
		/// <summary>
		///   <para>The backspace key.</para>
		/// </summary>
		Backspace = 8,
		/// <summary>
		///   <para>The forward delete key.</para>
		/// </summary>
		Delete = 127,
		/// <summary>
		///   <para>The tab key.</para>
		/// </summary>
		Tab = 9,
		/// <summary>
		///   <para>The Clear key.</para>
		/// </summary>
		Clear = 12,
		/// <summary>
		///   <para>Return key.</para>
		/// </summary>
		Return,
		/// <summary>
		///   <para>Pause on PC machines.</para>
		/// </summary>
		Pause = 19,
		/// <summary>
		///   <para>Escape key.</para>
		/// </summary>
		Escape = 27,
		/// <summary>
		///   <para>Space key.</para>
		/// </summary>
		Space = 32,
		/// <summary>
		///   <para>Numeric keypad 0.</para>
		/// </summary>
		Keypad0 = 256,
		/// <summary>
		///   <para>Numeric keypad 1.</para>
		/// </summary>
		Keypad1,
		/// <summary>
		///   <para>Numeric keypad 2.</para>
		/// </summary>
		Keypad2,
		/// <summary>
		///   <para>Numeric keypad 3.</para>
		/// </summary>
		Keypad3,
		/// <summary>
		///   <para>Numeric keypad 4.</para>
		/// </summary>
		Keypad4,
		/// <summary>
		///   <para>Numeric keypad 5.</para>
		/// </summary>
		Keypad5,
		/// <summary>
		///   <para>Numeric keypad 6.</para>
		/// </summary>
		Keypad6,
		/// <summary>
		///   <para>Numeric keypad 7.</para>
		/// </summary>
		Keypad7,
		/// <summary>
		///   <para>Numeric keypad 8.</para>
		/// </summary>
		Keypad8,
		/// <summary>
		///   <para>Numeric keypad 9.</para>
		/// </summary>
		Keypad9,
		/// <summary>
		///   <para>Numeric keypad '.'.</para>
		/// </summary>
		KeypadPeriod,
		/// <summary>
		///   <para>Numeric keypad '/'.</para>
		/// </summary>
		KeypadDivide,
		/// <summary>
		///   <para>Numeric keypad '*'.</para>
		/// </summary>
		KeypadMultiply,
		/// <summary>
		///   <para>Numeric keypad '-'.</para>
		/// </summary>
		KeypadMinus,
		/// <summary>
		///   <para>Numeric keypad '+'.</para>
		/// </summary>
		KeypadPlus,
		/// <summary>
		///   <para>Numeric keypad enter.</para>
		/// </summary>
		KeypadEnter,
		/// <summary>
		///   <para>Numeric keypad '='.</para>
		/// </summary>
		KeypadEquals,
		/// <summary>
		///   <para>Up arrow key.</para>
		/// </summary>
		UpArrow,
		/// <summary>
		///   <para>Down arrow key.</para>
		/// </summary>
		DownArrow,
		/// <summary>
		///   <para>Right arrow key.</para>
		/// </summary>
		RightArrow,
		/// <summary>
		///   <para>Left arrow key.</para>
		/// </summary>
		LeftArrow,
		/// <summary>
		///   <para>Insert key key.</para>
		/// </summary>
		Insert,
		/// <summary>
		///   <para>Home key.</para>
		/// </summary>
		Home,
		/// <summary>
		///   <para>End key.</para>
		/// </summary>
		End,
		/// <summary>
		///   <para>Page up.</para>
		/// </summary>
		PageUp,
		/// <summary>
		///   <para>Page down.</para>
		/// </summary>
		PageDown,
		/// <summary>
		///   <para>F1 function key.</para>
		/// </summary>
		F1,
		/// <summary>
		///   <para>F2 function key.</para>
		/// </summary>
		F2,
		/// <summary>
		///   <para>F3 function key.</para>
		/// </summary>
		F3,
		/// <summary>
		///   <para>F4 function key.</para>
		/// </summary>
		F4,
		/// <summary>
		///   <para>F5 function key.</para>
		/// </summary>
		F5,
		/// <summary>
		///   <para>F6 function key.</para>
		/// </summary>
		F6,
		/// <summary>
		///   <para>F7 function key.</para>
		/// </summary>
		F7,
		/// <summary>
		///   <para>F8 function key.</para>
		/// </summary>
		F8,
		/// <summary>
		///   <para>F9 function key.</para>
		/// </summary>
		F9,
		/// <summary>
		///   <para>F10 function key.</para>
		/// </summary>
		F10,
		/// <summary>
		///   <para>F11 function key.</para>
		/// </summary>
		F11,
		/// <summary>
		///   <para>F12 function key.</para>
		/// </summary>
		F12,
		/// <summary>
		///   <para>F13 function key.</para>
		/// </summary>
		F13,
		/// <summary>
		///   <para>F14 function key.</para>
		/// </summary>
		F14,
		/// <summary>
		///   <para>F15 function key.</para>
		/// </summary>
		F15,
		/// <summary>
		///   <para>The '0' key on the top of the alphanumeric keyboard.</para>
		/// </summary>
		Alpha0 = 48,
		/// <summary>
		///   <para>The '1' key on the top of the alphanumeric keyboard.</para>
		/// </summary>
		Alpha1,
		/// <summary>
		///   <para>The '2' key on the top of the alphanumeric keyboard.</para>
		/// </summary>
		Alpha2,
		/// <summary>
		///   <para>The '3' key on the top of the alphanumeric keyboard.</para>
		/// </summary>
		Alpha3,
		/// <summary>
		///   <para>The '4' key on the top of the alphanumeric keyboard.</para>
		/// </summary>
		Alpha4,
		/// <summary>
		///   <para>The '5' key on the top of the alphanumeric keyboard.</para>
		/// </summary>
		Alpha5,
		/// <summary>
		///   <para>The '6' key on the top of the alphanumeric keyboard.</para>
		/// </summary>
		Alpha6,
		/// <summary>
		///   <para>The '7' key on the top of the alphanumeric keyboard.</para>
		/// </summary>
		Alpha7,
		/// <summary>
		///   <para>The '8' key on the top of the alphanumeric keyboard.</para>
		/// </summary>
		Alpha8,
		/// <summary>
		///   <para>The '9' key on the top of the alphanumeric keyboard.</para>
		/// </summary>
		Alpha9,
		/// <summary>
		///   <para>Exclamation mark key '!'.</para>
		/// </summary>
		Exclaim = 33,
		/// <summary>
		///   <para>Double quote key '"'.</para>
		/// </summary>
		DoubleQuote,
		/// <summary>
		///   <para>Hash key '#'.</para>
		/// </summary>
		Hash,
		/// <summary>
		///   <para>Dollar sign key '$'.</para>
		/// </summary>
		Dollar,
		/// <summary>
		///   <para>Ampersand key '&amp;'.</para>
		/// </summary>
		Ampersand = 38,
		/// <summary>
		///   <para>Quote key '.</para>
		/// </summary>
		Quote,
		/// <summary>
		///   <para>Left Parenthesis key '('.</para>
		/// </summary>
		LeftParen,
		/// <summary>
		///   <para>Right Parenthesis key ')'.</para>
		/// </summary>
		RightParen,
		/// <summary>
		///   <para>Asterisk key '*'.</para>
		/// </summary>
		Asterisk,
		/// <summary>
		///   <para>Plus key '+'.</para>
		/// </summary>
		Plus,
		/// <summary>
		///   <para>Comma ',' key.</para>
		/// </summary>
		Comma,
		/// <summary>
		///   <para>Minus '-' key.</para>
		/// </summary>
		Minus,
		/// <summary>
		///   <para>Period '.' key.</para>
		/// </summary>
		Period,
		/// <summary>
		///   <para>Slash '/' key.</para>
		/// </summary>
		Slash,
		/// <summary>
		///   <para>Colon ':' key.</para>
		/// </summary>
		Colon = 58,
		/// <summary>
		///   <para>Semicolon ';' key.</para>
		/// </summary>
		Semicolon,
		/// <summary>
		///   <para>Less than '&lt;' key.</para>
		/// </summary>
		Less,
		/// <summary>
		///   <para>Equals '=' key.</para>
		/// </summary>
		Equals,
		/// <summary>
		///   <para>Greater than '&gt;' key.</para>
		/// </summary>
		Greater,
		/// <summary>
		///   <para>Question mark '?' key.</para>
		/// </summary>
		Question,
		/// <summary>
		///   <para>At key '@'.</para>
		/// </summary>
		At,
		/// <summary>
		///   <para>Left square bracket key '['.</para>
		/// </summary>
		LeftBracket = 91,
		/// <summary>
		///   <para>Backslash key '\'.</para>
		/// </summary>
		Backslash,
		/// <summary>
		///   <para>Right square bracket key ']'.</para>
		/// </summary>
		RightBracket,
		/// <summary>
		///   <para>Caret key '^'.</para>
		/// </summary>
		Caret,
		/// <summary>
		///   <para>Underscore '_' key.</para>
		/// </summary>
		Underscore,
		/// <summary>
		///   <para>Back quote key '`'.</para>
		/// </summary>
		BackQuote,
		/// <summary>
		///   <para>'a' key.</para>
		/// </summary>
		A,
		/// <summary>
		///   <para>'b' key.</para>
		/// </summary>
		B,
		/// <summary>
		///   <para>'c' key.</para>
		/// </summary>
		C,
		/// <summary>
		///   <para>'d' key.</para>
		/// </summary>
		D,
		/// <summary>
		///   <para>'e' key.</para>
		/// </summary>
		E,
		/// <summary>
		///   <para>'f' key.</para>
		/// </summary>
		F,
		/// <summary>
		///   <para>'g' key.</para>
		/// </summary>
		G,
		/// <summary>
		///   <para>'h' key.</para>
		/// </summary>
		H,
		/// <summary>
		///   <para>'i' key.</para>
		/// </summary>
		I,
		/// <summary>
		///   <para>'j' key.</para>
		/// </summary>
		J,
		/// <summary>
		///   <para>'k' key.</para>
		/// </summary>
		K,
		/// <summary>
		///   <para>'l' key.</para>
		/// </summary>
		L,
		/// <summary>
		///   <para>'m' key.</para>
		/// </summary>
		M,
		/// <summary>
		///   <para>'n' key.</para>
		/// </summary>
		N,
		/// <summary>
		///   <para>'o' key.</para>
		/// </summary>
		O,
		/// <summary>
		///   <para>'p' key.</para>
		/// </summary>
		P,
		/// <summary>
		///   <para>'q' key.</para>
		/// </summary>
		Q,
		/// <summary>
		///   <para>'r' key.</para>
		/// </summary>
		R,
		/// <summary>
		///   <para>'s' key.</para>
		/// </summary>
		S,
		/// <summary>
		///   <para>'t' key.</para>
		/// </summary>
		T,
		/// <summary>
		///   <para>'u' key.</para>
		/// </summary>
		U,
		/// <summary>
		///   <para>'v' key.</para>
		/// </summary>
		V,
		/// <summary>
		///   <para>'w' key.</para>
		/// </summary>
		W,
		/// <summary>
		///   <para>'x' key.</para>
		/// </summary>
		X,
		/// <summary>
		///   <para>'y' key.</para>
		/// </summary>
		Y,
		/// <summary>
		///   <para>'z' key.</para>
		/// </summary>
		Z,
		/// <summary>
		///   <para>Numlock key.</para>
		/// </summary>
		Numlock = 300,
		/// <summary>
		///   <para>Capslock key.</para>
		/// </summary>
		CapsLock,
		/// <summary>
		///   <para>Scroll lock key.</para>
		/// </summary>
		ScrollLock,
		/// <summary>
		///   <para>Right shift key.</para>
		/// </summary>
		RightShift,
		/// <summary>
		///   <para>Left shift key.</para>
		/// </summary>
		LeftShift,
		/// <summary>
		///   <para>Right Control key.</para>
		/// </summary>
		RightControl,
		/// <summary>
		///   <para>Left Control key.</para>
		/// </summary>
		LeftControl,
		/// <summary>
		///   <para>Right Alt key.</para>
		/// </summary>
		RightAlt,
		/// <summary>
		///   <para>Left Alt key.</para>
		/// </summary>
		LeftAlt,
		/// <summary>
		///   <para>Left Command key.</para>
		/// </summary>
		LeftCommand = 310,
		/// <summary>
		///   <para>Left Command key.</para>
		/// </summary>
		LeftApple = 310,
		/// <summary>
		///   <para>Left Windows key.</para>
		/// </summary>
		LeftWindows,
		/// <summary>
		///   <para>Right Command key.</para>
		/// </summary>
		RightCommand = 309,
		/// <summary>
		///   <para>Right Command key.</para>
		/// </summary>
		RightApple = 309,
		/// <summary>
		///   <para>Right Windows key.</para>
		/// </summary>
		RightWindows = 312,
		/// <summary>
		///   <para>Alt Gr key.</para>
		/// </summary>
		AltGr,
		/// <summary>
		///   <para>Help key.</para>
		/// </summary>
		Help = 315,
		/// <summary>
		///   <para>Print key.</para>
		/// </summary>
		Print,
		/// <summary>
		///   <para>Sys Req key.</para>
		/// </summary>
		SysReq,
		/// <summary>
		///   <para>Break key.</para>
		/// </summary>
		Break,
		/// <summary>
		///   <para>Menu key.</para>
		/// </summary>
		Menu,
		/// <summary>
		///   <para>First (primary) mouse button.</para>
		/// </summary>
		Mouse0 = 323,
		/// <summary>
		///   <para>Second (secondary) mouse button.</para>
		/// </summary>
		Mouse1,
		/// <summary>
		///   <para>Third mouse button.</para>
		/// </summary>
		Mouse2,
		/// <summary>
		///   <para>Fourth mouse button.</para>
		/// </summary>
		Mouse3,
		/// <summary>
		///   <para>Fifth mouse button.</para>
		/// </summary>
		Mouse4,
		/// <summary>
		///   <para>Sixth mouse button.</para>
		/// </summary>
		Mouse5,
		/// <summary>
		///   <para>Seventh mouse button.</para>
		/// </summary>
		Mouse6,
		/// <summary>
		///   <para>Button 0 on any joystick.</para>
		/// </summary>
		JoystickButton0,
		/// <summary>
		///   <para>Button 1 on any joystick.</para>
		/// </summary>
		JoystickButton1,
		/// <summary>
		///   <para>Button 2 on any joystick.</para>
		/// </summary>
		JoystickButton2,
		/// <summary>
		///   <para>Button 3 on any joystick.</para>
		/// </summary>
		JoystickButton3,
		/// <summary>
		///   <para>Button 4 on any joystick.</para>
		/// </summary>
		JoystickButton4,
		/// <summary>
		///   <para>Button 5 on any joystick.</para>
		/// </summary>
		JoystickButton5,
		/// <summary>
		///   <para>Button 6 on any joystick.</para>
		/// </summary>
		JoystickButton6,
		/// <summary>
		///   <para>Button 7 on any joystick.</para>
		/// </summary>
		JoystickButton7,
		/// <summary>
		///   <para>Button 8 on any joystick.</para>
		/// </summary>
		JoystickButton8,
		/// <summary>
		///   <para>Button 9 on any joystick.</para>
		/// </summary>
		JoystickButton9,
		/// <summary>
		///   <para>Button 10 on any joystick.</para>
		/// </summary>
		JoystickButton10,
		/// <summary>
		///   <para>Button 11 on any joystick.</para>
		/// </summary>
		JoystickButton11,
		/// <summary>
		///   <para>Button 12 on any joystick.</para>
		/// </summary>
		JoystickButton12,
		/// <summary>
		///   <para>Button 13 on any joystick.</para>
		/// </summary>
		JoystickButton13,
		/// <summary>
		///   <para>Button 14 on any joystick.</para>
		/// </summary>
		JoystickButton14,
		/// <summary>
		///   <para>Button 15 on any joystick.</para>
		/// </summary>
		JoystickButton15,
		/// <summary>
		///   <para>Button 16 on any joystick.</para>
		/// </summary>
		JoystickButton16,
		/// <summary>
		///   <para>Button 17 on any joystick.</para>
		/// </summary>
		JoystickButton17,
		/// <summary>
		///   <para>Button 18 on any joystick.</para>
		/// </summary>
		JoystickButton18,
		/// <summary>
		///   <para>Button 19 on any joystick.</para>
		/// </summary>
		JoystickButton19,
		/// <summary>
		///   <para>Button 0 on first joystick.</para>
		/// </summary>
		Joystick1Button0,
		/// <summary>
		///   <para>Button 1 on first joystick.</para>
		/// </summary>
		Joystick1Button1,
		/// <summary>
		///   <para>Button 2 on first joystick.</para>
		/// </summary>
		Joystick1Button2,
		/// <summary>
		///   <para>Button 3 on first joystick.</para>
		/// </summary>
		Joystick1Button3,
		/// <summary>
		///   <para>Button 4 on first joystick.</para>
		/// </summary>
		Joystick1Button4,
		/// <summary>
		///   <para>Button 5 on first joystick.</para>
		/// </summary>
		Joystick1Button5,
		/// <summary>
		///   <para>Button 6 on first joystick.</para>
		/// </summary>
		Joystick1Button6,
		/// <summary>
		///   <para>Button 7 on first joystick.</para>
		/// </summary>
		Joystick1Button7,
		/// <summary>
		///   <para>Button 8 on first joystick.</para>
		/// </summary>
		Joystick1Button8,
		/// <summary>
		///   <para>Button 9 on first joystick.</para>
		/// </summary>
		Joystick1Button9,
		/// <summary>
		///   <para>Button 10 on first joystick.</para>
		/// </summary>
		Joystick1Button10,
		/// <summary>
		///   <para>Button 11 on first joystick.</para>
		/// </summary>
		Joystick1Button11,
		/// <summary>
		///   <para>Button 12 on first joystick.</para>
		/// </summary>
		Joystick1Button12,
		/// <summary>
		///   <para>Button 13 on first joystick.</para>
		/// </summary>
		Joystick1Button13,
		/// <summary>
		///   <para>Button 14 on first joystick.</para>
		/// </summary>
		Joystick1Button14,
		/// <summary>
		///   <para>Button 15 on first joystick.</para>
		/// </summary>
		Joystick1Button15,
		/// <summary>
		///   <para>Button 16 on first joystick.</para>
		/// </summary>
		Joystick1Button16,
		/// <summary>
		///   <para>Button 17 on first joystick.</para>
		/// </summary>
		Joystick1Button17,
		/// <summary>
		///   <para>Button 18 on first joystick.</para>
		/// </summary>
		Joystick1Button18,
		/// <summary>
		///   <para>Button 19 on first joystick.</para>
		/// </summary>
		Joystick1Button19,
		/// <summary>
		///   <para>Button 0 on second joystick.</para>
		/// </summary>
		Joystick2Button0,
		/// <summary>
		///   <para>Button 1 on second joystick.</para>
		/// </summary>
		Joystick2Button1,
		/// <summary>
		///   <para>Button 2 on second joystick.</para>
		/// </summary>
		Joystick2Button2,
		/// <summary>
		///   <para>Button 3 on second joystick.</para>
		/// </summary>
		Joystick2Button3,
		/// <summary>
		///   <para>Button 4 on second joystick.</para>
		/// </summary>
		Joystick2Button4,
		/// <summary>
		///   <para>Button 5 on second joystick.</para>
		/// </summary>
		Joystick2Button5,
		/// <summary>
		///   <para>Button 6 on second joystick.</para>
		/// </summary>
		Joystick2Button6,
		/// <summary>
		///   <para>Button 7 on second joystick.</para>
		/// </summary>
		Joystick2Button7,
		/// <summary>
		///   <para>Button 8 on second joystick.</para>
		/// </summary>
		Joystick2Button8,
		/// <summary>
		///   <para>Button 9 on second joystick.</para>
		/// </summary>
		Joystick2Button9,
		/// <summary>
		///   <para>Button 10 on second joystick.</para>
		/// </summary>
		Joystick2Button10,
		/// <summary>
		///   <para>Button 11 on second joystick.</para>
		/// </summary>
		Joystick2Button11,
		/// <summary>
		///   <para>Button 12 on second joystick.</para>
		/// </summary>
		Joystick2Button12,
		/// <summary>
		///   <para>Button 13 on second joystick.</para>
		/// </summary>
		Joystick2Button13,
		/// <summary>
		///   <para>Button 14 on second joystick.</para>
		/// </summary>
		Joystick2Button14,
		/// <summary>
		///   <para>Button 15 on second joystick.</para>
		/// </summary>
		Joystick2Button15,
		/// <summary>
		///   <para>Button 16 on second joystick.</para>
		/// </summary>
		Joystick2Button16,
		/// <summary>
		///   <para>Button 17 on second joystick.</para>
		/// </summary>
		Joystick2Button17,
		/// <summary>
		///   <para>Button 18 on second joystick.</para>
		/// </summary>
		Joystick2Button18,
		/// <summary>
		///   <para>Button 19 on second joystick.</para>
		/// </summary>
		Joystick2Button19,
		/// <summary>
		///   <para>Button 0 on third joystick.</para>
		/// </summary>
		Joystick3Button0,
		/// <summary>
		///   <para>Button 1 on third joystick.</para>
		/// </summary>
		Joystick3Button1,
		/// <summary>
		///   <para>Button 2 on third joystick.</para>
		/// </summary>
		Joystick3Button2,
		/// <summary>
		///   <para>Button 3 on third joystick.</para>
		/// </summary>
		Joystick3Button3,
		/// <summary>
		///   <para>Button 4 on third joystick.</para>
		/// </summary>
		Joystick3Button4,
		/// <summary>
		///   <para>Button 5 on third joystick.</para>
		/// </summary>
		Joystick3Button5,
		/// <summary>
		///   <para>Button 6 on third joystick.</para>
		/// </summary>
		Joystick3Button6,
		/// <summary>
		///   <para>Button 7 on third joystick.</para>
		/// </summary>
		Joystick3Button7,
		/// <summary>
		///   <para>Button 8 on third joystick.</para>
		/// </summary>
		Joystick3Button8,
		/// <summary>
		///   <para>Button 9 on third joystick.</para>
		/// </summary>
		Joystick3Button9,
		/// <summary>
		///   <para>Button 10 on third joystick.</para>
		/// </summary>
		Joystick3Button10,
		/// <summary>
		///   <para>Button 11 on third joystick.</para>
		/// </summary>
		Joystick3Button11,
		/// <summary>
		///   <para>Button 12 on third joystick.</para>
		/// </summary>
		Joystick3Button12,
		/// <summary>
		///   <para>Button 13 on third joystick.</para>
		/// </summary>
		Joystick3Button13,
		/// <summary>
		///   <para>Button 14 on third joystick.</para>
		/// </summary>
		Joystick3Button14,
		/// <summary>
		///   <para>Button 15 on third joystick.</para>
		/// </summary>
		Joystick3Button15,
		/// <summary>
		///   <para>Button 16 on third joystick.</para>
		/// </summary>
		Joystick3Button16,
		/// <summary>
		///   <para>Button 17 on third joystick.</para>
		/// </summary>
		Joystick3Button17,
		/// <summary>
		///   <para>Button 18 on third joystick.</para>
		/// </summary>
		Joystick3Button18,
		/// <summary>
		///   <para>Button 19 on third joystick.</para>
		/// </summary>
		Joystick3Button19,
		/// <summary>
		///   <para>Button 0 on forth joystick.</para>
		/// </summary>
		Joystick4Button0,
		/// <summary>
		///   <para>Button 1 on forth joystick.</para>
		/// </summary>
		Joystick4Button1,
		/// <summary>
		///   <para>Button 2 on forth joystick.</para>
		/// </summary>
		Joystick4Button2,
		/// <summary>
		///   <para>Button 3 on forth joystick.</para>
		/// </summary>
		Joystick4Button3,
		/// <summary>
		///   <para>Button 4 on forth joystick.</para>
		/// </summary>
		Joystick4Button4,
		/// <summary>
		///   <para>Button 5 on forth joystick.</para>
		/// </summary>
		Joystick4Button5,
		/// <summary>
		///   <para>Button 6 on forth joystick.</para>
		/// </summary>
		Joystick4Button6,
		/// <summary>
		///   <para>Button 7 on forth joystick.</para>
		/// </summary>
		Joystick4Button7,
		/// <summary>
		///   <para>Button 8 on forth joystick.</para>
		/// </summary>
		Joystick4Button8,
		/// <summary>
		///   <para>Button 9 on forth joystick.</para>
		/// </summary>
		Joystick4Button9,
		/// <summary>
		///   <para>Button 10 on forth joystick.</para>
		/// </summary>
		Joystick4Button10,
		/// <summary>
		///   <para>Button 11 on forth joystick.</para>
		/// </summary>
		Joystick4Button11,
		/// <summary>
		///   <para>Button 12 on forth joystick.</para>
		/// </summary>
		Joystick4Button12,
		/// <summary>
		///   <para>Button 13 on forth joystick.</para>
		/// </summary>
		Joystick4Button13,
		/// <summary>
		///   <para>Button 14 on forth joystick.</para>
		/// </summary>
		Joystick4Button14,
		/// <summary>
		///   <para>Button 15 on forth joystick.</para>
		/// </summary>
		Joystick4Button15,
		/// <summary>
		///   <para>Button 16 on forth joystick.</para>
		/// </summary>
		Joystick4Button16,
		/// <summary>
		///   <para>Button 17 on forth joystick.</para>
		/// </summary>
		Joystick4Button17,
		/// <summary>
		///   <para>Button 18 on forth joystick.</para>
		/// </summary>
		Joystick4Button18,
		/// <summary>
		///   <para>Button 19 on forth joystick.</para>
		/// </summary>
		Joystick4Button19,
		/// <summary>
		///   <para>Button 0 on fifth joystick.</para>
		/// </summary>
		Joystick5Button0,
		/// <summary>
		///   <para>Button 1 on fifth joystick.</para>
		/// </summary>
		Joystick5Button1,
		/// <summary>
		///   <para>Button 2 on fifth joystick.</para>
		/// </summary>
		Joystick5Button2,
		/// <summary>
		///   <para>Button 3 on fifth joystick.</para>
		/// </summary>
		Joystick5Button3,
		/// <summary>
		///   <para>Button 4 on fifth joystick.</para>
		/// </summary>
		Joystick5Button4,
		/// <summary>
		///   <para>Button 5 on fifth joystick.</para>
		/// </summary>
		Joystick5Button5,
		/// <summary>
		///   <para>Button 6 on fifth joystick.</para>
		/// </summary>
		Joystick5Button6,
		/// <summary>
		///   <para>Button 7 on fifth joystick.</para>
		/// </summary>
		Joystick5Button7,
		/// <summary>
		///   <para>Button 8 on fifth joystick.</para>
		/// </summary>
		Joystick5Button8,
		/// <summary>
		///   <para>Button 9 on fifth joystick.</para>
		/// </summary>
		Joystick5Button9,
		/// <summary>
		///   <para>Button 10 on fifth joystick.</para>
		/// </summary>
		Joystick5Button10,
		/// <summary>
		///   <para>Button 11 on fifth joystick.</para>
		/// </summary>
		Joystick5Button11,
		/// <summary>
		///   <para>Button 12 on fifth joystick.</para>
		/// </summary>
		Joystick5Button12,
		/// <summary>
		///   <para>Button 13 on fifth joystick.</para>
		/// </summary>
		Joystick5Button13,
		/// <summary>
		///   <para>Button 14 on fifth joystick.</para>
		/// </summary>
		Joystick5Button14,
		/// <summary>
		///   <para>Button 15 on fifth joystick.</para>
		/// </summary>
		Joystick5Button15,
		/// <summary>
		///   <para>Button 16 on fifth joystick.</para>
		/// </summary>
		Joystick5Button16,
		/// <summary>
		///   <para>Button 17 on fifth joystick.</para>
		/// </summary>
		Joystick5Button17,
		/// <summary>
		///   <para>Button 18 on fifth joystick.</para>
		/// </summary>
		Joystick5Button18,
		/// <summary>
		///   <para>Button 19 on fifth joystick.</para>
		/// </summary>
		Joystick5Button19,
		/// <summary>
		///   <para>Button 0 on sixth joystick.</para>
		/// </summary>
		Joystick6Button0,
		/// <summary>
		///   <para>Button 1 on sixth joystick.</para>
		/// </summary>
		Joystick6Button1,
		/// <summary>
		///   <para>Button 2 on sixth joystick.</para>
		/// </summary>
		Joystick6Button2,
		/// <summary>
		///   <para>Button 3 on sixth joystick.</para>
		/// </summary>
		Joystick6Button3,
		/// <summary>
		///   <para>Button 4 on sixth joystick.</para>
		/// </summary>
		Joystick6Button4,
		/// <summary>
		///   <para>Button 5 on sixth joystick.</para>
		/// </summary>
		Joystick6Button5,
		/// <summary>
		///   <para>Button 6 on sixth joystick.</para>
		/// </summary>
		Joystick6Button6,
		/// <summary>
		///   <para>Button 7 on sixth joystick.</para>
		/// </summary>
		Joystick6Button7,
		/// <summary>
		///   <para>Button 8 on sixth joystick.</para>
		/// </summary>
		Joystick6Button8,
		/// <summary>
		///   <para>Button 9 on sixth joystick.</para>
		/// </summary>
		Joystick6Button9,
		/// <summary>
		///   <para>Button 10 on sixth joystick.</para>
		/// </summary>
		Joystick6Button10,
		/// <summary>
		///   <para>Button 11 on sixth joystick.</para>
		/// </summary>
		Joystick6Button11,
		/// <summary>
		///   <para>Button 12 on sixth joystick.</para>
		/// </summary>
		Joystick6Button12,
		/// <summary>
		///   <para>Button 13 on sixth joystick.</para>
		/// </summary>
		Joystick6Button13,
		/// <summary>
		///   <para>Button 14 on sixth joystick.</para>
		/// </summary>
		Joystick6Button14,
		/// <summary>
		///   <para>Button 15 on sixth joystick.</para>
		/// </summary>
		Joystick6Button15,
		/// <summary>
		///   <para>Button 16 on sixth joystick.</para>
		/// </summary>
		Joystick6Button16,
		/// <summary>
		///   <para>Button 17 on sixth joystick.</para>
		/// </summary>
		Joystick6Button17,
		/// <summary>
		///   <para>Button 18 on sixth joystick.</para>
		/// </summary>
		Joystick6Button18,
		/// <summary>
		///   <para>Button 19 on sixth joystick.</para>
		/// </summary>
		Joystick6Button19,
		/// <summary>
		///   <para>Button 0 on seventh joystick.</para>
		/// </summary>
		Joystick7Button0,
		/// <summary>
		///   <para>Button 1 on seventh joystick.</para>
		/// </summary>
		Joystick7Button1,
		/// <summary>
		///   <para>Button 2 on seventh joystick.</para>
		/// </summary>
		Joystick7Button2,
		/// <summary>
		///   <para>Button 3 on seventh joystick.</para>
		/// </summary>
		Joystick7Button3,
		/// <summary>
		///   <para>Button 4 on seventh joystick.</para>
		/// </summary>
		Joystick7Button4,
		/// <summary>
		///   <para>Button 5 on seventh joystick.</para>
		/// </summary>
		Joystick7Button5,
		/// <summary>
		///   <para>Button 6 on seventh joystick.</para>
		/// </summary>
		Joystick7Button6,
		/// <summary>
		///   <para>Button 7 on seventh joystick.</para>
		/// </summary>
		Joystick7Button7,
		/// <summary>
		///   <para>Button 8 on seventh joystick.</para>
		/// </summary>
		Joystick7Button8,
		/// <summary>
		///   <para>Button 9 on seventh joystick.</para>
		/// </summary>
		Joystick7Button9,
		/// <summary>
		///   <para>Button 10 on seventh joystick.</para>
		/// </summary>
		Joystick7Button10,
		/// <summary>
		///   <para>Button 11 on seventh joystick.</para>
		/// </summary>
		Joystick7Button11,
		/// <summary>
		///   <para>Button 12 on seventh joystick.</para>
		/// </summary>
		Joystick7Button12,
		/// <summary>
		///   <para>Button 13 on seventh joystick.</para>
		/// </summary>
		Joystick7Button13,
		/// <summary>
		///   <para>Button 14 on seventh joystick.</para>
		/// </summary>
		Joystick7Button14,
		/// <summary>
		///   <para>Button 15 on seventh joystick.</para>
		/// </summary>
		Joystick7Button15,
		/// <summary>
		///   <para>Button 16 on seventh joystick.</para>
		/// </summary>
		Joystick7Button16,
		/// <summary>
		///   <para>Button 17 on seventh joystick.</para>
		/// </summary>
		Joystick7Button17,
		/// <summary>
		///   <para>Button 18 on seventh joystick.</para>
		/// </summary>
		Joystick7Button18,
		/// <summary>
		///   <para>Button 19 on seventh joystick.</para>
		/// </summary>
		Joystick7Button19,
		/// <summary>
		///   <para>Button 0 on eighth joystick.</para>
		/// </summary>
		Joystick8Button0,
		/// <summary>
		///   <para>Button 1 on eighth joystick.</para>
		/// </summary>
		Joystick8Button1,
		/// <summary>
		///   <para>Button 2 on eighth joystick.</para>
		/// </summary>
		Joystick8Button2,
		/// <summary>
		///   <para>Button 3 on eighth joystick.</para>
		/// </summary>
		Joystick8Button3,
		/// <summary>
		///   <para>Button 4 on eighth joystick.</para>
		/// </summary>
		Joystick8Button4,
		/// <summary>
		///   <para>Button 5 on eighth joystick.</para>
		/// </summary>
		Joystick8Button5,
		/// <summary>
		///   <para>Button 6 on eighth joystick.</para>
		/// </summary>
		Joystick8Button6,
		/// <summary>
		///   <para>Button 7 on eighth joystick.</para>
		/// </summary>
		Joystick8Button7,
		/// <summary>
		///   <para>Button 8 on eighth joystick.</para>
		/// </summary>
		Joystick8Button8,
		/// <summary>
		///   <para>Button 9 on eighth joystick.</para>
		/// </summary>
		Joystick8Button9,
		/// <summary>
		///   <para>Button 10 on eighth joystick.</para>
		/// </summary>
		Joystick8Button10,
		/// <summary>
		///   <para>Button 11 on eighth joystick.</para>
		/// </summary>
		Joystick8Button11,
		/// <summary>
		///   <para>Button 12 on eighth joystick.</para>
		/// </summary>
		Joystick8Button12,
		/// <summary>
		///   <para>Button 13 on eighth joystick.</para>
		/// </summary>
		Joystick8Button13,
		/// <summary>
		///   <para>Button 14 on eighth joystick.</para>
		/// </summary>
		Joystick8Button14,
		/// <summary>
		///   <para>Button 15 on eighth joystick.</para>
		/// </summary>
		Joystick8Button15,
		/// <summary>
		///   <para>Button 16 on eighth joystick.</para>
		/// </summary>
		Joystick8Button16,
		/// <summary>
		///   <para>Button 17 on eighth joystick.</para>
		/// </summary>
		Joystick8Button17,
		/// <summary>
		///   <para>Button 18 on eighth joystick.</para>
		/// </summary>
		Joystick8Button18,
		/// <summary>
		///   <para>Button 19 on eighth joystick.</para>
		/// </summary>
		Joystick8Button19
	};
}
