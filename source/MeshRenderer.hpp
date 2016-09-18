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
};

NAMESPACE_FISHENGINE_END

#endif /* MeshRenderer_hpp */
