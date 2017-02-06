#include "AssetImporter.hpp"
#include "AssetDataBase.hpp"

namespace FishEngine
{

	void AssetImporter::SaveAndReimport()
	{
		AssetDatabase::ImportAsset(m_assetPath);
	}

	std::shared_ptr<AssetImporter> AssetImporter::GetAtPath(Path const & path)
	{
		auto const & it = s_pathToImpoter.find(path);
		if (it != s_pathToImpoter.end())
		{
			return it->second;
		}
	}

	std::map<boost::filesystem::path, std::shared_ptr<AssetImporter>> AssetImporter::s_pathToImpoter;

}

