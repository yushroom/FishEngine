#include <FishGUI/FishGUI.hpp>
//#define GLEW_STATIC
//#include <gl/glew.h>
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>
#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>

#include "nanovg.h"
#define NANOVG_GL3_IMPLEMENTATION
#include "nanovg_gl.h"

#include <vector>
#include <map>
#include <stack>
#include <cassert>
#include <iostream>
#include <sstream>
#include <locale>
#include <codecvt>

#include <thread>
#include <chrono>
using namespace std::chrono_literals;

#define ICON_SEARCH 0x1F50D
#define ICON_CIRCLED_CROSS 0x2716
#define ICON_CHEVRON_RIGHT 0xE75E
#define ICON_CHECK 0x2713
#define ICON_LOGIN 0xE740
#define ICON_TRASH 0xE729


void _checkOpenGLError(const char *file, int line)
{
	GLenum err(glGetError());
	
	while (err != GL_NO_ERROR)
	{
		std::string error;
		
		switch (err)
		{
			case GL_INVALID_OPERATION:      error = "INVALID_OPERATION";      break;
			case GL_INVALID_ENUM:           error = "INVALID_ENUM";           break;
			case GL_INVALID_VALUE:          error = "INVALID_VALUE";          break;
			case GL_OUT_OF_MEMORY:          error = "OUT_OF_MEMORY";          break;
			case GL_INVALID_FRAMEBUFFER_OPERATION:  error = "INVALID_FRAMEBUFFER_OPERATION";  break;
			default:                        error = "UNKNOWN_ERROR";
		}
		
//		LogError(Format("GL_%1% - %2%:%3%", error, file, line));
		printf("GL_%s - %s:%d\n", error.c_str(), file, line);
		err = glGetError();
	}
}

#define glCheckError() _checkOpenGLError(__FILE__, __LINE__)

template<typename T>
inline std::string StringCast (T const & t)
{
	std::ostringstream sout;
	sout << t;
	return sout.str();
}

// Returns 1 if col.rgba is 0.0f,0.0f,0.0f,0.0f, 0 otherwise
int isBlack(NVGcolor col)
{
	if( col.r == 0.0f && col.g == 0.0f && col.b == 0.0f && col.a == 0.0f )
	{
		return 1;
	}
	return 0;
}

static char* cpToUTF8(int cp, char* str)
{
	int n = 0;
	if (cp < 0x80) n = 1;
	else if (cp < 0x800) n = 2;
	
	else if (cp < 0x10000) n = 3;
	else if (cp < 0x200000) n = 4;
	else if (cp < 0x4000000) n = 5;
	else if (cp <= 0x7fffffff) n = 6;
	str[n] = '\0';
	switch (n) {
		case 6: str[5] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0x4000000;
		case 5: str[4] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0x200000;
		case 4: str[3] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0x10000;
		case 3: str[2] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0x800;
		case 2: str[1] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0xc0;
		case 1: str[0] = cp;
	}
	return str;
}

void drawWindow(NVGcontext* vg, const char* title, float x, float y, float w, float h)
{
	float cornerRadius = 3.0f;
	NVGpaint shadowPaint;
	NVGpaint headerPaint;
	
	nvgSave(vg);
	//	nvgClearState(vg);
	
	// Window
	nvgBeginPath(vg);
	nvgRoundedRect(vg, x,y, w,h, cornerRadius);
	nvgFillColor(vg, nvgRGBA(28,30,34,192));
	//	nvgFillColor(vg, nvgRGBA(0,0,0,128));
	nvgFill(vg);
	
	// Drop shadow
	shadowPaint = nvgBoxGradient(vg, x,y+2, w,h, cornerRadius*2, 10, nvgRGBA(0,0,0,128), nvgRGBA(0,0,0,0));
	nvgBeginPath(vg);
	nvgRect(vg, x-10,y-10, w+20,h+30);
	nvgRoundedRect(vg, x,y, w,h, cornerRadius);
	nvgPathWinding(vg, NVG_HOLE);
	nvgFillPaint(vg, shadowPaint);
	nvgFill(vg);
	
	if (title) {
		// Header
		headerPaint = nvgLinearGradient(vg, x,y,x,y+15, nvgRGBA(255,255,255,8), nvgRGBA(0,0,0,16));
		nvgBeginPath(vg);
		nvgRoundedRect(vg, x+1,y+1, w-2,30, cornerRadius-1);
		nvgFillPaint(vg, headerPaint);
		nvgFill(vg);
		nvgBeginPath(vg);
		nvgMoveTo(vg, x+0.5f, y+0.5f+30);
		nvgLineTo(vg, x+0.5f+w-1, y+0.5f+30);
		nvgStrokeColor(vg, nvgRGBA(0,0,0,32));
		nvgStroke(vg);
		
		nvgFontSize(vg, 18.0f);
		nvgFontFace(vg, "sans-bold");
		nvgTextAlign(vg,NVG_ALIGN_CENTER|NVG_ALIGN_MIDDLE);
		
		nvgFontBlur(vg,2);
		nvgFillColor(vg, nvgRGBA(0,0,0,128));
		nvgText(vg, x+w/2,y+16+1, title, NULL);
		
		nvgFontBlur(vg,0);
		nvgFillColor(vg, nvgRGBA(220,220,220,160));
		nvgText(vg, x+w/2,y+16, title, NULL);
	}
	
	nvgRestore(vg);
}


