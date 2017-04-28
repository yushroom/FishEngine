#pragma once

#include "Collider.hpp"

namespace FishEngine
{
	class FE_EXPORT MeshCollider final : public Collider
	{
	public:
		DefineComponent(MeshCollider);

		MeshCollider() = default;

		MeshPtr sharedMesh() const { return m_sharedMesh; }

	protected:
		virtual void CreatePhysicsShape() override;

	private:

		// The mesh object used for collision detection.
		MeshPtr m_sharedMesh = nullptr;

		// Use a convex collider from the mesh.
		bool	m_convex = false;

		// Allow the physics engine to increase the volume of the input mesh in attempt to generate a valid convex mesh.
		bool	m_inflateMesh = false;

		float	m_skinWidth = 0.01f;
	};
}