#ifndef SkinnedMeshRenderer_hpp
#define SkinnedMeshRenderer_hpp

#include "Renderer.hpp"
#include "Animator.hpp"

namespace FishEngine
{
	class FE_EXPORT SkinnedMeshRenderer : public Renderer
	{
	public:
		InjectClassName(SkinnedMeshRenderer)

		SkinnedMeshRenderer() = default;

		SkinnedMeshRenderer(MaterialPtr material);

		virtual void Update() override;
		
		//protected:
		virtual void Render() const override;

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
		std::vector<std::weak_ptr<Transform>> m_bones;  // to be done
		
		Meta(NonSerializable)
		mutable std::vector<Matrix4x4> m_matrixPalette;
		void UpdateMatrixPalette() const;
	};

}

#endif /* SkinnedMeshRenderer_hpp */
