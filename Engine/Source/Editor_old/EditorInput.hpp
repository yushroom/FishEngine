#ifndef EditorEditorInput_hpp
#define EditorEditorInput_hpp

#include <Input.hpp>

namespace FishEditor
{
    constexpr int keyCount = 512;

    class EditorInput {
    public:
        EditorInput() = delete;

        // Returns the value of the virtual axis identified by axisName.
        static float GetAxis(FishEngine::Axis axis)
        {
            return m_axis[(int)axis];
        }

        // Returns true while the user holds down the key identified by name. Think auto fire.
        static bool GetKey(FishEngine::KeyCode key);

        // Returns true during the frame the user starts pressing down the key identified by name.
        static bool GetKeyDown(FishEngine::KeyCode key);

        // Returns true during the frame the user releases the key identified by name.
        static bool GetKeyUp(FishEngine::KeyCode key);

        // Returns whether the given mouse button is held down.
        // button values are 0 for left button, 1 for right button, 2 for the middle button.
        static bool GetMouseButton(int button);

        // Returns true during the frame the user pressed the given mouse button.
        // button values are 0 for left button, 1 for right button, 2 for the middle button.
        static bool GetMouseButtonDown(int button);

        // Returns true during the frame the user releases the given mouse button.
        // button values are 0 for left button, 1 for right button, 2 for the middle button.
        static bool GetMouseButtonUp(int button);

        static void CopyToInput();

    private:
        static void Init();
        static void Update();

        static void UpdateAxis(FishEngine::Axis axis, float value);
        static void UpdateMousePosition(float xpos, float ypos);
        static void UpdateKeyState(int key, FishEngine::KeyState state);
        static void UpdateMouseButtonState(int button, FishEngine::MouseButtonState state);

    private:
        friend class FishEditorWindow;

        static FishEngine::KeyState m_keyStates[keyCount];

        // button values are 0 for left button, 1 for right button, 2 for the middle button.
        static FishEngine::MouseButtonState m_mouseButtonStates[3];
        static float m_mousePositionX;  // normalized, (0, 1)
        static float m_mousePositionY;  // normalized, (0, 1)
        static float m_axis[(int)FishEngine::Axis::AxisCount];
    };
}

#endif // EditorEditorInput_hpp
