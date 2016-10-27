#include "Input.hpp"
#include <cassert>

#include "Debug.hpp"

namespace FishEngine
{
    KeyState Input::m_keyStates[keyCount] = { KeyState::None };
    MouseButtonState Input::m_mouseButtonStates[3] = { MouseButtonState::None };

    float Input::m_mousePositionX = 0;
    float Input::m_mousePositionY = 0;
    float Input::m_axis[(int)Axis::AxisCount] = { 0.0f };

    void Input::Init()
    {
        for (auto& s : m_keyStates)
            s = KeyState::None;
        for (auto& b : m_mouseButtonStates)
            b = MouseButtonState::None;

        for (auto& a : m_axis) {
            a = 0.f;
        }
    }

    bool Input::GetKey(KeyCode key)
    {
        return m_keyStates[(int)key] == KeyState::Held;
    }

    bool Input::GetKeyDown(KeyCode key)
    {
        return m_keyStates[(int)key] == KeyState::Down;
    }

    bool Input::GetKeyUp(KeyCode key)
    {
        return m_keyStates[(int)key] == KeyState::Up;
    }

    bool Input::GetMouseButton(int button)
    {
        if (button >= 0 && button <= 3) {
            return m_mouseButtonStates[button] == MouseButtonState::Held;
        }
        else {
            Debug::LogWarning("invalid mouse button id: %d", button);
            return false;
        }
    }

    bool Input::GetMouseButtonDown(int button)
    {
        if (button >= 0 && button <= 3) {
            return m_mouseButtonStates[button] == MouseButtonState::Down;
        }
        else {
            Debug::LogWarning("invalid mouse button id: %d", button);
            return false;
        }
    }

    bool Input::GetMouseButtonUp(int button)
    {
        if (button >= 0 && button <= 3) {
            return m_mouseButtonStates[button] == MouseButtonState::Up;
        }
        else {
            Debug::LogWarning("invalid mouse button id: %d", button);
            return false;
        }
    }

    void Input::Update()
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

    void Input::UpdateAxis(Axis axis, float value)
    {
        m_axis[(int)axis] = value;
    }

    void Input::UpdateMousePosition(float xpos, float ypos)
    {
        m_axis[(int)Axis::MouseX] = xpos - m_mousePositionX;
        m_axis[(int)Axis::MouseY] = ypos - m_mousePositionY;
        m_mousePositionX = xpos;
        m_mousePositionY = ypos;
    }

    void Input::UpdateKeyState(int key, KeyState state)
    {
        assert(key >= 0 && key < keyCount);
        m_keyStates[key] = state;
    }

    void Input::UpdateMouseButtonState(int button, MouseButtonState state)
    {
        assert(button >= 0 && button <= 3);
        m_mouseButtonStates[button] = state;
    }
}
