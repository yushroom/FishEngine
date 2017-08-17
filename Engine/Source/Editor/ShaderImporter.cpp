#include "ShaderImporter.hpp"
#include <FishEngine/Shader.hpp>

FishEngine::ShaderPtr FishEditor::ShaderImporter::Load()
{
	auto shader = FishEngine::Shader::CreateFromFile(m_assetPath);
	m_asset->Add( shader );
	return shader;
}

