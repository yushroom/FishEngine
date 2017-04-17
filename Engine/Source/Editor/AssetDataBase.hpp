#pragma once

#include <set>
#include <Resources.hpp>
#include <QIcon>
#include <ReflectClass.hpp>
#include <GUID.hpp>
#include <AssetImporter.hpp>
#include <Application.hpp>
#include <Debug.hpp>

namespace FishEngine
{
	class Texture;
};

namespace FishEditor
{
	enum class ImportAssetOptions
	{
		// Default import options.
		Default,
		
		// User initiated asset import.
		ForceUpdate,
		
		// Import all assets synchronously.
		ForceSynchronousImport,
		
		// When a folder is imported, import all its contents as well.
		ImportRecursive,
		
		// Force a full reimport but don't download the assets from the cache server.
		DontDownloadFromCacheServer,
		
		// Forces asset import as uncompressed for edition facilities.
		ForceUncompressedImport,
	};
	
	
	class Meta(NonSerializable) AssetDatabase
	{
	public:
		AssetDatabase() = delete;
		
		// Get the GUID for the asset at path.
		static FishEngine::GUID AssetPathToGUID(FishEngine::Path const & path);

		// Translate a GUID to its current asset path.
		//static FishEngine::Path GUIDToAssetPath(boost::uuids::uuid const & guid);
		
		static bool CopyAsset(FishEngine::Path const & path, FishEngine::Path const & newPath);
		static void CreateAsset(FishEngine::ObjectPtr asset, FishEngine::Path const & path);
		static FishEngine::Path CreateFolder(FishEngine::Path const & parentFolder, FishEngine::Path const & newFolderName);
		static bool DeleteAsset(FishEngine::Path const & path);
		
		//Returns the path name relative to the project folder where the asset is stored.
		//All paths are relative to the project folder, for example: "Assets/MyTextures/hello.png".
		static FishEngine::Path GetAssetPath(int instanceID);
		static FishEngine::Path GetAssetPath(FishEngine::ObjectPtr assetObject);
		
		//static TexturePtr GetCacheIcon(std::string const & path);
		static QIcon const & GetCacheIcon(FishEngine::Path path);
		
		static void ImportAsset(FishEngine::Path const & path, ImportAssetOptions options = ImportAssetOptions::Default);
		
		// Returns an array of all asset objects at assetPath.
		// All paths are relative to the project folder, for example: "Assets/MyTextures/hello.png".
		
		// Returns the first asset object of type type at given path assetPath.
		static FishEngine::ObjectPtr LoadAssetAtPath(FishEngine::Path const & path);

		template<class T>
		static std::shared_ptr<T> LoadAssetAtPath2(FishEngine::Path const & path);
		
//		template <class T>
//		static std::shared_ptr<T> LoadAssetAtPath(FishEngine::Path const & path)
//		{
//			return FishEngine::As<T>( LoadAssetAtPath(path) );
//		}

		static std::map<FishEngine::Path, QIcon> s_cacheIcons;

		static std::set<std::shared_ptr<FishEngine::Object>> s_allAssetObjects;
	};



	template<class T>
	std::shared_ptr<T> AssetDatabase::LoadAssetAtPath2(FishEngine::Path const & path)
	{
		static_assert(std::is_base_of<FishEngine::Object, T>::value, "Object only");
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
		//return AssetImporter::s_importerGUIDToObject[importer->GetGUID()]->mainObject();
		for (auto & obj : importer->asset()->m_assetObjects)
		{
			if (FishEngine::IsSubClassOf<T>( obj->ClassID() ))
				return FishEngine::As<T>(obj);
		}
		abort();
		return nullptr;
	}
}
