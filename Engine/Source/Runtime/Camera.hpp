#ifndef Camera_hpp
#define Camera_hpp

#include "Script.hpp"
//#include "GLEnvironment.hpp"
#include "Matrix4x4.hpp"
#include "Ray.hpp"
#include "Frustum.hpp"

namespace FishEngine 
{
    enum class CameraType
    {
        Game,
        SceneView,
        Preview,
    };

    class FE_EXPORT Camera : public Behaviour
    {
    public:
        InjectClassName(Camera)

        //private:
        Camera() = default;
        Camera(float fov, float nearClipPlane, float farClipPlane);

        // The aspect ratio (width divided by height).
        float aspect() const
        {
            return m_aspect;
        }

        float fieldOfView() const
        {
            return m_fieldOfView;
        }

        float orthographicSize() const
        {
            return m_orthographicSize;
        }

        bool orghographic() const
        {
            return m_orthographic;
        }

        void setOrthographic(bool value)
        {
            m_orthographic = value;
            m_isDirty = true;
        }

        void setAspect(float aspect)
        {
            m_aspect = aspect;
            m_isDirty = true;
            m_isAspectSet = true;
        }

        void setFieldOfView(float fieldOfView)
        {
            m_fieldOfView = fieldOfView;
            m_isDirty = true;
        }

        float nearClipPlane() const
        {
            return m_nearClipPlane;
        }

        void setNearClipPlane(const float nearClipPlane)
        {
            m_nearClipPlane = nearClipPlane;
            m_isDirty = true;
        }

        float farClipPlane() const
        {
            return m_farClipPlane;
        }

        void setFarClipPlane(const float farClipPlane)
        {
            m_farClipPlane = farClipPlane;
            m_isDirty = true;
        }
        
        Frustum frustum() const
        {
            return {m_fieldOfView, m_farClipPlane, m_nearClipPlane, m_aspect};
        }

        // Matrix that transforms from world to camera space (i.e. view matrix).
        Matrix4x4 worldToCameraMatrix() const;

        // projection matrix.
        Matrix4x4 projectionMatrix() const;

        const Vector4& viewport() const
        {
            return m_viewport;
        }

        void ResetAspect();

        // Returns a ray going from camera through a screen point.
        Ray ScreenPointToRay(const Vector3& position);

        void virtual OnDrawGizmos() override;
        void virtual OnDrawGizmosSelected() override;

        // TODO
        // The first enabled camera tagged "MainCamera" (Read Only).
        static CameraPtr main();

        static CameraPtr mainGameCamera();

        static void OnWindowSizeChanged(const int width, const int height);

        static CameraPtr
        Create(
            float       fov             = 60,
            float       nearClipPlane   = 0.3f,
            float       farClipPlane    = 1000.0f,
            CameraType  type            = CameraType::Game);

    private:
        friend class RenderSystem;
        friend class FishEditor::EditorGUI;
        friend class FishEditor::EditorRenderSystem;
        friend class FishEditor::FishEditorWindow;
        friend class FishEditor::SceneViewEditor;
        friend class Serialization;

        float m_fieldOfView         = 60.f;

        // Projection's half-size(vertical) when in orthographic mode.
        float m_orthographicSize    = 5.f;

        // The aspect ratio (width divided by height).
        float m_aspect              = 1.0f;
        bool m_isAspectSet         = false;

        float m_farClipPlane        = 0.3f;
        float m_nearClipPlane       = 100.f;
        Vector4 m_viewport{ 0, 0, 1, 1 };
        mutable bool m_isDirty = true;

        CameraType  m_cameraType = CameraType::Game;
        bool        m_orthographic = false;

        mutable Matrix4x4 m_projectMatrix;

        // temp
        // https://docs.unity3d.com/Manual/SceneViewNavigation.html
        mutable Vector3 m_focusPoint{ 0, 0, 0 };
        void FrameSelected(GameObjectPtr& selected);

        static CameraPtr m_mainCamera;
        static std::vector<CameraPtr> m_allCameras;
    };
}

#endif /* Camera_hpp */
