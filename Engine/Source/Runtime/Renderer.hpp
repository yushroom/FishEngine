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
        InjectClassName(Renderer);

        Renderer() = default;

        Renderer(MaterialPtr material)
        {
            m_materials.push_back(material);
        }

        virtual void Render() const = 0;

        void AddMaterial(MaterialPtr material) {
            m_materials.push_back(material);
        }

        MaterialPtr material() const {
            return m_materials.size() > 0 ? m_materials[0] : nullptr;
        }

        std::vector<MaterialPtr>& materials() {
            return m_materials;
        }

        void SetMaterial(MaterialPtr material) {
            m_materials.clear();
            m_materials.push_back(material);
        }

        virtual Bounds localBounds() const = 0;
        Bounds bounds() const;

        //virtual void OnInspectorGUI() override;
        virtual void OnDrawGizmosSelected() override;

    protected:
        friend class FishEditor::EditorGUI;
        std::vector<MaterialPtr> m_materials;
    };
}

#endif /* Renderer_hpp */
