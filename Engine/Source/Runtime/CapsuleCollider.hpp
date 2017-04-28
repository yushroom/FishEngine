#ifndef CapsuleCollider_hpp
#define CapsuleCollider_hpp

#include "Collider.hpp"

namespace FishEngine
{
	class FE_EXPORT CapsuleCollider : public Collider
	{
	public:
		DefineComponent(CapsuleCollider);
		
		CapsuleCollider() = default;
		
		CapsuleCollider(const Vector3&  center,
						const float     height,
						const float     radius);
		
		// The center of the capsule, measured in the object's local space.
		Vector3 center() const { return m_center; }
		void setCenter(Vector3 const & center) { m_center = center; }

		// The radius of the sphere, measured in the object's local space.
		float radius() const { return m_radius; }
		void setRadius(float radius) { m_radius = radius; }

		// The height of the capsule measured in the object's local space.
		float height() const { return m_height; }
		void setHeight(float height) { m_height = height; }

		// The direction of the capsule.
		// The value can be 0, 1 or 2 corresponding to the X, Y and Z axes, respectively.
		int direction() const { return m_direction; }
		void setDirection(int direction) { m_direction = direction; }

		virtual void OnDrawGizmosSelected() override;
		
	private:
		friend class FishEditor::Inspector;
		Vector3 m_center = {0, 0, 0};
		int m_direction = 1; // The value can be 0, 1 or 2 corresponding to the X, Y and Z axes, respectively.
		float m_height = 2;
		float m_radius = 0.5f;
		
		virtual void CreatePhysicsShape() override;
	};
}

#endif // CapsuleBoxCollider_hpp
