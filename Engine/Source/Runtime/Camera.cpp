#include "Camera.hpp"
#include "GameObject.hpp"
#include "Scene.hpp"
#include "Screen.hpp"
#include "Gizmos.hpp"

namespace FishEngine
{
    CameraPtr Camera::m_mainCamera = nullptr;
    std::vector<CameraPtr> Camera::m_allCameras;


    void Camera::OnWindowSizeChanged(const int width, const int height)
    {
        const float aspect = float(width) / height;
        for (auto& cam : m_allCameras)
        {
            if (!cam->m_isAspectSet)
            {
                cam->m_aspect = aspect;
                cam->m_isDirty = true;
            }
        }
    }

    CameraPtr Camera::Create(
        float fov, 
        float nearClipPlane, 
        float farClipPlane, 
        CameraType type /*= CameraType::Game*/)
    {
        auto camera = std::make_shared<Camera>(fov, nearClipPlane, farClipPlane);
        camera->m_cameraType = type;
        m_allCameras.push_back(camera);
        return camera;
    }

    Camera::Camera(float fov, float zNear, float zFar)
        : m_fieldOfView(fov), m_nearClipPlane(zNear), m_farClipPlane(zFar)
    {
        //m_focusPoint = transform()->position() + transform()->forward() * 5.f;
        ResetAspect();
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


    void Camera::ResetAspect()
    {
        m_aspect = Screen::aspect();
        m_isAspectSet = false;
    }

    Ray Camera::ScreenPointToRay(const Vector3& position)
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

    void Camera::OnDrawGizmos()
    {
        Gizmos::DrawIcon(transform()->position(), "Camera");
    }

    void Camera::OnDrawGizmosSelected()
    {
        Gizmos::setColor(Color::white);
        Gizmos::setMatrix(transform()->localToWorldMatrix());
        Gizmos::DrawFrustum(transform()->position(), m_fieldOfView, m_farClipPlane, m_nearClipPlane, m_aspect);
        Gizmos::setMatrix(Matrix4x4::identity);
    }

    CameraPtr Camera::main()
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


    CameraPtr Camera::
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


    void Camera::
    FrameSelected(GameObjectPtr& selected)
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
