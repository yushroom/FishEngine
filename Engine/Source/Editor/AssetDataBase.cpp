#include "AssetDataBase.hpp"
#include <Debug.hpp>
#include <Texture.hpp>
#include <AssetImporter.hpp>
#include <Application.hpp>

using namespace FishEngine;

namespace FishEditor
{
    std::map<FishEngine::Path, QIcon> AssetDatabase::s_cacheIcons;

	std::set<std::shared_ptr<FishEngine::Object>> AssetDatabase::s_allAssetObjects;

	//Path AssetDatabase::GUIDToAssetPath(const boost::uuids::uuid &guid)
 //   {
 //       return AssetImporter::s_objectInstanceIDToPath[guid];
 //   }

	FishEngine::Path AssetDatabase::GetAssetPath(int instanceID)
	{
		return AssetImporter::s_objectInstanceIDToPath[instanceID];
	}

	const QIcon & AssetDatabase::GetCacheIcon(FishEngine::Path path)
	{
        static QIcon unknown_icon(":/Resources/unknown_file.png");

        auto type = FishEngine::Resources::GetAssetType(path.extension());
        if (type != FishEngine::AssetType::Texture)
        {
            return unknown_icon;
        }
		auto it = s_cacheIcons.find(path);
		if (it != s_cacheIcons.end())
		{
			return it->second;
		}
		s_cacheIcons.emplace(path, QIcon(QString::fromStdString(path.string())));
		return s_cacheIcons[path];
	}

	template <>
	std::shared_ptr<Texture>
		AssetDatabase::LoadAssetAtPath(FishEngine::Path const & path)
	{
		auto p = FishEngine::Applicaiton::dataPath().parent_path() / path;
		if (!boost::filesystem::exists(p))
		{
			return nullptr;
		}
		auto importer = AssetImporter::GetAtPath(p);
		return AssetImporter::s_importerGUIDToTexture[importer->GetGUID()];
	}

	template <>
	std::shared_ptr<FishEngine::GameObject> 
		AssetDatabase::LoadAssetAtPath(FishEngine::Path const & path)
	{
		Path p;
		if (path.is_absolute())
		{
			Debug::LogWarning("AssetDatabase::LoadAssetAtPath, path should be relative to project root dir, eg. Assets/a.fbx");
			p = path;
		}
		else
		{
			p = FishEngine::Applicaiton::dataPath().parent_path() / path;
		}
		if (!boost::filesystem::exists(p))
		{
			return nullptr;
		}
		auto importer = AssetImporter::GetAtPath(p);
		return AssetImporter::s_importerGUIDToModel[importer->GetGUID()];
	}

}
