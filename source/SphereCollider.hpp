#ifndef SphereCollider_hpp
#define SphereCollider_hpp

#include "Collider.hpp"

namespace FishEngine
{
    class SphereCollider : public Collider
    {
    public:
        InjectClassName(SphereCollider);
        SphereCollider(const Vector3& center,
                       const float raduis);
        
        virtual void OnDrawGizmos() override;
        virtual void CreatePhysicsShape() override;
        
    private:
        friend class FishEditor::EditorGUI;
        Vector3 m_center;
        float m_radius;
    };
}

#endif // SphereCollider_hpp
