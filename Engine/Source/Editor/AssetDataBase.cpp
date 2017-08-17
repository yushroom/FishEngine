#include "AssetDataBase.hpp"
#include <FishEngine/Debug.hpp>
#include <FishEngine/Texture.hpp>
#include "AssetImporter.hpp"
#include <FishEngine/Application.hpp>

using namespace FishEngine;

namespace FishEditor
{
	std::map<FishEngine::Path, QIcon> AssetDatabase::s_cacheIcons;

	std::set<std::shared_ptr<FishEngine::Object>> AssetDatabase::s_allAssetObjects;

	FishEngine::GUID AssetDatabase::AssetPathToGUID(FishEngine::Path const & path)
	{
		return AssetImporter::s_pathToImpoter[path]->GetGUID();
	}

	//Path AssetDatabase::GUIDToAssetPath(const boost::uuids::uuid &guid)
 //   {
 //       return AssetImporter::s_objectInstanceIDToPath[guid];
 //   }

	FishEngine::Path AssetDatabase::GetAssetPath(int instanceID)
	{
		return AssetImporter::s_objectInstanceIDToPath[instanceID];
	}

	FishEngine::Path AssetDatabase::GetAssetPath(FishEngine::ObjectPtr assetObject)
	{
		return GetAssetPath(assetObject->GetInstanceID());
	}

	const QIcon & AssetDatabase::GetCacheIcon(FishEngine::Path path)
	{
		static QIcon default_icon(  ":/Resources/Assets/DefaultAsset@64.png");
		static QIcon material_icon( ":/Resources/Assets/Material@64.png");
		static QIcon model_icon(    ":/Resources/Assets/PrefabModel@64.png");
		static QIcon prefab_icon(   ":/Resources/Assets/Prefab@64.png");
		static QIcon shader_icon(   ":/Resources/Assets/Shader@64.png");
		static QIcon audioclip_icon(":/Resources/Assets/AudioClip@64.png");
		static QIcon folder_icon(   ":/Resources/Assets/Folder@64.png");

		if (boost::filesystem::is_directory(path))
		{
			return folder_icon;
		}
		
		auto type = FishEngine::Resources::GetAssetType(path.extension());
		if (type == FishEngine::AssetType::Material)
		{
			return material_icon;
		}
		else if (type == FishEngine::AssetType::Shader)
		{
			return shader_icon;
		}
		else if (type == FishEngine::AssetType::Prefab)
		{
			return prefab_icon;
		}
		else if (type == FishEngine::AssetType::Model)
		{
			return model_icon;
		}
		else if (type == FishEngine::AssetType::AudioClip)
		{
			return audioclip_icon;
		}
		else if (type == FishEngine::AssetType::Texture)
		{
			auto it = s_cacheIcons.find(path);
			if (it != s_cacheIcons.end())
			{
				return it->second;
			}
//			s_cacheIcons.emplace(path, QIcon(QString::fromStdString(path.string())));
//			return s_cacheIcons[path];
		}
		return default_icon;
	}


	FishEngine::ObjectPtr AssetDatabase::LoadAssetAtPath(FishEngine::Path const & path)
	{
		FishEngine::Path p;
		if (path.is_absolute())
		{
			LogWarning("AssetDatabase::LoadAssetAtPath, path should be relative to project root dir, eg. Assets/a.fbx");
			p = path;
		}
		else
		{
			p = FishEngine::Application::dataPath().parent_path() / path;
		}
		if (!boost::filesystem::exists(p))
		{
			return nullptr;
		}
		auto importer = AssetImporter::GetAtPath(p);
		if (importer == nullptr)
			return nullptr;
		//return As<T>( AssetImporter::s_importerGUIDToObject[importer->GetGUID()] );
		return importer->asset()->mainObject();
	}

}
