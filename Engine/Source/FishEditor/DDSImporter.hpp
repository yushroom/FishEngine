#pragma once

#include "AssetImporter.hpp"

namespace FishEditor
{
	class DDSImporter final : public AssetImporter
	{
	public:

		InjectClassName(DDSImporter);

		DDSImporter() = default;

		FishEngine::TexturePtr Load();
	};
}