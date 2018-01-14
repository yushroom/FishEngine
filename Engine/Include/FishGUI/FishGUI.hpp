#ifndef RFGUI_hpp
#define RFGUI_hpp

#include <string>
#include <functional>
#include <list>

struct GLFWwindow;

namespace FishGUI
{
    struct Vector2 {
		float x = 0;
		float y = 0;
    };
	
	struct Vector3 {
		float x, y, z;
	};
	
	struct Vector4 {
		float x, y, z, w;
	};
	
	struct Rect {
		int x, y, width, height;
	};
	
	struct Int2 {
		int x = 0;
		int y = 0;
	};
	
	struct Size {
		int width = 0;
		int height = 0;
	};
	
    enum class TabPosition
	{
        Left,
        Right,
        Top,
        Bottom,
        Floating,
    };
	
	struct Window;
	
	struct Tab
	{
		std::string		m_title;
		TabPosition		m_position;
		Window*			m_window = nullptr;
		Rect			m_rect;
		int				m_size = 1;
		int				m_minimalSize = 128;
		bool			m_resizable = true;
		bool			m_resizing = false;
		bool			m_moving = false;
		std::function<void(void)> m_renderFunction;
	};
    
	enum class GUIAlignment {
        HoriontallyLeft     = 1 << 0,
        HoriontallyCenter   = 1 << 1,
        HoriontallyRight    = 1 << 2,
        VerticallyTop       = 1 << 3,
        VerticallyCenter    = 1 << 4,
        VerticallyBottom    = 1 << 5,
        Center = HoriontallyCenter | VerticallyCenter
    };
    
    void Init(GLFWwindow * window);
	
	Tab* CreateTab(const char* title, TabPosition tabPosition, int widthOrHeight);
    
    void Begin();
    void End();
	
	void RenderWindows();
	
	void BeginTab(Tab * tab);
	void EndTab();
	
    bool Button(const char* text);
	void Label(const char* text, GUIAlignment alignment = GUIAlignment::Center);
	bool CheckBox(const char * label, bool* v);
	bool InputText(const char* label, std::string& text);
	bool Slider(const char* label, float *value, float minValue, float maxValue);
}

#endif // RFGUI_hpp