void drawSearchBox(NVGcontext* vg, const char* text, float x, float y, float w, float h)
{
	NVGpaint bg;
	char icon[8];
	float cornerRadius = h/2-1;
	
	// Edit
	bg = nvgBoxGradient(vg, x,y+1.5f, w,h, h/2,5, nvgRGBA(0,0,0,16), nvgRGBA(0,0,0,92));
	nvgBeginPath(vg);
	nvgRoundedRect(vg, x,y, w,h, cornerRadius);
	nvgFillPaint(vg, bg);
	nvgFill(vg);
	
	/*	nvgBeginPath(vg);
	 nvgRoundedRect(vg, x+0.5f,y+0.5f, w-1,h-1, cornerRadius-0.5f);
	 nvgStrokeColor(vg, nvgRGBA(0,0,0,48));
	 nvgStroke(vg);*/
	
	nvgFontSize(vg, h*1.3f);
	nvgFontFace(vg, "icons");
	nvgFillColor(vg, nvgRGBA(255,255,255,64));
	nvgTextAlign(vg,NVG_ALIGN_CENTER|NVG_ALIGN_MIDDLE);
	nvgText(vg, x+h*0.55f, y+h*0.55f, cpToUTF8(ICON_SEARCH,icon), NULL);
	
	nvgFontSize(vg, 20.0f);
	nvgFontFace(vg, "sans");
	nvgFillColor(vg, nvgRGBA(255,255,255,32));
	
	nvgTextAlign(vg,NVG_ALIGN_LEFT|NVG_ALIGN_MIDDLE);
	nvgText(vg, x+h*1.05f,y+h*0.5f,text, NULL);
	
	nvgFontSize(vg, h*1.3f);
	nvgFontFace(vg, "icons");
	nvgFillColor(vg, nvgRGBA(255,255,255,32));
	nvgTextAlign(vg,NVG_ALIGN_CENTER|NVG_ALIGN_MIDDLE);
	nvgText(vg, x+w-h*0.55f, y+h*0.55f, cpToUTF8(ICON_CIRCLED_CROSS,icon), NULL);
}

void drawDropDown(NVGcontext* vg, const char* text, float x, float y, float w, float h)
{
	NVGpaint bg;
	char icon[8];
	float cornerRadius = 4.0f;
	
	bg = nvgLinearGradient(vg, x,y,x,y+h, nvgRGBA(255,255,255,16), nvgRGBA(0,0,0,16));
	nvgBeginPath(vg);
	nvgRoundedRect(vg, x+1,y+1, w-2,h-2, cornerRadius-1);
	nvgFillPaint(vg, bg);
	nvgFill(vg);
	
	nvgBeginPath(vg);
	nvgRoundedRect(vg, x+0.5f,y+0.5f, w-1,h-1, cornerRadius-0.5f);
	nvgStrokeColor(vg, nvgRGBA(0,0,0,48));
	nvgStroke(vg);
	
	nvgFontSize(vg, 20.0f);
	nvgFontFace(vg, "sans");
	nvgFillColor(vg, nvgRGBA(255,255,255,160));
	nvgTextAlign(vg,NVG_ALIGN_LEFT|NVG_ALIGN_MIDDLE);
	nvgText(vg, x+h*0.3f,y+h*0.5f,text, NULL);
	
	nvgFontSize(vg, h*1.3f);
	nvgFontFace(vg, "icons");
	nvgFillColor(vg, nvgRGBA(255,255,255,64));
	nvgTextAlign(vg,NVG_ALIGN_CENTER|NVG_ALIGN_MIDDLE);
	nvgText(vg, x+w-h*0.5f, y+h*0.5f, cpToUTF8(ICON_CHEVRON_RIGHT,icon), NULL);
}

void drawLabel(NVGcontext* vg, const char* text, float x, float y, float w, float h)
{
	NVG_NOTUSED(w);
	
	nvgFontSize(vg, 18.0f);
	nvgFontFace(vg, "sans");
	nvgFillColor(vg, nvgRGBA(255,255,255,128));
	
	nvgTextAlign(vg,NVG_ALIGN_LEFT|NVG_ALIGN_MIDDLE);
	nvgText(vg, x,y+h*0.5f,text, NULL);
}

void drawEditBoxBase(NVGcontext* vg, float x, float y, float w, float h)
{
	NVGpaint bg;
	// Edit
	bg = nvgBoxGradient(vg, x+1,y+1+1.5f, w-2,h-2, 3,4, nvgRGBA(255,255,255,32), nvgRGBA(32,32,32,32));
	nvgBeginPath(vg);
	nvgRoundedRect(vg, x+1,y+1, w-2,h-2, 4-1);
	nvgFillPaint(vg, bg);
	nvgFill(vg);
	
	nvgBeginPath(vg);
	nvgRoundedRect(vg, x+0.5f,y+0.5f, w-1,h-1, 4-0.5f);
	nvgStrokeColor(vg, nvgRGBA(0,0,0,48));
	nvgStroke(vg);
}

void drawEditBox(NVGcontext* vg, const char* text, float x, float y, float w, float h)
{
	
	drawEditBoxBase(vg, x,y, w,h);
	
	nvgFontSize(vg, 16.0f);
	nvgFontFace(vg, "sans");
	nvgFillColor(vg, nvgRGBA(255,255,255,64));
	nvgTextAlign(vg,NVG_ALIGN_LEFT|NVG_ALIGN_MIDDLE);
	nvgText(vg, x+h*0.3f,y+h*0.5f,text, NULL);
}


void drawCheckBox(NVGcontext* vg, const char* text, float x, float y, float w, float h, bool enabled)
{
	NVGpaint bg;
	char icon[8];
	NVG_NOTUSED(w);
	
	nvgFontSize(vg, 18.0f);
	nvgFontFace(vg, "sans");
	nvgFillColor(vg, nvgRGBA(255,255,255,160));
	
	nvgTextAlign(vg,NVG_ALIGN_LEFT|NVG_ALIGN_MIDDLE);
	nvgText(vg, x+28,y+h*0.5f,text, NULL);
	
	bg = nvgBoxGradient(vg, x+1,y+(int)(h*0.5f)-9+1, 18,18, 3,3, nvgRGBA(0,0,0,32), nvgRGBA(0,0,0,92));
	nvgBeginPath(vg);
	nvgRoundedRect(vg, x+1,y+(int)(h*0.5f)-9, 18,18, 3);
	nvgFillPaint(vg, bg);
	nvgFill(vg);
	
	if (enabled)
	{
		nvgFontSize(vg, 40);
		nvgFontFace(vg, "icons");
		nvgFillColor(vg, nvgRGBA(255,255,255,128));
		nvgTextAlign(vg,NVG_ALIGN_CENTER|NVG_ALIGN_MIDDLE);
		nvgText(vg, x+9+2, y+h*0.5f, cpToUTF8(ICON_CHECK,icon), NULL);
	}
}


