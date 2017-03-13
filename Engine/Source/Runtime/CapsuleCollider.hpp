#ifndef CapsuleCollider_hpp
#define CapsuleCollider_hpp

#include "Collider.hpp"

namespace FishEngine
{
	class FE_EXPORT CapsuleCollider : public Collider
	{
	public:
		InjectClassName(CapsuleCollider);
		
		CapsuleCollider() = default;
		
		CapsuleCollider(const Vector3&  center,
						const float     height,
						const float     radius);
		
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
