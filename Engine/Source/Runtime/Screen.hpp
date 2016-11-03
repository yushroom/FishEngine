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

        static float aspect() { return static_cast<float>(m_width) / static_cast<float>(m_height); }

        // The current DPI of the screen / device (Read Only).
        static float dpi() { return m_dpi; }
        
        static float pixelsPerPoint() { return m_pixelsPerPoint; }
        //static void setPixelsPerPoint(const float value) { m_pixelsPerPoint = value; }
        
    private:
        friend class FishEditor::FishEditorWindow;
        friend class FishEditor::EditorRenderSystem;
        friend class FishEditor::SceneViewEditor;
        static int m_width;
        static int m_height;
        static float m_dpi;
        
        // This value is the number of screen pixels per point of interface space. For instance, 2.0 on retina displays.
        static float m_pixelsPerPoint;
    };
}

#endif // Screen_hpp