void drawButton(NVGcontext* vg, int preicon, const char* text, float x, float y, float w, float h, NVGcolor colTop, NVGcolor colBot)
{
	NVGpaint bg;
	char icon[8];
	float cornerRadius = 4.0f;
	float tw = 0, iw = 0;
	
//	bg = nvgLinearGradient(vg, x,y,x,y+h, nvgRGBA(255,255,255,isBlack(col)?16:32), nvgRGBA(0,0,0,isBlack(col)?16:32));
	bg = nvgLinearGradient(vg, x,y,x,y+h, colTop, colBot);
	nvgBeginPath(vg);
	nvgRoundedRect(vg, x+1,y+1, w-2,h-2, cornerRadius-1);
//	if (!isBlack(col)) {
//		nvgFillColor(vg, col);
//		nvgFill(vg);
//	}
	nvgFillPaint(vg, bg);
	nvgFill(vg);
	
	nvgBeginPath(vg);
	nvgRoundedRect(vg, x+0.5f,y+0.5f, w-1,h-1, cornerRadius-0.5f);
	nvgStrokeColor(vg, nvgRGBA(0,0,0,48));
	nvgStroke(vg);
	
	nvgFontSize(vg, 20.0f);
	nvgFontFace(vg, "sans-bold");
	tw = nvgTextBounds(vg, 0,0, text, NULL, NULL);
	if (preicon != 0) {
		nvgFontSize(vg, h*1.3f);
		nvgFontFace(vg, "icons");
		iw = nvgTextBounds(vg, 0,0, cpToUTF8(preicon,icon), NULL, NULL);
		iw += h*0.15f;
	}
	
	if (preicon != 0) {
		nvgFontSize(vg, h*1.3f);
		nvgFontFace(vg, "icons");
		nvgFillColor(vg, nvgRGBA(255,255,255,96));
		nvgTextAlign(vg,NVG_ALIGN_LEFT|NVG_ALIGN_MIDDLE);
		nvgText(vg, x+w*0.5f-tw*0.5f-iw*0.75f, y+h*0.5f, cpToUTF8(preicon,icon), NULL);
	}
	
	nvgFontSize(vg, 20.0f);
	nvgFontFace(vg, "sans-bold");
	nvgTextAlign(vg,NVG_ALIGN_LEFT|NVG_ALIGN_MIDDLE);
	nvgFillColor(vg, nvgRGBA(0,0,0,160));
	nvgText(vg, x+w*0.5f-tw*0.5f+iw*0.25f,y+h*0.5f-1,text, NULL);
	nvgFillColor(vg, nvgRGBA(255,255,255,160));
	nvgText(vg, x+w*0.5f-tw*0.5f+iw*0.25f,y+h*0.5f,text, NULL);
}

void drawSlider(NVGcontext* vg, float pos, float x, float y, float w, float h)
{
	NVGpaint bg, knob;
	float cy = y+(int)(h*0.5f);
	float kr = (int)(h*0.25f);
	
	nvgSave(vg);
	//	nvgClearState(vg);
	
	// Slot
	bg = nvgBoxGradient(vg, x,cy-2+1, w,4, 2,2, nvgRGBA(0,0,0,32), nvgRGBA(0,0,0,128));
	nvgBeginPath(vg);
	nvgRoundedRect(vg, x,cy-2, w,4, 2);
	nvgFillPaint(vg, bg);
	nvgFill(vg);
	
	// Knob Shadow
	bg = nvgRadialGradient(vg, x+(int)(pos*w),cy+1, kr-3,kr+3, nvgRGBA(0,0,0,64), nvgRGBA(0,0,0,0));
	nvgBeginPath(vg);
	nvgRect(vg, x+(int)(pos*w)-kr-5,cy-kr-5,kr*2+5+5,kr*2+5+5+3);
	nvgCircle(vg, x+(int)(pos*w),cy, kr);
	nvgPathWinding(vg, NVG_HOLE);
	nvgFillPaint(vg, bg);
	nvgFill(vg);
	
	// Knob
	knob = nvgLinearGradient(vg, x,cy-kr,x,cy+kr, nvgRGBA(255,255,255,16), nvgRGBA(0,0,0,16));
	nvgBeginPath(vg);
	nvgCircle(vg, x+(int)(pos*w),cy, kr-1);
	nvgFillColor(vg, nvgRGBA(40,43,48,255));
	nvgFill(vg);
	nvgFillPaint(vg, knob);
	nvgFill(vg);
	
	nvgBeginPath(vg);
	nvgCircle(vg, x+(int)(pos*w),cy, kr-0.5f);
	nvgStrokeColor(vg, nvgRGBA(0,0,0,92));
	nvgStroke(vg);
	
	nvgRestore(vg);
}


namespace FishGUI {
	
	enum class KeyState {
		None = 0,
		Down = 1,
		Held = 2,
		Up = 3,
	};
	
	enum class MouseButtonState {
		None = 0,
		Down = 1,
		Held = 2,
		Up = 3,
	};
	
	enum class MouseButton {
		Left = 0,
		Right = 1,
		Middle = 2,
	};
	
	
	struct Input
	{
		void Init()
		{
			Clear();
		}
		
		void Clear()
		{
			for (auto& s : m_mouseButtonStates) {
				s = MouseButtonState::None;
			}
			m_inputMode = false;
			m_stringBuffer.clear();
			m_mousePosition.x = 0;
			m_mousePosition.y = 0;
			m_leftMouseButtonDoubleClicked = false;
			
			for (int i = 0; i < 3; ++i) {
				m_lastMouseButtonClickTime[i] = 0;
			}
		}
		
		void Update()
		{
			m_leftMouseButtonDoubleClicked = false;
			for (auto& s : m_mouseButtonStates) {
				if (s == MouseButtonState::Down)
					s = MouseButtonState::Held;
				else if (s == MouseButtonState::Up)
					s = MouseButtonState::None;
			}
		}
		
		// Returns whether the given mouse button is held down.
		// button values are 0 for left button, 1 for right button, 2 for the middle button.
		bool GetMouseButton(MouseButton button)
		{
			return m_mouseButtonStates[static_cast<int>(button)] == MouseButtonState::Held;
		}
		
