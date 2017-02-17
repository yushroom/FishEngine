#pragma once

#include "FishEditor.hpp"
#include "ModelImporter.hpp"
#include <boost/filesystem.hpp>

namespace FishEditor
{
	class FBXImporter : public ModelImporter
	{
	public:
		FBXImporter() = default;
		
		FishEngine::GameObjectPtr Load(boost::filesystem::path const & path);
	};
}