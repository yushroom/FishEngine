#ifndef Gizmos_hpp
#define Gizmos_hpp

#include "FishEngine.hpp"
#include "Color.hpp"
#include "Vector3.hpp"
#include "GLEnvironment.hpp"

namespace FishEngine
{
    class Gizmos
    {
    public:
        Gizmos() = delete;
        
        static Color color() {
            return s_color;
        }
        
        static void setColor(const Color& c) {
            s_color = c;
        }
        
        static void
        DrawCube(Vector3 center,
                 Vector3 size);
        
        static void
        DrawIcon(Vector3 center,
                 const std::string& name,
                 bool allowScaling = true);
        
        static void
        DrawLine(const Vector3& from,
                 const Vector3& to);
        
        static void
        DrawSphere(const Vector3& center,
                   const float radius);
        static void
        DrawRay(const Ray& r);
        
        static void
        DrawRay(const Vector3& from,
                const Vector3& direction);
        
        static void
        DrawWireCube(const Vector3& center,
                     const Vector3& size);
        
        static void
        DrawWireSphere(const Vector3& center,
                       const float radius);
        
    private:
        friend FishEditor::EditorRenderSystem;
        friend FishEditor::EditorGUI;
        static Color s_color;

        static void Init();
        
        static std::shared_ptr<SimpleMesh> s_lineMesh;
        static std::shared_ptr<SimpleMesh> s_circleMesh;
        static std::shared_ptr<SimpleMesh> s_boxMesh;
    };
}

#endif // Gizmos_hpp
