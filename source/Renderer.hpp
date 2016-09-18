#ifndef Renderer_hpp
#define Renderer_hpp

#include "Component.hpp"
#include "Material.hpp"
#include "imgui/imgui.h"

NAMESPACE_FISHENGINE_BEGIN

class Renderer : public Component
{
public:
    InjectClassName(Renderer)
    
    Renderer() = default;
    
    Renderer(Material::PMaterial material)
    {
        m_materials.push_back(material);
    }
    
    void AddMaterial(std::shared_ptr<Material> material) {
        m_materials.push_back(material);
    }
    
    Material::PMaterial material() const {
        if (m_materials.size() > 0)
            return m_materials[0];
        return nullptr;
    }
    
    std::vector<Material::PMaterial>& materials() {
        return m_materials;
    }
    
    virtual void OnEditorGUI() override {
        for (auto& m : m_materials) {
            m->OnEditorGUI();
        }
    }
    
protected:
    virtual void Render() const = 0;
    std::vector<Material::PMaterial> m_materials;
};

NAMESPACE_FISHENGINE_END

#endif /* Renderer_hpp */
