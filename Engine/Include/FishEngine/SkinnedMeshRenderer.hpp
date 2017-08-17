#ifndef SkinnedMeshRenderer_hpp
#define SkinnedMeshRenderer_hpp

#include "Renderer.hpp"
#include "Animator.hpp"

namespace FishEngine
{
	class FE_EXPORT SkinnedMeshRenderer : public Renderer
	{
	public:
		DefineComponent(SkinnedMeshRenderer)

		SkinnedMeshRenderer() = default;

		SkinnedMeshRenderer(MaterialPtr material);

		virtual void Update() override;

		//virtual void PreRender() const override;
		//virtual void Render() const override;

		void UpdataAnimation();

		void setAvatar(AvatarPtr avatar)
		{
			m_avatar = avatar;
		}

		void setRootBone(std::weak_ptr<Transform> rootBone)
		{
			m_rootBone = rootBone;
		}

		// AABB of this Skinned Mesh in its local space.
		virtual Bounds localBounds() const override;

		// The mesh used for skinning.
		MeshPtr sharedMesh()
		{
			return m_sharedMesh;
		}

		// The bones used to skin the mesh.
		std::vector<std::weak_ptr<Transform>> & bones()
		{
			return m_bones;
		}

		// The bones used to skin the mesh.
		std::vector<std::weak_ptr<Transform>> const & bones() const
		{
			return m_bones;
		}
		
		//std::vector<Matrix4x4> const & matrixPalette() const;

		void setSharedMesh(MeshPtr sharedMesh);

		virtual void OnDrawGizmosSelected() override;

	private:
		friend class FishEditor::Inspector;
		//friend class FishEditor::EditorRenderSystem;
		friend class FishEditor::SceneViewEditor;
		friend class Scene;

		// The mesh used for skinning.
		MeshPtr m_sharedMesh = nullptr;

		AvatarPtr m_avatar = nullptr;
		std::weak_ptr<Transform> m_rootBone;

		// The bones used to skin the mesh.
		// same size with sharedMesh.bindposes
		std::vector<std::weak_ptr<Transform>> m_bones;

		Meta(NonSerializable)
		mutable std::vector<Matrix4x4> m_matrixPalette;
		void UpdateMatrixPalette() const;
	};
}

#endif /* SkinnedMeshRenderer_hpp */
