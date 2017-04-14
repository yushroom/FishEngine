#pragma once

#include "AssetImporter.hpp"
#include "Shader.hpp"

namespace FishEditor
{
	class ShaderImporter final : public AssetImporter
	{
	public:
		InjectClassName(ShaderImporter);

		ShaderImporter() = default;

		FishEngine::ShaderPtr GetShader()
		{
			return FishEngine::As<FishEngine::Shader>(m_asset->m_assetObjects.front());
		}

		FishEngine::ShaderPtr Load();
	};
}