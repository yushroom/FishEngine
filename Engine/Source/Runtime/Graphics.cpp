#include "Graphics.hpp"
#include "Pipeline.hpp"
#include "Material.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"
#include "Light.hpp"
#include "RenderSettings.hpp"
#include "RenderSystem.hpp"

namespace FishEngine
{
	void Graphics::DrawMesh(const MeshPtr& mesh, const Matrix4x4& matrix, const MaterialPtr& material)
	{
		Pipeline::UpdatePerDrawUniforms(matrix);
		DrawMesh(mesh, material);
	}

	void Graphics::DrawMesh(const MeshPtr& mesh, const MaterialPtr& material)
	{
		DrawMesh(mesh, material, -1);
	}
	
	void Graphics::DrawMesh(const MeshPtr& mesh, const MaterialPtr& material, int subMeshIndex)
	{
		//if (mesh->m_skinned)
		//{
		//	//material->EnableKeyword(ShaderKeyword::SkinnedAnimation);
		//	auto shader = Shader::FindBuiltin("Internal-BoneAnimation");
		//	shader->Use();
		//	shader->PreRender();
		//	shader->CheckStatus();
		//	mesh->RenderSkinned();
		//	shader->PostRender();
		//	glCheckError();
		//	//auto emitSync = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
		//	//glWaitSync(emitSync, 0, GL_TIMEOUT_IGNORED);
		//	//glDeleteSync(emitSync);
		//}
		//else
		//{
		//	//material->DisableKeyword(ShaderKeyword::SkinnedAnimation);
		//}
		
		auto shader = material->shader();
		if (shader->HasUniform("AmbientCubemap"))
		{
			//shader->BindTexture("AmbientCubemap", RenderSettings::ambientCubemap());
			material->SetTexture("AmbientCubemap", RenderSettings::ambientCubemap());
		}
		if (shader->HasUniform("PreIntegratedGF"))
		{
			//shader->BindTexture("PreIntegratedGF", RenderSettings::preintegratedGF());
			material->SetTexture("PreIntegratedGF", RenderSettings::preintegratedGF());
		}

		
		shader->Use();
		shader->PreRender();
		material->BindProperties();
		shader->CheckStatus();
		mesh->Render(subMeshIndex);
		shader->PostRender();
	}
}

