#ifndef MeshRenderer_hpp
#define MeshRenderer_hpp

#include "Renderer.hpp"

NAMESPACE_FISHENGINE_BEGIN

class MeshRenderer : public Renderer
{
public:
    InjectClassName(MeshRenderer)
    
    MeshRenderer() = default;
    
    MeshRenderer(std::shared_ptr<Material> material);
    
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

NAMESPACE_FISHENGINE_END

#endif /* MeshRenderer_hpp */
