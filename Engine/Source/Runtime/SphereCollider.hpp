#ifndef SphereCollider_hpp
#define SphereCollider_hpp

#include "Collider.hpp"

namespace FishEngine
{
    class FE_EXPORT SphereCollider : public Collider
    {
    public:
        InjectClassName(SphereCollider);
        SphereCollider() = default;
        SphereCollider(const Vector3& center,
                       const float raduis);
        
        virtual void OnDrawGizmosSelected() override;

    private:
        friend class FishEditor::EditorGUI;
        Vector3 m_center{0, 0, 0};
        float m_radius = 1;
        
        virtual void CreatePhysicsShape() override;
    };
}

#endif // SphereCollider_hpp
