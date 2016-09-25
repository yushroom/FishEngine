#ifndef Renderer_hpp
#define Renderer_hpp

#include "Component.hpp"
#include "Material.hpp"

NAMESPACE_FISHENGINE_BEGIN

class Renderer : public Component
{
public:
    InjectClassName(Renderer)
    
    Renderer() = default;
    
    Renderer(std::shared_ptr<Material> material)
    {
        m_materials.push_back(material);
    }
    
    void AddMaterial(std::shared_ptr<Material> material) {
        m_materials.push_back(material);
    }
    
    std::shared_ptr<Material> material() const {
        return m_materials.size() > 0 ? m_materials[0] : nullptr;
    }
    
    std::vector<std::shared_ptr<Material>>& materials() {
        return m_materials;
    }

    void SetMaterial(std::shared_ptr<Material> material) {
        m_materials.clear();
        m_materials.push_back(material);
    }
    
    virtual void OnInspectorGUI() override;
    
protected:
    virtual void Render() const = 0;
    std::vector<std::shared_ptr<Material>> m_materials;
};

NAMESPACE_FISHENGINE_END

#endif /* Renderer_hpp */
