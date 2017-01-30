#include "EditorInput.hpp"
#include <cassert>

#include <Debug.hpp>

using namespace FishEngine;

namespace FishEditor
{
    KeyState EditorInput::m_keyStates[keyCount] = { KeyState::None };
    MouseButtonState EditorInput::m_mouseButtonStates[3] = { MouseButtonState::None };

    float EditorInput::m_mousePositionX = 0;
    float EditorInput::m_mousePositionY = 0;
    float EditorInput::m_axis[(int)Axis::AxisCount] = { 0.0f };

    void EditorInput::Init()
    {
        for (auto& s : m_keyStates)
            s = KeyState::None;
        for (auto& b : m_mouseButtonStates)
            b = MouseButtonState::None;

        for (auto& a : m_axis) {
            a = 0.f;
        }
        Input::Init();
    }

    bool EditorInput::GetKey(KeyCode key)
    {
        return m_keyStates[(int)key] == KeyState::Held;
    }

    bool EditorInput::GetKeyDown(KeyCode key)
    {
        return m_keyStates[(int)key] == KeyState::Down;
    }

    bool EditorInput::GetKeyUp(KeyCode key)
    {
        return m_keyStates[(int)key] == KeyState::Up;
    }

    bool EditorInput::GetMouseButton(int button)
    {
        if (button >= 0 && button <= 3) {
            return m_mouseButtonStates[button] == MouseButtonState::Held;
        }
        else {
            Debug::LogWarning("invalid mouse button id: %d", button);
            return false;
        }
    }

    bool EditorInput::GetMouseButtonDown(int button)
    {
        if (button >= 0 && button <= 3) {
            return m_mouseButtonStates[button] == MouseButtonState::Down;
        }
        else {
            Debug::LogWarning("invalid mouse button id: %d", button);
            return false;
        }
    }

    bool EditorInput::GetMouseButtonUp(int button)
    {
        if (button >= 0 && button <= 3) {
            return m_mouseButtonStates[button] == MouseButtonState::Up;
        }
        else {
            Debug::LogWarning("invalid mouse button id: %d", button);
            return false;
        }
    }

    void EditorInput::CopyToInput()
    {
        memcpy(Input::m_keyStates, m_keyStates, sizeof(m_keyStates));
        memcpy(Input::m_axis, m_axis, sizeof(m_axis));
        memcpy(Input::m_mouseButtonStates, m_mouseButtonStates, sizeof(m_axis));
        Input::m_mousePositionX = m_mousePositionX;
        Input::m_mousePositionY = m_mousePositionY;
    }

    void EditorInput::Update()
    {
        for (auto& s : m_keyStates) {
            if (s == KeyState::Up || s == KeyState::Down)
                s = KeyState::None;
        }

        for (auto& b : m_mouseButtonStates) {
            if (b == MouseButtonState::Down) b = MouseButtonState::Held;
            else if (b == MouseButtonState::Up) b = MouseButtonState::None;
        }

        for (auto& a : m_axis) {
            a = 0.f;
        }
        //m_axis[(int)Axis::MouseScrollWheel] = 1.0f;
    }

    void EditorInput::UpdateAxis(Axis axis, float value)
    {
        m_axis[(int)axis] = value;
    }

    void EditorInput::UpdateMousePosition(float xpos, float ypos)
    {
        m_axis[(int)Axis::MouseX] = xpos - m_mousePositionX;
        m_axis[(int)Axis::MouseY] = ypos - m_mousePositionY;
        m_mousePositionX = xpos;
        m_mousePositionY = ypos;
    }

    void EditorInput::UpdateKeyState(int key, KeyState state)
    {
        assert(key >= 0 && key < keyCount);
        m_keyStates[key] = state;
    }

    void EditorInput::UpdateMouseButtonState(int button, MouseButtonState state)
    {
        assert(button >= 0 && button <= 3);
        m_mouseButtonStates[button] = state;
    }
}
