#ifndef Collider_hpp
#define Collider_hpp

#include "FishEngine.hpp"
#include "Component.hpp"
#include "Bounds.hpp"
#include "Ray.hpp"

namespace physx {
    class PxShape;
    class PxRigidDynamic;
}

namespace FishEngine
{
    class FE_EXPORT Collider : public Component
    {
    public:
        InjectClassName(Collider)

        Vector3 ClosestPointOnBounds(const Vector3& position);
        //bool Raycast(const Ray& ray, RaycastHit* out_hitInfo, const float maxDistance);

        virtual void Start() override;
        
    protected:

		Meta(NonSerializable)
        RigidbodyPtr m_attachedRigidbody;

		Meta(NonSerializable)
        Bounds  m_bounds;

        float   m_contactOffset;
        bool    m_enabled;
        bool    m_isTrigger;
        
        physx::PxShape* physicsShape();
        virtual void CreatePhysicsShape() = 0;
        
		Meta(NonSerializable)
        physx::PxShape* m_physxShape = nullptr;
        //physx::PxRigidDynamic* m_physxRigidDynamic;
    };
}

#endif // Collider_hpp
