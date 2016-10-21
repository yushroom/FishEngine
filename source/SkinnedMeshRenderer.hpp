#ifndef SkinnedMeshRenderer_hpp
#define SkinnedMeshRenderer_hpp

#include "Renderer.hpp"
#include "Animator.hpp"

namespace FishEngine {
    
    class SkinnedMeshRenderer : public Renderer
    {
    public:
        InjectClassName(SkinnedMeshRenderer)

        SkinnedMeshRenderer() = default;

        SkinnedMeshRenderer(PMaterial material);

        virtual void Update() override;
        
        //protected:
        virtual void Render() const override;

        void setAvatar(PAvatar avatar) {
            m_avatar = avatar;
        }

        void setRootBone(std::weak_ptr<Transform> rootBone) {
            m_rootBone = rootBone;
        }
        
        // AABB of this Skinned Mesh in its local space.
        Bounds localBounds() const;
        
        // The mesh used for skinning.
        PMesh sharedMesh()
        {
            return m_sharedMesh;
        }
        
        void setSharedMesh(PMesh sharedMesh)
        {
            m_sharedMesh = sharedMesh;
        }

        virtual void OnDrawGizmosSelected() override;

    private:
        friend class FishEditor::EditorGUI;
        friend class FishEditor::EditorRenderSystem;
        friend class Scene;
        
        // The mesh used for skinning.
        PMesh m_sharedMesh = nullptr;
        
        PAvatar m_avatar = nullptr;
        std::weak_ptr<Transform> m_rootBone;
        
        // The bones used to skin the mesh.
        std::vector<std::weak_ptr<Transform>> m_bones;  // to be done
        
        mutable std::vector<Matrix4x4> m_matrixPalette;
        void UpdateMatrixPalette() const;
    };

}

#endif /* SkinnedMeshRenderer_hpp */
