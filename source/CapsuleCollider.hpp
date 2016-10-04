#ifndef CapsuleCollider_hpp
#define CapsuleCollider_hpp

#include "Collider.hpp"

namespace FishEngine
{
    class CapsuleCollider : public Collider
    {
    public:
        InjectClassName(CapsuleCollider);
        
        virtual void OnDrawGizmos() override;
        
    private:
        friend class FishEditor::EditorGUI;
        Vector3 m_center;
        Vector3 m_direction;
        float m_height;
        float m_radius;
    };
}

#endif // CapsuleBoxCollider_hpp
