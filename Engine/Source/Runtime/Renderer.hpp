#ifndef Renderer_hpp
#define Renderer_hpp

#include "Component.hpp"
#include "Material.hpp"
#include "Bounds.hpp"

namespace FishEngine
{
    enum class ShadowCastingMode
    {
        Off,
        On,
        TwoSided,
        ShdowsOnly,
    };

    class FE_EXPORT Renderer : public Component
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

        ShadowCastingMode shadowCastingMode() const
        {
            return m_shadowCastingMode;
        }

        void setShadowCastingMode(ShadowCastingMode shadowCastingMode)
        {
            m_shadowCastingMode = shadowCastingMode;
        }

    protected:
        friend class Serialization;
        friend class FishEditor::EditorGUI;
        std::vector<MaterialPtr> m_materials;

        ShadowCastingMode m_shadowCastingMode = ShadowCastingMode::On;
    };
}

#endif /* Renderer_hpp */
