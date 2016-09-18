#ifndef MeshFilter_hpp
#define MeshFilter_hpp

#include "Component.hpp"

NAMESPACE_FISHENGINE_BEGIN

class MeshFilter : public Component
{
public:
    InjectClassName(MeshFilter)
    
    MeshFilter() = default;
    
    MeshFilter(std::shared_ptr<Mesh> mesh) : m_mesh(mesh) {
        
    }
    
    std::shared_ptr<Mesh> mesh() const {
        return m_mesh;
    }
    
    void SetMesh(std::shared_ptr<Mesh> mesh) {
        m_mesh = mesh;
    }
    
    virtual void OnEditorGUI() override;
    
private:
    std::shared_ptr<Mesh> m_mesh = nullptr;
};

NAMESPACE_FISHENGINE_END

#endif /* MeshFilter_hpp */
