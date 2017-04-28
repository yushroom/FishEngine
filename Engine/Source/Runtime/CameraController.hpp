#ifndef CameraController_hpp
#define CameraController_hpp

#include "Input.hpp"
#include "Transform.hpp"
#include "Camera.hpp"

namespace FishEngine
{
	
	class FE_EXPORT CameraController : public Script
	{
	public:
		DefineComponent(CameraController)

		bool m_lookAtMode = false;
		float m_rotateSpeed = 200;
		float m_dragSpeed = 10;
		Vector3 m_orbitCenter = Vector3::zero;

		// https://docs.unity3d.com/Manual/SceneViewNavigation.html
		enum class ControlType
		{
			None,
			Move,
			Rotate,
			Orbit,
			Zoom,
		};

		bool Check()
		{
			bool alt = Input::GetKey(KeyCode::LeftAlt) || Input::GetKey(KeyCode::RightAlt);
			bool ctrl = Input::GetKey(KeyCode::LeftControl) || Input::GetKey(KeyCode::RightControl);
			bool cmd = Input::GetKey(KeyCode::LeftCommand) || Input::GetKey(KeyCode::RightCommand);
			bool left = Input::GetMouseButtonDown(0) || Input::GetMouseButton(0);
			bool right = Input::GetMouseButtonDown(1) || Input::GetMouseButton(1);
			bool middle = Input::GetMouseButtonDown(2) || Input::GetMouseButton(2);
			float scrollValue = Input::GetAxis(Axis::MouseScrollWheel);
			bool scroll = scrollValue != 0.0f;

			ControlType type = ControlType::None;
			if (middle || (alt && ctrl && left) || (alt && cmd && left))
				type = ControlType::Move;
			else if (alt && left)
				type = ControlType::Orbit;
			else if (scroll || (alt && right))
				type = ControlType::Zoom;
			else if (right)
				type = ControlType::Rotate;

			bool modified = true;
			if (type == ControlType::Move)
			{
				float x = m_dragSpeed * Input::GetAxis(Axis::MouseX);
				float y = m_dragSpeed * Input::GetAxis(Axis::MouseY);
				transform()->Translate(-x, -y, 0);
				LogWarning("Move");
			}
			else if (type == ControlType::Rotate)
			{
				float x = m_rotateSpeed * Input::GetAxis(Axis::MouseX);
				float y = m_rotateSpeed * Input::GetAxis(Axis::MouseY);
				auto pivot = transform()->position();
				transform()->RotateAround(pivot, Vector3::up, x);
				transform()->RotateAround(pivot, transform()->right(), -y);
				LogWarning("Rotate");
			}
			else if (type == ControlType::Orbit)
			{
				float x = m_rotateSpeed * Input::GetAxis(Axis::MouseX);
				float y = m_rotateSpeed * Input::GetAxis(Axis::MouseY);
				transform()->RotateAround(m_orbitCenter, Vector3::up, x);
				transform()->RotateAround(m_orbitCenter, transform()->right(), -y);
				LogWarning("Orbit");
			}
			else if (type == ControlType::Zoom)
			{
				auto forward = transform()->forward();
				float deltaZ = 0;
				if (scrollValue != 0.f)
				{
					deltaZ = 0.2f*scrollValue;
					//transform()->Translate(0.2f*scrollValue*forward, Space::World);
				}
				else
				{
					float x = m_dragSpeed * Input::GetAxis(Axis::MouseX);
					float y = m_dragSpeed * Input::GetAxis(Axis::MouseY);
					deltaZ = fabsf(x) > fabsf(y) ? x : -y;
				}
				transform()->Translate(deltaZ*forward, Space::World);
				LogWarning("Zoom");
			}
			else
			{
				modified = false;
			}
			return modified;
		}

		virtual void Update() override
		{
			Check();
		}
	};
}

#endif //CameraController_hpp
