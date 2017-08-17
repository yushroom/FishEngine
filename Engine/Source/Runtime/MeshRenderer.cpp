#include <FishEngine/MeshRenderer.hpp>

#include <cassert>

#include <FishEngine/GameObject.hpp>
#include <FishEngine/Debug.hpp>
#include <FishEngine/Scene.hpp>
#include <FishEngine/MeshFilter.hpp>
#include <FishEngine/Mesh.hpp>
//#include "Light.hpp"
//#include "Animator.hpp"
#include <FishEngine/Pipeline.hpp>
#include <FishEngine/Camera.hpp>
//#include "Gizmos.hpp"
//#include "Shader.hpp"
#include <FishEngine/Graphics.hpp>


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


	//void MeshRenderer::PreRender() const
	//{
	//	auto model = transform()->localToWorldMatrix();
	//	Pipeline::UpdatePerDrawUniforms(model);
	//}

#if 0
	void MeshRenderer::Render() const
	{
		auto meshFilter = gameObject()->GetComponent<MeshFilter>();
		if (meshFilter == nullptr) {
			LogWarning("This GameObject has no MeshFilter");
			return;
		}

		PreRender();

		const auto& mesh = meshFilter->mesh();
		auto materialCount = m_materials.size();
		auto submeshCount = mesh->subMeshCount();
		
		if (materialCount == submeshCount)
		{
			for (int i = 0; i < materialCount; ++i)
			{
				auto & material = m_materials[i];
				Graphics::DrawMesh(mesh, material, i);
			}
		}
		else
		{
			LogWarning("Material size and submesh count mismatch");
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
#endif

	void MeshRenderer::OnDrawGizmosSelected()
	{
		//Gizmos::setColor(Color::blue);
		//auto b = bounds();
		//Gizmos::DrawWireCube(b.center(), b.size());
	}
}
