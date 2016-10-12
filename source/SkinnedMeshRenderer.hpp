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

        //protected:
        virtual void Render() const override;

        void setAvatar(std::shared_ptr<Avatar> avatar) {
            m_avatar = avatar;
        }

        void setRootBone(std::weak_ptr<Transform> rootBone) {
            m_rootBone = rootBone;
        }

    private:
        friend class FishEditor::EditorGUI;
        std::shared_ptr<Avatar> m_avatar = nullptr;     // TODO: => SkinnedMeshRenderer
        std::weak_ptr<Transform> m_rootBone;            // TODO: => SkinnedMeshRenderer
    };

}

#endif /* SkinnedMeshRenderer_hpp */
