#ifndef SkinnedMeshRenderer_hpp
#define SkinnedMeshRenderer_hpp

#include "Renderer.hpp"

namespace FishEngine {
    
    class SkinnedMeshRenderer : public Renderer
    {
    public:
        InjectClassName(SkinnedMeshRenderer);
        
        SkinnedMeshRenderer() = default;
        
        SkinnedMeshRenderer(std::shared_ptr<Material> material);
        
        virtual void Render() const override;
        
        const std::vector<std::weak_ptr<Transform>>& bones() const {
            return m_bones;
        }
        
    private:
        friend class ModelImporter;
        std::vector<std::weak_ptr<Transform>> m_bones;
    };
}

#endif /* SkinnedMeshRenderer_hpp */
