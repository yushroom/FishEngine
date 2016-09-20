#ifndef CameraController_hpp
#define CameraController_hpp

#include "Input.hpp"
#include <imgui/imgui.h>

NAMESPACE_FISHENGINE_BEGIN

class CameraController : public Script
{
public:
    InjectClassName(CameraController)
    
    bool m_isRotating = false;
    bool m_lookAtMode = true;
    float m_rotateSpeed = 200;
    float m_dragSpeed = 10;
    
    virtual void Start() override
    {
    }
    
    virtual void OnInspectorGUI() override {
        ImGui::Checkbox("rotate camera", &m_isRotating);
        ImGui::Checkbox("lookat mode", &m_lookAtMode);
        if (ImGui::Button("reset")) {
            transform()->setLocalPosition(0, 0, -5);
            transform()->LookAt(Vector3(0, 0, 0));
            //transform()->setLocalEulerAngles(0, 0, 0);
        }
    }
    
    virtual void Update() override
    {
        if (m_isRotating) {
            transform()->RotateAround(Vector3(0, 0, 0), Vector3(0, 1, 0), 1);
        }
        
        if (Input::GetMouseButton(1)) {
            float x = m_rotateSpeed * Input::GetAxis(Input::Axis::MouseX);
            float y = m_rotateSpeed * Input::GetAxis(Input::Axis::MouseY);
            auto point = m_lookAtMode ? Vector3(0, 0, 0) : transform()->position();
            transform()->RotateAround(point, Vector3(0, 1, 0), x);
            transform()->RotateAround(point, transform()->right(), -y);
        }
        
        if (Input::GetMouseButton(2)) {
            float x = m_dragSpeed * Input::GetAxis(Input::Axis::MouseX);
            float y = m_dragSpeed * Input::GetAxis(Input::Axis::MouseY);
            transform()->Translate(-x, -y, 0);
        }
    }
};

NAMESPACE_FISHENGINE_END

#endif //CameraController_hpp
