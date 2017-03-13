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

		inline bool enabled() const { return m_enabled; }
		void setEnabled(bool enabled) { m_enabled = enabled; }
		
	protected:
		friend class FishEditor::Inspector;
		friend class Rigidbody;
		Meta(NonSerializable)
		
		// The rigidbody the collider is attached to.
		std::weak_ptr<Rigidbody> m_attachedRigidbody;

		// The world space bounding volume of the collider.
		Meta(NonSerializable)
		Bounds  m_bounds;

		// Contact offset value of this collider.
		float   m_contactOffset;
		
		// Enabled Colliders will collide with other colliders, disabled Colliders won't.
		bool    m_enabled		= true;
		
		// Is the collider a trigger?
		bool    m_isTrigger		= false;
		
		physx::PxShape* physicsShape() { return m_physxShape; }
		virtual void CreatePhysicsShape() = 0;
		
		Meta(NonSerializable)
		physx::PxShape* m_physxShape = nullptr;
		//physx::PxRigidDynamic* m_physxRigidDynamic;
	};
}

#endif // Collider_hpp
