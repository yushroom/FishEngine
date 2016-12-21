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

    // enum count
    template<>
    constexpr int EnumCount<ShadowCastingMode>() { return 4; }

    // string array
    static const char* ShadowCastingModeStrings[] = {
        "Off",
        "On",
        "TwoSided",
        "ShdowsOnly",
    };

    // cstring array
    template<>
    inline constexpr const char** EnumToCStringArray<ShadowCastingMode>()
    {
        return ShadowCastingModeStrings;
    }

    // index to enum
    template<>
    inline ShadowCastingMode ToEnum<ShadowCastingMode>(const int index)
    {
        switch (index) {
        case 0: return ShadowCastingMode::Off; break;
        case 1: return ShadowCastingMode::On; break;
        case 2: return ShadowCastingMode::TwoSided; break;
        case 3: return ShadowCastingMode::ShdowsOnly; break;
        default: abort(); break;
        }
    }

    // enum to index
    template<>
    inline int EnumToIndex<ShadowCastingMode>(ShadowCastingMode e)
    {
        switch (e) {
        case ShadowCastingMode::Off: return 0; break;
        case ShadowCastingMode::On: return 1; break;
        case ShadowCastingMode::TwoSided: return 2; break;
        case ShadowCastingMode::ShdowsOnly: return 3; break;
        default: abort(); break;
        }
    }

    // string to enum
    template<>
    inline ShadowCastingMode ToEnum<ShadowCastingMode>(const std::string& s)
    {
        if (s == "Off") return ShadowCastingMode::Off;
        if (s == "On") return ShadowCastingMode::On;
        if (s == "TwoSided") return ShadowCastingMode::TwoSided;
        if (s == "ShdowsOnly") return ShadowCastingMode::ShdowsOnly;
        abort();
    }

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
        friend class FishEditor::EditorGUI;
        std::vector<MaterialPtr> m_materials;

        ShadowCastingMode m_shadowCastingMode = ShadowCastingMode::On;
    };
}

#endif /* Renderer_hpp */
