#pragma once

#include <ReflectEnum.hpp>
#include <AssetDataBase.hpp>

namespace FishEngine
{


/**************************************************
* FishEditor::ImportAssetOptions
**************************************************/

// enum count
template<>
constexpr int EnumCount<FishEditor::ImportAssetOptions>() { return 6; }

// string array
static const char* ImportAssetOptionsStrings[] =
{
    "Default",
	"ForceUpdate",
	"ForceSynchronousImport",
	"ImportRecursive",
	"DontDownloadFromCacheServer",
	"ForceUncompressedImport"
};

// cstring array
template<>
inline constexpr const char** EnumToCStringArray<FishEditor::ImportAssetOptions>()
{
    return ImportAssetOptionsStrings;
}

// index to enum
template<>
inline FishEditor::ImportAssetOptions ToEnum<FishEditor::ImportAssetOptions>(const int index)
{
    switch (index) {
    case 0: return FishEditor::ImportAssetOptions::Default; break;
	case 1: return FishEditor::ImportAssetOptions::ForceUpdate; break;
	case 2: return FishEditor::ImportAssetOptions::ForceSynchronousImport; break;
	case 3: return FishEditor::ImportAssetOptions::ImportRecursive; break;
	case 4: return FishEditor::ImportAssetOptions::DontDownloadFromCacheServer; break;
	case 5: return FishEditor::ImportAssetOptions::ForceUncompressedImport; break;
	
    default: abort(); break;
    }
}

// enum to index
template<>
inline int EnumToIndex<FishEditor::ImportAssetOptions>(FishEditor::ImportAssetOptions e)
{
    switch (e) {
    case FishEditor::ImportAssetOptions::Default: return 0; break;
	case FishEditor::ImportAssetOptions::ForceUpdate: return 1; break;
	case FishEditor::ImportAssetOptions::ForceSynchronousImport: return 2; break;
	case FishEditor::ImportAssetOptions::ImportRecursive: return 3; break;
	case FishEditor::ImportAssetOptions::DontDownloadFromCacheServer: return 4; break;
	case FishEditor::ImportAssetOptions::ForceUncompressedImport: return 5; break;
	
    default: abort(); break;
    }
}

// string to enum
template<>
inline FishEditor::ImportAssetOptions ToEnum<FishEditor::ImportAssetOptions>(const std::string& s)
{
    if (s == "Default") return FishEditor::ImportAssetOptions::Default;
	if (s == "ForceUpdate") return FishEditor::ImportAssetOptions::ForceUpdate;
	if (s == "ForceSynchronousImport") return FishEditor::ImportAssetOptions::ForceSynchronousImport;
	if (s == "ImportRecursive") return FishEditor::ImportAssetOptions::ImportRecursive;
	if (s == "DontDownloadFromCacheServer") return FishEditor::ImportAssetOptions::DontDownloadFromCacheServer;
	if (s == "ForceUncompressedImport") return FishEditor::ImportAssetOptions::ForceUncompressedImport;
	
    abort();
}


} // namespace FishEngine
