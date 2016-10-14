#ifndef BoxCollider_hpp
#define BoxCollider_hpp

#include "Collider.hpp"

namespace FishEngine
{
    class BoxCollider : public Collider
    {
    public:
        InjectClassName(BoxCollider);
        BoxCollider(const Vector3& center,
                    const Vector3& size);
        
        virtual void OnDrawGizmos() override;
        
    private:
        friend class FishEditor::EditorGUI;
        Vector3 m_center;
        Vector3 m_size;
        
        virtual void CreatePhysicsShape() override;
    };
}

#endif // BoxCollider_hpp
