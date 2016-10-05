#ifndef CapsuleCollider_hpp
#define CapsuleCollider_hpp

#include "Collider.hpp"

namespace FishEngine
{
    class CapsuleCollider : public Collider
    {
    public:
        InjectClassName(CapsuleCollider);
        
        CapsuleCollider(const Vector3&  center,
                        const float     height,
                        const float     radius);
        virtual void OnDrawGizmos() override;
        
    private:
        friend class FishEditor::EditorGUI;
        Vector3 m_center;
        int m_direction; // The value can be 0, 1 or 2 corresponding to the X, Y and Z axes, respectively.
        float m_height;
        float m_radius;
    };
}

#endif // CapsuleBoxCollider_hpp
