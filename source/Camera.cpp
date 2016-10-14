#include "Camera.hpp"
#include "GameObject.hpp"
#include "Scene.hpp"
//#include <imgui/imgui.h>
#include "Screen.hpp"

namespace FishEngine
{
    PCamera Camera::m_mainCamera = nullptr;
    std::vector<PCamera> Camera::m_allCameras;

    Camera::Camera(float fov, float aspect, float zNear, float zFar)
        : m_fieldOfView(fov), m_aspect(aspect), m_nearClipPlane(zNear), m_farClipPlane(zFar)
    {
        //m_focusPoint = transform()->position() + transform()->forward() * 5.f;
    }


    Matrix4x4 Camera::projectionMatrix() const
    {
        if (m_isDirty) {
            if (m_orthographic) {
                float w = Screen::aspect() * m_orthographicSize;
                m_projectMatrix = Matrix4x4::Ortho(-w, w, -m_orthographicSize, m_orthographicSize, m_nearClipPlane, m_farClipPlane);
            }
            else {
                m_projectMatrix = Matrix4x4::Perspective(m_fieldOfView, m_aspect, m_nearClipPlane, m_farClipPlane);
            }
            m_isDirty = false;
        }
        return m_projectMatrix;
    }

    //void Camera::OnInspectorGUI()
    //{
    //    const char* listbox_items[] = {
    //        "Perspective", "Orthographic"
    //    };
    //    int list_item_current = m_orthographic ? 1 : 0;
    //    ImGui::Combo("Projection", &list_item_current, listbox_items, 2);
    //    if (m_orthographic != (list_item_current == 1)) {
    //        m_orthographic = !m_orthographic;
    //        m_isDirty = true;
    //    }
    //    //m_orthographic = list_item_current == 1;
    //    
    //    if (m_orthographic) {
    //        if (ImGui::InputFloat("Size", &m_orthographicSize)) {
    //            m_isDirty = true;
    //        }
    //    }
    //    else {
    //        if (ImGui::SliderFloat("Field of View", &m_fieldOfView, 1, 179)) {
    //            m_isDirty = true;
    //        }
    //    }
    //
    //    if (ImGui::InputFloat("Clipping Planes(Near)", &m_nearClipPlane)) {
    //        m_isDirty = true;
    //    }
    //    if (ImGui::InputFloat("Clipping Planes(Far)", &m_farClipPlane)) {
    //        m_isDirty = true;
    //    }
    //    ImGui::InputFloat4("Viewport Rect", m_viewport.data());
    //}
    //
    FishEngine::Ray FishEngine::Camera::ScreenPointToRay(const Vector3& position)
    {
        //http://antongerdelan.net/opengl/raycasting.html

        // NDC space
        float x = (2.0f * position.x) / Screen::width() - 1.0f;
        float y = (2.0f * position.y) / Screen::height() - 1.0f;
        Vector4 ray_clip(x, y, 1.f, 1.0f);

        Vector4 ray_eye = projectionMatrix().inverse() * ray_clip;
        ray_eye.z = 1.0f; // forward
        ray_eye.w = 0.0f;

        Vector4 ray_world_h = transform()->localToWorldMatrix() * ray_eye;
        Vector3 ray_world(ray_world_h.x, ray_world_h.y, ray_world_h.z);
        return Ray(transform()->position(), ray_world.normalized());
    }

    Matrix4x4 Camera::worldToCameraMatrix() const
    {
        return transform()->worldToLocalMatrix();
    }

    std::shared_ptr<Camera> Camera::main()
    {
        if (m_mainCamera == nullptr)
        {
            for (auto& c : m_allCameras)
            {
                if (c->tag() == "MainCamera")
                {
                    m_mainCamera = c;
                    return m_mainCamera;
                }
            }
        }
        return m_mainCamera;
    }


    std::shared_ptr<Camera> Camera::
        mainGameCamera()
    {
        for (auto& c : m_allCameras)
        {
            if (c->tag() == "MainCamera" && c->m_cameraType == CameraType::Game)
            {
                return c;
            }
        }
        return nullptr;
    }


    void FishEngine::Camera::FrameSelected(std::shared_ptr<GameObject>& selected)
    {
        auto camera = Camera::main()->transform();
        float focus_distance = Vector3::Distance(camera->position(), m_focusPoint);
        m_focusPoint = selected->transform()->position();
        auto camera_dir = camera->forward().normalized();
        auto target_camera_position = m_focusPoint - camera_dir * focus_distance;
        auto translation = target_camera_position - camera->position();
        camera->Translate(translation, Space::World);
    }
}
