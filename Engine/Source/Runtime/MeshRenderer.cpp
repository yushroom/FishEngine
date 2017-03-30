#include "MeshRenderer.hpp"

#include <cassert>

#include "GameObject.hpp"
#include "Debug.hpp"
#include "Scene.hpp"
#include "MeshFilter.hpp"
#include "Mesh.hpp"
//#include "Light.hpp"
//#include "Animator.hpp"
#include "Pipeline.hpp"
#include "Camera.hpp"
//#include "Gizmos.hpp"
//#include "Shader.hpp"
#include "Graphics.hpp"


namespace FishEngine
{
	MeshRenderer::MeshRenderer(MaterialPtr material) : Renderer(material)
	{

	}

	Bounds MeshRenderer::localBounds() const
	{
		auto mf = gameObject()->GetComponent<MeshFilter>();
		if (mf == nullptr || mf->mesh() == nullptr)
			return Bounds();
		return mf->mesh()->bounds();
	}

	void MeshRenderer::Render() const
	{
		auto meshFilter = gameObject()->GetComponent<MeshFilter>();
		if (meshFilter == nullptr) {
			Debug::LogWarning("This GameObject has no MeshFilter");
			return;
		}

		auto model = transform()->localToWorldMatrix();
		Pipeline::UpdatePerDrawUniforms(model);

		const auto& mesh = meshFilter->mesh();
//		for (auto& m : m_materials)
//		{
//			Graphics::DrawMesh(mesh, m);
//		}
		
		auto materialCount = m_materials.size();
		auto submeshCount = mesh->subMeshCount();
		
		if (materialCount == submeshCount)
		{
			for (int i = 0; i < materialCount; ++i)
			{
				auto & m = m_materials[i];
				Graphics::DrawMesh(mesh, m, i);
			}
		}
		else
		{
			Debug::LogWarning("[MeshRenderer::Render] Material size and submesh count not match");
			if (materialCount < submeshCount)
			{
				int materialId = 0;
				for (int i = 0; i < submeshCount; ++i)
				{
					auto & mat = m_materials[materialId];
					materialId = (materialId + 1) % materialCount;
					Graphics::DrawMesh(mesh, mat, i);
				}
			}
			else // materialCount > mesh->subMeshCount()
			{
				int submeshId = 0;
				for (auto & mat : m_materials)
				{
					Graphics::DrawMesh(mesh, mat, submeshId);
					submeshId = (submeshId + 1) % submeshCount;
				}
			}
		}
	}

	void MeshRenderer::OnDrawGizmosSelected()
	{
		//Gizmos::setColor(Color::blue);
		//auto b = bounds();
		//Gizmos::DrawWireCube(b.center(), b.size());
	}
}
