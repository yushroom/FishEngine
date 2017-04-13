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
		DefineComponent(Renderer);

		Renderer() = default;

		Renderer(MaterialPtr material)
		{
			m_materials.push_back(material);
		}

		virtual void PreRender() const = 0;
		//virtual void Render() const = 0;

		void AddMaterial(MaterialPtr material)
		{
			m_materials.push_back(material);
		}

		MaterialPtr material() const
		{
			return m_materials.size() > 0 ? m_materials[0] : nullptr;
		}

		std::vector<MaterialPtr>& materials()
		{
			return m_materials;
		}

		void SetMaterial(MaterialPtr material)
		{
			//m_materials.clear();
			if (m_materials.empty())
				m_materials.push_back(material);
			else
				m_materials[0] = material;
		}

		virtual Bounds localBounds() const = 0;
		Bounds bounds() const;

		// Makes the rendered 3D object visible if enabled.
		bool enabled() const
		{
			return m_enabled;
		}

		// Makes the rendered 3D object visible if enabled.
		void setEnabled(bool enabled)
		{
			m_enabled = enabled;
		}

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
		
		void setReceiveShadows(bool value)
		{
			m_receiveShadows = value;
		}

	protected:
		friend class FishEditor::Inspector;
		friend class FishEditor::EditorGUI;
		bool m_enabled = true;	// Makes the rendered 3D object visible if enabled.
		std::vector<MaterialPtr> m_materials;

		ShadowCastingMode	m_shadowCastingMode = ShadowCastingMode::On;
		bool				m_receiveShadows = true;
	};
}

#endif /* Renderer_hpp */
