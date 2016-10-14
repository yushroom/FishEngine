#ifndef Renderer_hpp
#define Renderer_hpp

#include "Component.hpp"
#include "Material.hpp"
#include "Bounds.hpp"

namespace FishEngine
{
    class Renderer : public Component
    {
    public:
        InjectClassName(Renderer)

            Renderer() = default;

        Renderer(PMaterial material)
        {
            m_materials.push_back(material);
        }

        virtual void Render() const = 0;

        void AddMaterial(PMaterial material) {
            m_materials.push_back(material);
        }

        PMaterial material() const {
            return m_materials.size() > 0 ? m_materials[0] : nullptr;
        }

        std::vector<PMaterial>& materials() {
            return m_materials;
        }

        void SetMaterial(PMaterial material) {
            m_materials.clear();
            m_materials.push_back(material);
        }

        //virtual void OnInspectorGUI() override;

    protected:
        friend class FishEditor::EditorGUI;
        std::vector<PMaterial> m_materials;
    };
}

#endif /* Renderer_hpp */
