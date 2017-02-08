#pragma once

#include <Resources.hpp>
#include <QIcon>

using Path = FishEngine::Path;

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
	
    class AssetDatabase
    {
    public:
        AssetDatabase() = delete;
		
		static Path AssetPathToGUID(Path const & path);
		static Path GUIDToAssetPath(Path const & guid);
		
		static bool CopyAsset(Path const & path, Path const & newPath);
		static void CreateAsset(FishEngine::ObjectPtr asset, Path const & path);
		static Path CreateFolder(Path const & parentFolder, Path const & newFolderName);
		static bool DeleteAsset(Path const & path);
		
		//Returns the path name relative to the project folder where the asset is stored.
		//All paths are relative to the project folder, for example: "Assets/MyTextures/hello.png".
		static std::string GetAssetPath(int instanceID);
		static std::string GetAssetPath(FishEngine::ObjectPtr assetObject);
		
        //static TexturePtr GetCacheIcon(std::string const & path);
		static QIcon const & GetCacheIcon(Path path);
		
		static void ImportAsset(Path const & path, ImportAssetOptions options = ImportAssetOptions::Default);

        static std::map<Path, QIcon> m_cacheIcons;
    };
}
