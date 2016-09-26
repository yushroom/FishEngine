#ifndef Screen_hpp
#define Screen_hpp

#include "FishEngine.hpp"

namespace FishEngine
{
    class Screen
    {
    public:
        Screen() = delete;

        // The current width of the screen window in pixels (Read Only).
        static int width() { return m_width; }

        // The current height of the screen window in pixels (Read Only).
        static int height() { return m_height; }

        static float aspect() { return float(m_width) / float(m_height); }

        // The current DPI of the screen / device (Read Only).
        static float dpi() { return m_dpi; }

    private:
        friend class FishEditor::FishEditorWindow;
        static int m_width;
        static int m_height;
        static float m_dpi;
    };
}

#endif // Screen_hpp
