#include "AssetDataBase.hpp"
#include <Debug.hpp>
#include <Texture.hpp>
#include <AssetImporter.hpp>

using namespace FishEngine;

namespace FishEditor
{
    std::map<FishEngine::Path, QIcon> AssetDatabase::m_cacheIcons;
	
	const QIcon & AssetDatabase::GetCacheIcon(FishEngine::Path path)
	{
        static QIcon unknown_icon(":/Resources/unknown_file.png");

        auto type = FishEngine::Resources::GetAssetType(path.extension());
        if (type != FishEngine::AssetType::Texture)
        {
            return unknown_icon;
        }
		auto it = m_cacheIcons.find(path);
		if (it != m_cacheIcons.end())
		{
			return it->second;
		}
		m_cacheIcons.emplace(path, QIcon(QString::fromStdString(path.string())));
		return m_cacheIcons[path];
	}

	template <>
	static std::shared_ptr<Texture>
		AssetDatabase::LoadAssetAtPath(FishEngine::Path const & path)
	{
		auto importer = AssetImporter::GetAtPath(path);
		return AssetImporter::s_importerGuidToTexture[importer->GetGUID()];
	}
}