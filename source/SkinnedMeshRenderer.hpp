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

        SkinnedMeshRenderer(std::shared_ptr<Material> material);

        virtual void Update() override;
        
        //protected:
        virtual void Render() const override;

        void setAvatar(std::shared_ptr<Avatar> avatar) {
            m_avatar = avatar;
        }

        void setRootBone(std::weak_ptr<Transform> rootBone) {
            m_rootBone = rootBone;
        }
        
        // AABB of this Skinned Mesh in its local space.
        Bounds localBounds() const;
        
        // The mesh used for skinning.
        std::shared_ptr<Mesh> sharedMesh()
        {
            return m_sharedMesh;
        }
        
        void setSharedMesh(std::shared_ptr<Mesh> sharedMesh)
        {
            m_sharedMesh = sharedMesh;
        }

    private:
        friend class FishEditor::EditorGUI;
        
        // The mesh used for skinning.
        std::shared_ptr<Mesh> m_sharedMesh = nullptr;
        
        std::shared_ptr<Avatar> m_avatar = nullptr;
        std::weak_ptr<Transform> m_rootBone;
        
        // The bones used to skin the mesh.
        std::vector<std::weak_ptr<Transform>> m_bones;  // to be done
        
        mutable std::vector<Matrix4x4> m_boneTransformation;
    };

}

#endif /* SkinnedMeshRenderer_hpp */
