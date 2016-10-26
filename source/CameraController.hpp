#ifndef CameraController_hpp
#define CameraController_hpp

#include "Input.hpp"
#include <imgui/imgui.h>

namespace FishEngine
{
    class CameraController : public Script
    {
    public:
        InjectClassName(CameraController)

        bool m_isRotating = false;
        bool m_lookAtMode = true;
        float m_rotateSpeed = 200;
        float m_dragSpeed = 10;

        Vector3 m_originalPosition;
        Quaternion m_originalRotation;
        Vector3 m_originalScale;

        virtual void Start() override
        {
            m_originalPosition = transform()->localPosition();
            m_originalRotation = transform()->localRotation();
            m_originalScale = transform()->localScale();
        }

        virtual void OnInspectorGUI() override {
            ImGui::Checkbox("rotate camera", &m_isRotating);
            ImGui::Checkbox("lookat mode", &m_lookAtMode);
            if (ImGui::Button("reset")) {
                //transform()->setLocalPosition(0, 0, -5);
                //transform()->LookAt(Vector3(0, 0, 0));
                //transform()->setLocalEulerAngles(0, 0, 0);
                transform()->setLocalPosition(m_originalPosition);
                transform()->setLocalRotation(m_originalRotation);
                transform()->setLocalScale(m_originalScale);
            }
        }

        virtual void Update() override
        {
            if (m_isRotating) {
                transform()->RotateAround(Vector3(0, 0, 0), Vector3(0, 1, 0), 1);
            }

            if (Input::GetMouseButton(1)) {
                float x = m_rotateSpeed * Input::GetAxis(Axis::MouseX);
                float y = m_rotateSpeed * Input::GetAxis(Axis::MouseY);
                auto point = m_lookAtMode ? Vector3(0, 0, 0) : transform()->position();
                transform()->RotateAround(point, Vector3(0, 1, 0), x);
                transform()->RotateAround(point, transform()->right(), -y);
            }

            if (Input::GetMouseButton(2)) {
                float x = m_dragSpeed * Input::GetAxis(Axis::MouseX);
                float y = m_dragSpeed * Input::GetAxis(Axis::MouseY);
                transform()->Translate(-x, -y, 0);
            }

            float s = Input::GetAxis(Axis::MouseScrollWheel);
            if (s != 0.f) {
                auto t = Camera::main()->transform();
                t->setLocalPosition(t->position() + 0.2f*s*t->forward());
            }
        }
    };
}

#endif //CameraController_hpp