		// Returns true during the frame the user pressed the given mouse button.
		// button values are 0 for left button, 1 for right button, 2 for the middle button.
		bool GetMouseButtonDown(MouseButton button)
		{
			return m_mouseButtonStates[static_cast<int>(button)] == MouseButtonState::Down;
		}
		
		// Returns true during the frame the user releases the given mouse button.
		// button values are 0 for left button, 1 for right button, 2 for the middle button.
		bool GetMouseButtonUp(MouseButton button)
		{
			return m_mouseButtonStates[static_cast<int>(button)] == MouseButtonState::Up;
		}
		
		MouseButtonState GetMouseButtonState(MouseButton button)
		{
			return m_mouseButtonStates[static_cast<int>(button)];
		}
		
		
		
		MouseButtonState	m_mouseButtonStates[3];
		Vector2				m_mousePosition = {0, 0};
		bool				m_inputMode = false;
		std::string			m_stringBuffer;
		
		// time record for double-click check
		double				m_lastMouseButtonClickTime[3] = {0, 0, 0};
		
		// TODO
		bool				m_leftMouseButtonDoubleClicked = false;
	};

	
	constexpr NVGcolor Color(int intensity, int alpha)
	{
//		NVGcolor c;
//		c.r = c.g = c.b = intensity/255.0f;
//		c.a = alpha / 255.0f;
//		return c;
		return NVGcolor{{{intensity/255.0f, intensity/255.0f, intensity/255.0f, alpha/255.0f}}};
	}
	
	struct Theme {
		int mStandardFontSize                 = 16;
		int mButtonFontSize                   = 20;
		int mTextBoxFontSize                  = 20;
		int mWindowCornerRadius               = 2;
		int mWindowHeaderHeight               = 30;
		int mWindowDropShadowSize             = 10;
		int mButtonCornerRadius               = 2;
		float mTabBorderWidth                 = 0.75f;
		int mTabInnerMargin                   = 5;
		int mTabMinButtonWidth                = 20;
		int mTabMaxButtonWidth                = 160;
		int mTabControlWidth                  = 20;
		int mTabButtonHorizontalPadding       = 10;
		int mTabButtonVerticalPadding         = 2;
		
		NVGcolor mDropShadow					   = Color(0, 128);
		NVGcolor mTransparent                      = Color(0, 0);
		NVGcolor mBorderDark                       = Color(29, 255);
		NVGcolor mBorderLight                      = Color(92, 255);
		NVGcolor mBorderMedium                     = Color(35, 255);
		NVGcolor mTextColor                        = Color(255, 160);
		NVGcolor mDisabledTextColor                = Color(255, 80);
		NVGcolor mTextColorShadow                  = Color(0, 160);
		NVGcolor mIconColor                        = mTextColor;
		
		NVGcolor mButtonGradientTopFocused         = Color(64, 255);
		NVGcolor mButtonGradientBotFocused         = Color(48, 255);
		NVGcolor mButtonGradientTopUnfocused       = Color(74, 255);
		NVGcolor mButtonGradientBotUnfocused       = Color(58, 255);
		NVGcolor mButtonGradientTopPushed          = Color(41, 255);
		NVGcolor mButtonGradientBotPushed          = Color(29, 255);
		
		/* Window-related */
		NVGcolor mWindowFillUnfocused              = Color(43, 230);
		NVGcolor mWindowFillFocused                = Color(45, 230);
		NVGcolor mWindowTitleUnfocused             = Color(220, 160);
		NVGcolor mWindowTitleFocused               = Color(255, 190);
		
		NVGcolor mWindowHeaderGradientTop          = mButtonGradientTopUnfocused;
		NVGcolor mWindowHeaderGradientBot          = mButtonGradientBotUnfocused;
		NVGcolor mWindowHeaderSepTop               = mBorderLight;
		NVGcolor mWindowHeaderSepBot               = mBorderDark;
		
		NVGcolor mWindowPopup                      = Color(50, 255);
		NVGcolor mWindowPopupTransparent           = Color(50, 0);
	};

	
	struct CenterScreen
	{
		int x = 0;
		int y = 0;
		int w = 1;
		int h = 1;
		
		void Reset(int width, int height)
		{
			x = 0;
			y = 0;
			w = width;
			h = height;
		}
		
		void AddTab(Tab* tab)
		{
			auto tabPosition = tab->m_position;
			auto & size = tab->m_size;
			auto rect = & tab->m_rect;
			if (tabPosition == TabPosition::Left) {
//				assert(widthOrHeight <= w);
				size = std::min(size, w);
				rect->x = x;
				rect->y = y;
				rect->width = size;
				rect->height = h;
				x += size;
				w -= size;
			} else if (tabPosition == TabPosition::Right) {
//				assert(widthOrHeight <= w);
				size = std::min(size, w);
				rect->x = x+w-size;
				rect->y = y;
				rect->width = size;
				rect->height = h;
				w -= size;
			} else if (tabPosition == TabPosition::Top) {
//				assert(widthOrHeight <= h);
				size = std::min(size, h);
				rect->x = x;
				rect->y = y;
				rect->width = w;
				rect->height = size;
				y += size;
				h -= size;
			} else if (tabPosition == TabPosition::Bottom) {
//				assert(widthOrHeight <= h);
				size = std::min(size, h);
				rect->x = x;
				rect->y = y+h-size;
				rect->width = w;
				rect->height = size;
				h -= size;
			} else { // floating
				rect->x = x;
				rect->y = y;
				rect->width = w;
				rect->height = h;
				w = 0;
				h = 0;
			}
		}
	};

	
	struct TabState
	{
		Rect rect;
		const int x_margin_left = 10;
		int x_margin_right = 10; // or 18
		const int y_margin = 5;
		const int y_cell_height = 20;
		int y_filled = 0;
		int cell_count = 0;
		
		void reset() {
			rect.x = 0;
			rect.y = 0;
			y_filled = 0;
			cell_count = 0;
		}
		
		void nextCellOrigin(int *px, int *py) {
			*px = rect.x + x_margin_left;
			*py = rect.y + y_filled + y_margin;
		}
		
		int get_avaliable_width() {
			return rect.width - x_margin_left - x_margin_right;
		}
		
