#ifndef MeshRenderer_hpp
#define MeshRenderer_hpp

#include "Renderer.hpp"

namespace FishEngine
{
	class FE_EXPORT MeshRenderer : public Renderer
	{
	public:
		DefineComponent(MeshRenderer);

		MeshRenderer() = default;

		MeshRenderer(MaterialPtr material);

		virtual void PreRender() const override;
		//virtual void Render() const override;

		virtual Bounds localBounds() const override;
		virtual void OnDrawGizmosSelected() override;

	private:
		friend class FishEditor::Inspector;
	};
}

#endif /* MeshRenderer_hpp */
