#include "SkinnedMeshRenderer.hpp"

#include <cassert>

#include "GameObject.hpp"
#include "Debug.hpp"
#include "Scene.hpp"
#include "Mesh.hpp"
#include "Light.hpp"
#include "Animator.hpp"
#include "Pipeline.hpp"
#include "Camera.hpp"
#include "Gizmos.hpp"
#include "Shader.hpp"
#include "Graphics.hpp"

namespace FishEngine
{
	SkinnedMeshRenderer::
		SkinnedMeshRenderer(MaterialPtr material)
		: Renderer(material)
	{

	}


	Bounds SkinnedMeshRenderer::
		localBounds() const {
		return m_sharedMesh->bounds();
	}

#if 0

	void RecursivelyGetTransformation(
		const TransformPtr&                   transform,
		const std::map<std::string, int>&   nameToIndex,
		std::vector<Matrix4x4>&             outMatrixPalette)
	{
		const auto& name = transform->name();
		const auto& it = nameToIndex.find(name);
		if (it != nameToIndex.end())
		{
			const auto boneIndex = it->second;
			//outMatrixPalette[boneIndex] = transform->localToWorldMatrixFast();
			outMatrixPalette[boneIndex] = transform->localToWorldMatrix();
		}
		for (auto& child : transform->children())
		{
			RecursivelyGetTransformation(child.lock(), nameToIndex, outMatrixPalette);
		}
	}

#endif

	void SkinnedMeshRenderer::setSharedMesh(MeshPtr sharedMesh)
	{
		m_sharedMesh = sharedMesh;
		m_matrixPalette.resize(m_sharedMesh->boneCount());
	}

	void SkinnedMeshRenderer::UpdateMatrixPalette() const
	{
		m_matrixPalette.resize(m_sharedMesh->boneCount());
		//RecursivelyGetTransformation(m_rootBone.lock(), m_avatar->m_boneToIndex, m_matrixPalette);
		const auto& worldToLocal = gameObject()->transform()->worldToLocalMatrix();
		const auto& bindposes = m_sharedMesh->bindposes();
		for (uint32_t i = 0; i < m_matrixPalette.size(); ++i)
		{
			auto bone = m_bones[i].lock();
			auto& mat = m_matrixPalette[i];
			// we multiply worldToLocal because we assume that the mesh is in local space in shader.
			mat = worldToLocal * bone->localToWorldMatrix() * bindposes[i];

			// macOS bug
			// see the definition of Bones in ShaderVariables.inc
			mat = mat.transpose();
		}
	}

	void SkinnedMeshRenderer::Update()
	{
		UpdateMatrixPalette();
	}


	void SkinnedMeshRenderer::Render() const
	{
		auto model = transform()->localToWorldMatrix();
		Pipeline::UpdatePerDrawUniforms(model);

		UpdateMatrixPalette();

		Pipeline::UpdateBonesUniforms(m_matrixPalette);

		for (auto& material : m_materials)
		{
			material->EnableKeyword(ShaderKeyword::SkinnedAnimation);
			Graphics::DrawMesh(m_sharedMesh, material);
			material->DisableKeyword(ShaderKeyword::SkinnedAnimation);
		}
	}

	void SkinnedMeshRenderer::OnDrawGizmosSelected()
	{
		Gizmos::setColor(Color::white);
		Gizmos::setMatrix(transform()->localToWorldMatrix());
		Bounds b = localBounds();
		Gizmos::DrawWireCube(b.center(), b.size());
	}
}