		int get_current_cell_id() {
			return cell_count;
		}
		
		// cell height without margin
		void add_cell(const int height) {
			y_filled += height + y_margin;
			cell_count ++;
		}
	};
	
	struct Window
	{
		std::string			m_title;
		Rect				m_rect;
		Size				m_frameBufferSize;
		std::list<Tab*>		m_tabs;
		GLFWwindow*			m_glfwWindow;
		GLNVGvertexBuffers* m_buffers = nullptr;
		
		Rect				m_centerRect;
		
		// a window which is not focused may be rendered as normal, but will not receive input events
		bool				m_isFocused = false;
		
		~Window()
		{
			for (auto t : m_tabs)
				delete t;
			m_tabs.clear();
			
			// fixme : delete vertex buffer on GPU
			free(m_buffers);
		}
		
		void BeforeFrame()
		{
			assert(m_glfwWindow != nullptr);
			glfwGetWindowPos(m_glfwWindow, &m_rect.x, &m_rect.y);
			glfwGetWindowSize(m_glfwWindow, &m_rect.width, &m_rect.height);
			glfwGetFramebufferSize(m_glfwWindow, &m_frameBufferSize.width, &m_frameBufferSize.height);
			m_isFocused = (glfwGetWindowAttrib(m_glfwWindow, GLFW_FOCUSED) == 1);
		}
		
		inline int Width() const
		{
			return m_rect.width;
		}
		
		inline int Height() const
		{
			return m_rect.height;
		}
		
		float PixelRatio() const
		{
			return float(m_frameBufferSize.width) / m_rect.width;
		}
		
		void SetPosition(int x, int y)
		{
			assert(m_glfwWindow != nullptr);
			glfwSetWindowPos(m_glfwWindow, x, y);
			m_rect.x = x;
			m_rect.y = y;
		}
		
		void SetSize(int width, int height)
		{
			assert(m_glfwWindow != nullptr);
			glfwSetWindowSize(m_glfwWindow, width, height);
			m_rect.width = width;
			m_rect.height = height;
		}
		
		void SetTitle(const char* title)
		{
			assert(m_glfwWindow != nullptr);
			m_title = title;
			glfwSetWindowTitle(m_glfwWindow, title);
		}
		
		bool IsFocused() const
		{
//			return glfwGetWindowAttrib(m_window, GLFW_FOCUSED) == 1;
			return m_isFocused;
		}
		
		void AddTab(Tab * tab)
		{
			auto old_window = tab->m_window;
			if (old_window == this)
				return;
			
			if (old_window != nullptr)
				old_window->m_tabs.remove(tab);
			tab->m_window = this;
			m_tabs.push_back(tab);
		}
		
		void SetDecorated(bool decorated)
		{
			assert(m_glfwWindow != nullptr);
			int v = decorated ? GLFW_TRUE : GLFW_FALSE;
//			glfwSetWindowAttrib(m_glfwWindow, GLFW_DECORATED, v);
		}
	};
	
	struct WindowManager
	{
		Window* mainWindow() {
			return m_windows.front();
		}
		
		void CloseWindow(Window* window) {
			
		}
		
		Window* FocusedWindow() {
			for (auto w : m_windows) {
				if (w->IsFocused())
					return w;
			}
			return nullptr;
		}
		
		Window* FindWindow(GLFWwindow* glfwWindow) {
			for (auto w : m_windows) {
				if (w->m_glfwWindow == glfwWindow)
					return w;
			}
			return nullptr;
		}
		
		std::list<Window*> m_windows;
	};
	
	enum class CursorType {
		Arrow = 0,
		IBeam,
		Crosshair,
		Hand,
		HResize,
		VResize,
		// not a cursor
		CursorCount
	};
	
	struct Cursor {
		bool		m_cursorChanged = true;
		CursorType	m_cursorType = CursorType::Arrow;
		CursorType	m_cursorTypeLastFame = CursorType::Hand;
		GLFWcursor* m_glfwCursors[(int)CursorType::CursorCount];
		
		void SetCursorType(CursorType type) {
			m_cursorType = type;
		}

	};
	
	struct RenderContext
	{
		Window* window = nullptr;
		int width;
		int height;
	};

	
	struct Global {
		Theme			theme;
		RenderContext	renderContext;
		CenterScreen	centerScreen;
		Input			input;
		TabState		currentTab;
		WindowManager	windowManager;
		Cursor			cursor;
		NVGcontext *	nvgContext = nullptr;
	};
	
	static Global g;
	
	
	Vector2 MousePosition()
	{
		return g.renderContext.window->IsFocused() ? g.input.m_mousePosition : Vector2{0, 0};
	}
	
	bool IsMouseButtonDown(MouseButton button)
	{
		return g.renderContext.window->IsFocused() ? g.input.GetMouseButtonDown(button) : false;
	}
	
	bool IsMouseButtonPressed(MouseButton button)
	{
		return g.renderContext.window->IsFocused() ? g.input.GetMouseButton(button) : false;
	}
	
	bool IsMouseButtonUp(MouseButton button)
	{
		return g.renderContext.window->IsFocused() ? g.input.GetMouseButtonUp(button) : false;
	}
	
	bool IsMouseButtonDoubleClicked(MouseButton button)
	{
		return g.renderContext.window->IsFocused() ? g.input.GetMouseButtonDown(button) && g.input.m_leftMouseButtonDoubleClicked : false;
	}
	
	MouseButtonState GetMouseButtonState(MouseButton button)
	{
		return g.renderContext.window->IsFocused() ? g.input.m_mouseButtonStates[static_cast<int>(button)] : MouseButtonState::None;
	}
	
	
	void SetCursorType(CursorType type)
	{
		if (g.renderContext.window->IsFocused()) {
			g.cursor.SetCursorType(type);
		}
	}
	
	void RenderCursor()
	{
		if (g.cursor.m_cursorType != g.cursor.m_cursorTypeLastFame) {
			glfwSetCursor(g.windowManager.FocusedWindow()->m_glfwWindow, g.cursor.m_glfwCursors[static_cast<int>(g.cursor.m_cursorType)]);
			g.cursor.m_cursorTypeLastFame = g.cursor.m_cursorType;
		}
	}
	
