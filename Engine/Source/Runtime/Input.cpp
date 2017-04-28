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
			LogWarning(Format("invalid mouse button id: %1%", button));
			return false;
		}
	}

	bool Input::GetMouseButtonDown(int button)
	{
		if (button >= 0 && button <= 3) {
			return m_mouseButtonStates[button] == MouseButtonState::Down;
		}
		else {
			LogWarning(Format("invalid mouse button id: %1%", button));
			return false;
		}
	}

	bool Input::GetMouseButtonUp(int button)
	{
		if (button >= 0 && button <= 3) {
			return m_mouseButtonStates[button] == MouseButtonState::Up;
		}
		else {
			LogWarning(Format("invalid mouse button id: %1%", button));
			return false;
		}
	}

	void Input::Update()
	{
		for (auto& s : m_keyStates) {
			if (s == KeyState::Down)
				s = KeyState::Held;
			if (s == KeyState::Up)
				s = KeyState::None;
		}

		for (auto& b : m_mouseButtonStates) {
			if (b == MouseButtonState::Down)
				b = MouseButtonState::Held;
			else if (b == MouseButtonState::Up)
				b = MouseButtonState::None;
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
		if (key >= 0 && key < keyCount)
			m_keyStates[key] = state;
	}

	void Input::UpdateKeyState(KeyCode key, KeyState state)
	{
		int iKey = static_cast<int>(key);
		UpdateKeyState(iKey, state);
	}

	void Input::UpdateMouseButtonState(int button, MouseButtonState state)
	{
		if (button >= 0 && button <= 3)
			m_mouseButtonStates[button] = state;
	}
}
