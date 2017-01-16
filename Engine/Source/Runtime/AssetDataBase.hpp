#pragma once

#include "Resources.hpp"

namespace FishEngine
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
		static void CreateAsset(Object asset, Path const & path);
		static Path CreateFolder(Path const & parentFolder, Path const & newFolderName);
		static bool DeleteAsset(Path const & path);
		
		static void ImportAsset(Path const & path, ImportAssetOptions options = ImportAssetOptions::Default);
    };
}
