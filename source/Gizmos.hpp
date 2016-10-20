#ifndef Gizmos_hpp
#define Gizmos_hpp

#include "FishEngine.hpp"
#include "Color.hpp"
#include "Vector3.hpp"
#include "Matrix4x4.hpp"
#include "GLEnvironment.hpp"

namespace FishEngine
{
    class Gizmos
    {
    public:
        Gizmos() = delete;
        
        static Color color()
        {
            return s_color;
        }
        
        static void
        setColor(const Color& c)
        {
            s_color = c;
        }
        
        static void
        DrawCube(
            Vector3 center,
            Vector3 size);
        
        static void
        DrawIcon(
            const Vector3&      center,
            const std::string&  name,
            bool                allowScaling = true);
        
        static void
        DrawLine(
            const Vector3& from,
            const Vector3& to);
        
        
        static void
        DrawSphere(
            const Vector3& center,
            const float radius);
        
        
        static void
        DrawRay(const Ray& r);
        
        
        static void
        DrawRay(
            const Vector3& from,
            const Vector3& direction);

        static void
        DrawHalfCircle(
            const Vector3&  center,
            const float     radius,
            const Vector3   dir1,
            const Vector3   dir2);

        static void
        DrawCircle(
            const Vector3&  center,
            const float     radius,
            const Vector3&  direction = Vector3::up);

        static void
        DrawLight(
            const Vector3&  center,
            const Vector3&  direction);

        static void
        DrawFrustum(
            const Matrix4x4&    localToWorld,
            const float         zNear,
            const float         zFar,
            const float         fovy,
            const float         aspect);

        static void
        DrawWireCube(
            const Vector3&      center,
            const Vector3&      size,
            const Matrix4x4&    modelMatrix = Matrix4x4::identity);

        static void
        DrawWireSphere(
            const Vector3&      center,
            const float         radius,
            const Matrix4x4&    modelMatrix = Matrix4x4::identity);

        static void
        DrawWireCapsule(
            const Vector3&      center,
            const float         radius,
            const float         height,
            const Matrix4x4&    modelMatrix = Matrix4x4::identity);

    private:
        friend FishEditor::EditorRenderSystem;
        friend FishEditor::EditorGUI;
        
        static void
        DrawHalfWireSphere(
            const Vector3& center,
            const float radius,
            const Matrix4x4& modelMatrix = Matrix4x4::identity);
        
        static Color s_color;

        static void Init();
        
        static PSimpleMesh s_circleMesh;
        static PSimpleMesh s_boxMesh;
        static PSimpleMesh s_light;
        //static PSimpleMesh s_wiredSphereMesh;
    };
}

#endif // Gizmos_hpp