    void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GL_TRUE);
    }
	
	void glfwCharCallback(GLFWwindow *window, unsigned int codepoint)
	{
		std::cout << codepoint << std::endl;
		static std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
		std::string u8str = converter.to_bytes(codepoint);
		std::cout << u8str << std::endl;
		g.input.m_stringBuffer += u8str;
	}
	
    void glfwMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
    {
		MouseButtonState state = MouseButtonState::None;
		if (action == GLFW_PRESS)
		{
			state = MouseButtonState::Down;
			double lastTime = g.input.m_lastMouseButtonClickTime[button];
			double now = glfwGetTime();
			if (now - lastTime < 0.3)
			{
				g.input.m_leftMouseButtonDoubleClicked = true;
				g.input.m_lastMouseButtonClickTime[button] = 0;
			}
			else
			{
				g.input.m_lastMouseButtonClickTime[button] = now;
			}
		}
		else if (action == GLFW_RELEASE)
		{
			state = MouseButtonState::Up;
		}
		
		g.input.m_mouseButtonStates[button] = state;
    }
	
	inline bool PointInRect(int px, int py, int x, int y, int w, int h)
	{
		return (px > x && px < x+w && py > y && py < y+h);
	}
	
	inline bool PointInRect(int px, int py, Rect const & rect)
	{
		return PointInRect(px, py, rect.x, rect.y, rect.width, rect.height);
	}
    
    inline bool MouseInRect(int x, int y, int w, int h)
	{
		auto mx = MousePosition().x;
        auto my = MousePosition().y;
		return PointInRect(mx, my, x, y, w, h);
    }


	void glfwBindWindowCallbacks(GLFWwindow* window)
	{
		glfwSetKeyCallback(window, glfwKeyCallback);
		glfwSetMouseButtonCallback(window, glfwMouseButtonCallback);
		glfwSetCharCallback(window, glfwCharCallback);
//		glfwSetWindowSizeCallback(window, glfwWindowResizeCallback);
//		glfwSetglfwWindowFocusCallback(window, glfwWindowFocusCallback);
	}

	
	GLFWwindow * CreateGLFWwindow(const char* title, int width, int height)
	{
		auto window = glfwCreateWindow(width, height, title, nullptr, g.windowManager.mainWindow()->m_glfwWindow);
		glfwBindWindowCallbacks(window);
//		glfwMakeContextCurrent(window);
//		glfwFocusWindow(window);
		return window;
	}
	
	Window* CreateWindow(GLFWwindow* glfwWindow)
	{
		glfwBindWindowCallbacks(glfwWindow);
		auto w = new Window;
		// TODO: title
		// w->m_title = title;
		w->m_glfwWindow = glfwWindow;
		g.windowManager.m_windows.push_back(w);
		return w;
	}
	
	Window* CreateWindow(std::string title, int width = 256, int height = 512)
	{
		auto glfwWindow = CreateGLFWwindow(title.c_str(), width, height);
		auto w =  CreateWindow(glfwWindow);
		w->m_title = title;
		return w;
	}


	Tab* CreateTab(const char* title, TabPosition tabPosition, int widthOrHeight)
	{
		Tab* tab = new Tab;
		tab->m_title = title;
		tab->m_position = tabPosition;
		if (tabPosition == TabPosition::Floating)
		{
			auto window = CreateWindow("Dialog", 256, 512);
			window->AddTab(tab);
		}
		else
		{
			g.windowManager.mainWindow()->m_tabs.push_back(tab);
			if (tabPosition == TabPosition::Left || tabPosition == TabPosition::Right)
			{
				tab->m_minimalSize = 128;
			} else {
				tab->m_minimalSize = 56;
			}
			tab->m_window = g.windowManager.mainWindow();
		}
		tab->m_position = tabPosition;
		tab->m_size = widthOrHeight;
		
		return tab;
	}

	
	void Init(GLFWwindow * window)
	{
		CreateWindow(window); // add main window
		
		g.nvgContext = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);
		
		nvgCreateFont(g.nvgContext, "icons", "/Users/yushroom/program/github/nanovg/example/entypo.ttf");
		nvgCreateFont(g.nvgContext, "sans", "/Users/yushroom/program/github/nanovg/example/Roboto-Regular.ttf");
		nvgCreateFont(g.nvgContext, "sans-bold", "/Users/yushroom/program/github/nanovg/example/Roboto-Bold.ttf");
		nvgCreateFont(g.nvgContext, "emoji", "/Users/yushroom/program/github/nanovg/example/NotoEmoji-Regular.ttf");
		
		for (int i = 0; i < static_cast<int>(CursorType::CursorCount); ++i) {
			g.cursor.m_glfwCursors[i] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR + i);
		}
	}
	
    void Begin()
	{
		for (auto& w : g.windowManager.m_windows)
			w->BeforeFrame();
		
		g.cursor.SetCursorType(CursorType::Arrow);
		
		// Focused window
		static Window* lastFocusedWindow = nullptr;
		auto focusedWindow = g.windowManager.FocusedWindow();
		
		if (lastFocusedWindow != focusedWindow) {
			printf("focused window changed!\n");
			lastFocusedWindow = focusedWindow;
			g.input.Clear();
		}
		
		if (focusedWindow != nullptr) {
			RenderWindows();
//			g.cursor.Render();
			RenderCursor();
		} else {
			std::this_thread::sleep_for(33.3ms);
		}
    }
    
    void End()
	{
		g.input.Update();
		
		glfwPollEvents();
		
		Window* windowToClose = nullptr; // better use a std::vector<Window*>
		
		for (auto & w : g.windowManager.m_windows) {
			if (glfwWindowShouldClose(w->m_glfwWindow) || w->m_tabs.empty()) {
				windowToClose = w;
			}
		}
		
		if (windowToClose != nullptr && windowToClose != g.windowManager.mainWindow()) {
			printf("close window[%s]\n", windowToClose->m_title.c_str());
			glfwDestroyWindow(windowToClose->m_glfwWindow);
			g.windowManager.m_windows.remove(windowToClose);
			delete windowToClose;
			
			glfwFocusWindow(g.windowManager.mainWindow()->m_glfwWindow);
		}
    }
	
	void RenderWindow(Window & w)
	{
		int iconified = glfwGetWindowAttrib(w.m_glfwWindow, GLFW_ICONIFIED);
		if (iconified)
			return;
		
		assert(w.m_glfwWindow != nullptr);
		glfwMakeContextCurrent(w.m_glfwWindow);
		g.renderContext.window = &w;
		
		double mx, my;
		glfwGetCursorPos(w.m_glfwWindow, &mx, &my);
		g.input.m_mousePosition.x = float(mx);
		g.input.m_mousePosition.y = float(my);
		
		int width = w.Width();
		int height = w.Height();
		int fbWidth = w.m_frameBufferSize.width;
		int fbHeight = w.m_frameBufferSize.height;
		
		g.renderContext.width = width;
		g.renderContext.height = height;
		
		g.centerScreen.Reset(width, height);
		
		GLNVGcontext * gl = (GLNVGcontext *)nvgInternalParams(g.nvgContext)->userPtr;
		if (w.m_buffers == nullptr)
		{
			w.m_buffers = glnvg__createVertexBuffers(gl);
		}
		gl->buffers = w.m_buffers;
		
		glViewport(0, 0, fbWidth, fbHeight);
		glClearColor(0.3f, 0.3f, 0.32f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		
		float ratio = float(fbWidth) / width;
		nvgBeginFrame(g.nvgContext, width, height, ratio);
		
		// determine rect of each tab
		if (w.m_tabs.size() == 1)
		{
			auto tab = w.m_tabs.front();
			tab->m_rect.x = tab->m_rect.y = 0;
			tab->m_rect.width = w.m_rect.width;
			tab->m_rect.height = w.m_rect.height;
		}
		else
		{
			for (auto& tab : w.m_tabs)
			{
				g.centerScreen.AddTab(tab);
			}
		}
		
		auto tabs = w.m_tabs;	// make a copy of tabs, since w.m_tabs may be changed during iteration(eg. docked to mainWindow)
		for (auto& tab : tabs)
		{
			BeginTab(tab);
			if (tab->m_renderFunction != nullptr)
				tab->m_renderFunction();
			EndTab();
		}
		
		w.m_centerRect.x = g.centerScreen.x;
		w.m_centerRect.y = g.centerScreen.y;
		w.m_centerRect.width = g.centerScreen.w;
		w.m_centerRect.height = g.centerScreen.h;
		
		nvgEndFrame(g.nvgContext);
		glCheckError();
		glfwSwapBuffers(w.m_glfwWindow);
	}
	
	void RenderWindows()
	{
		RenderWindow(*g.windowManager.mainWindow());

		for (auto& w : g.windowManager.m_windows)
		{
			RenderWindow(*w);
		}
	}
	
	void BeginTab(Tab * tab)
	{
		constexpr int tabTitleBarHeight = 20;
		
		g.currentTab.reset();
		auto tabPosition = tab->m_position;
		const char * title = tab->m_title.c_str();
		
		bool mouseHoverResizeBar = false;
		
		auto& r = g.currentTab.rect;
		g.currentTab.rect = tab->m_rect;
		
		// Dock
		
//		tab->m_window->SetDecorated(!tab->m_moving);

		if (tab->m_moving)
		{
			int x = tab->m_window->m_rect.x;
			int y = tab->m_window->m_rect.y;
			int w = tab->m_window->m_rect.width;
			int h = tab->m_window->m_rect.height;
			x += g.input.m_mousePosition.x;
			y += g.input.m_mousePosition.y;
			
			tab->m_window->SetPosition(x-w/2, y);
			
			// x, y is global mouse position
			Rect mainWindowRect = g.windowManager.mainWindow()->m_rect;
			if (PointInRect(x, y, mainWindowRect.x + mainWindowRect.width - 40, mainWindowRect.y, 40, mainWindowRect.height))
			{
//				printf("to be docked\n");
				if (IsMouseButtonUp(MouseButton::Left)) {
					printf("docked\n");
					tab->m_position = TabPosition::Right;
					tab->m_window->m_tabs.remove(tab);
					tab->m_window = g.windowManager.mainWindow();
					g.windowManager.mainWindow()->m_tabs.push_back(tab);
//					tab->m_moving = false;
				}
			}

			if (GetMouseButtonState(MouseButton::Left) != MouseButtonState::Held)
			{
				printf("dock end\n");
				tab->m_moving = false;
			}
		}
		else if (tab->m_window != g.windowManager.mainWindow())	// TODO: hack
		{
			if (IsMouseButtonDown(MouseButton::Left))
			{
				if (MouseInRect(r.x, r.y, r.width, tabTitleBarHeight))
				{
					printf("Tab Title bar Clicked\n");
					tab->m_moving = true;
				}
			}
		}
		
		
		if (tab->m_position != TabPosition::Floating && IsMouseButtonDoubleClicked(MouseButton::Left) && MouseInRect(r.x, r.y, r.width, tabTitleBarHeight))
		{
			printf("docked -> floating\n");
			
			tab->m_position = TabPosition::Floating;
			auto window = CreateWindow("New Tab");
			window->AddTab(tab);
			return;
		}
		
		// Resize
		
		if (tabPosition == TabPosition::Floating)
		{
			drawWindow(g.nvgContext, title, r.x, r.y, r.width, r.height);
			g.currentTab.y_filled += 20;
		}
		else
		{
			constexpr int margin = 5;
			const int x = MousePosition().x;
			const int y = MousePosition().y;
			if (tabPosition == TabPosition::Left)
			{
				if (tab->m_resizing)
				{
					if (IsMouseButtonPressed(MouseButton::Left))
					{
						int right = r.x + r.width;
						r.width += x - right;
						r.width = std::max(r.width, tab->m_minimalSize);
						tab->m_size = r.width;
					}
					else
					{
						tab->m_resizing = false;
					}
				}
				
				if (x > r.x+r.width-margin &&
					x < r.x+r.width+margin)
				{
					SetCursorType(CursorType::HResize);
					mouseHoverResizeBar = true;
				}
			}
			else if (tabPosition == TabPosition::Right)
			{
				if (tab->m_resizing)
				{
					if (IsMouseButtonPressed(MouseButton::Left))
					{
						int right = r.x + r.width;
						r.width += r.x - x;
						r.width = std::max(r.width, tab->m_minimalSize);
						r.x = right - r.width;
						tab->m_size = r.width;
					}
					else
					{
						tab->m_resizing = false;
					}
				}
				
				if (x > r.x-margin && x < r.x+margin)
				{
					SetCursorType(CursorType::HResize);
					mouseHoverResizeBar = true;
				}
			}
			else if (tabPosition == TabPosition::Top)
			{
				if (tab->m_resizing)
				{
					if (IsMouseButtonPressed(MouseButton::Left))
					{
						int bottom = r.y + r.height;
						r.height += y - bottom;
						r.height = std::max(r.height, tab->m_minimalSize);
						tab->m_size = r.height;
					}
					else
					{
						tab->m_resizing = false;
					}
				}

				if (y > r.y+r.height-margin &&
					y < r.y+r.height+margin)
				{
					SetCursorType(CursorType::VResize);
					mouseHoverResizeBar = true;
				}
			}
			else if (tabPosition == TabPosition::Bottom)
			{
				if (tab->m_resizing)
				{
					if (IsMouseButtonPressed(MouseButton::Left))
					{
						int old_bottom = r.y + r.height;
						// TODO
//						r.height += r.y - y;
//						r.height = std::max(r.height, tab->m_minimalSize);
//						r.y = old_bottom - r.height;
//						tab->m_size = r.height;
						
						int height = r.height +  r.y - y;
						height = std::max(height, tab->m_minimalSize);
						r.y = old_bottom - height;
						tab->m_size = height;
					}
					else
					{
						tab->m_resizing = false;
					}
				}
				
				if (y > r.y-margin && y < r.y+margin)
				{
					SetCursorType(CursorType::VResize);
					mouseHoverResizeBar = true;
				}
			}
			
			if (mouseHoverResizeBar && IsMouseButtonDown(MouseButton::Left))
			{
				tab->m_resizing = true;
			}
			
			drawWindow(g.nvgContext, title, r.x, r.y, r.width, r.height);
			g.currentTab.y_filled += tabTitleBarHeight;
		}
    }
	
	void EndTab()
	{
	}
    
    bool Button(const char* text)
	{
        int x, y, w, h;
        g.currentTab.nextCellOrigin(&x, &y);
        w = g.currentTab.get_avaliable_width();
        h = g.currentTab.y_cell_height;
		
        bool clicked = false;
//        int id = g.currentTab.get_current_cell_id();
        bool mouse_inside = MouseInRect(x, y, w, h);
		NVGcolor colorTop = g.theme.mButtonGradientTopUnfocused;
		NVGcolor colorBot = g.theme.mButtonGradientBotUnfocused;
		if (mouse_inside)
		{
			colorTop = g.theme.mButtonGradientTopFocused;
			colorBot = g.theme.mButtonGradientBotFocused;
			if (IsMouseButtonUp(MouseButton::Left)) {
				clicked = true;
			} else if (IsMouseButtonPressed(MouseButton::Left)) {
				colorTop = g.theme.mButtonGradientTopPushed;
				colorBot = g.theme.mButtonGradientBotPushed;
			}
		}
		drawButton(g.nvgContext, 0, text, x, y, w, h, colorTop, colorBot);

        g.currentTab.add_cell(h);
        return clicked;
    }
    
    void Label(const char* text, GUIAlignment alignment)
	{
        int x, y, w, h;
        g.currentTab.nextCellOrigin(&x, &y);
        w = g.currentTab.get_avaliable_width();
        h = g.currentTab.y_cell_height;
		drawLabel(g.nvgContext, text, x, y, w, h);
		
        g.currentTab.add_cell(h);
    }
	
	bool CheckBox(const char * label, bool* v)
	{
		int x, y, w, h;
		g.currentTab.nextCellOrigin(&x, &y);
		w = g.currentTab.get_avaliable_width();
		h = g.currentTab.y_cell_height;
		drawCheckBox(g.nvgContext, label, x, y, w, h, *v);
		g.currentTab.add_cell(h);
		return false;
	}
	
	bool InputText(const char* label, std::string& text)
	{
		int x, y, w, h;
		g.currentTab.nextCellOrigin(&x, &y);
		w = g.currentTab.get_avaliable_width();
		h = g.currentTab.y_cell_height;
		int label_width = w / 2 - g.currentTab.x_margin_left;
		drawLabel(g.nvgContext, label, x, y, label_width, h);
		x += label_width + g.currentTab.x_margin_left;
		w -= label_width + g.currentTab.x_margin_left;
		drawEditBox(g.nvgContext, text.c_str(), x, y, w, h);
		
		if (MouseInRect(x, y, w, h)) {
			SetCursorType(CursorType::IBeam);
			if (g.input.GetMouseButtonDown(MouseButton::Left)) {
//				std::cout << "here" << std::endl;
				g.input.m_inputMode = true;
			}
		}
		
		g.currentTab.add_cell(h);
		return false;
	}
	
	bool Slider(const char* label, float *value, float minValue, float maxValue)
	{
		int x, y, w, h;
		g.currentTab.nextCellOrigin(&x, &y);
		w = g.currentTab.get_avaliable_width();
		h = g.currentTab.y_cell_height;
		int label_width = w/2 - g.currentTab.x_margin_left;
		int box_width = 40;
		int slider_width = w - label_width - box_width - g.currentTab.x_margin_left*2;
		
		drawLabel(g.nvgContext, label, x, y, label_width, h);
		x += label_width + g.currentTab.x_margin_left;
		float pos = (*value - minValue) / (maxValue - minValue);
		drawSlider(g.nvgContext, pos, x, y, slider_width, h);
		
		x += slider_width + g.currentTab.x_margin_left;
//		assert(label_width + box_width + slider_width + g.currentTab.x_margin_left*2 == w);
		auto value_str = StringCast(*value);
		drawEditBox(g.nvgContext, value_str.c_str(), x, y, box_width, h);
		
		g.currentTab.add_cell(h);
		return false;
	}
}
