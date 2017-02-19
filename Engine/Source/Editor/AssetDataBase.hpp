#pragma once

#include <set>
#include <Resources.hpp>
#include <QIcon>
#include <ReflectClass.hpp>

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
		
		static FishEngine::Path AssetPathToGUID(FishEngine::Path const & path);
        static FishEngine::Path GUIDToAssetPath(boost::uuids::uuid const & guid);
		
		static bool CopyAsset(FishEngine::Path const & path, FishEngine::Path const & newPath);
		static void CreateAsset(FishEngine::ObjectPtr asset, FishEngine::Path const & path);
		static FishEngine::Path CreateFolder(FishEngine::Path const & parentFolder, FishEngine::Path const & newFolderName);
		static bool DeleteAsset(FishEngine::Path const & path);
		
		//Returns the path name relative to the project folder where the asset is stored.
		//All paths are relative to the project folder, for example: "Assets/MyTextures/hello.png".
		static std::string GetAssetPath(int instanceID);
		static std::string GetAssetPath(FishEngine::ObjectPtr assetObject);
		
        //static TexturePtr GetCacheIcon(std::string const & path);
		static QIcon const & GetCacheIcon(FishEngine::Path path);
		
		static void ImportAsset(FishEngine::Path const & path, ImportAssetOptions options = ImportAssetOptions::Default);

		// Returns an array of all asset objects at assetPath.
		// All paths are relative to the project folder, for example: "Assets/MyTextures/hello.png".
		template <class T>
		static std::shared_ptr<T> LoadAssetAtPath(FishEngine::Path const & path);

        static std::map<FishEngine::Path, QIcon> s_cacheIcons;

		static std::set<std::shared_ptr<FishEngine::Object>> s_allAssetObjects;
    };

	// texture
	template <>
	std::shared_ptr<FishEngine::Texture> AssetDatabase::LoadAssetAtPath(FishEngine::Path const & path);

	// 3d model
	template <>
	std::shared_ptr<FishEngine::GameObject> AssetDatabase::LoadAssetAtPath(FishEngine::Path const & path);

	//template <>
	//static std::shared_ptr<Texture> AssetDatabase::LoadAssetAtPath(Path const & path);
}
