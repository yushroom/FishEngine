#include "AssetImporter.hpp"

//#include <iostream>

#include <GameObject.hpp>
#include <Timer.hpp>

#include "TextureImporter.hpp"
#include "FBXImporter.hpp"
#include "NativeFormatImporter.hpp"

#include "AssetArchive.hpp"
#include "SceneArchive.hpp"

#include <boost/uuid/uuid_generators.hpp>


using namespace FishEngine;

namespace FishEditor
{
	std::map<FishEngine::GUID, FishEngine::ObjectPtr> AssetImporter::s_importerGUIDToObject;
	std::map<int, FishEngine::Path> AssetImporter::s_objectInstanceIDToPath;
	std::map<FishEngine::Path, std::shared_ptr<AssetImporter>> AssetImporter::s_pathToImpoter;

	AssetImporter::AssetImporter()
		: m_guid(boost::uuids::random_generator()())
	{

	}

	void AssetImporter::SaveAndReimport()
	{
		//AssetDatabase::ImportAsset(m_assetPath);
		uint32_t time_created = static_cast<uint32_t>(time(NULL));
		m_assetTimeStamp = time_created;
		auto meta_path = m_assetPath.string() + ".meta";
		std::ofstream fout(meta_path);
		AssetOutputArchive archive(fout);
		archive.SerializeAssetImporter(*this);
		Reimport();
	}

	template<class AssetImporterType>
	std::shared_ptr<AssetImporterType> AssetImporter::GetAssetImporter(Path const & assetPath)
	{
		//bool need_generate = true;
		auto meta_path = assetPath.string() + ".meta";
		auto name = assetPath.stem().string();

#if 0
		if (boost::filesystem::exists(meta_path))
		{
			uint32_t asset_modified_time = static_cast<uint32_t>(boost::filesystem::last_write_time(assetPath));
			std::ifstream fin(meta_path);
			AssetInputArchive archive(fin);
			uint32_t meta_created_time;
			archive >> make_nvp("timeCreated", meta_created_time);
			GUID guid;
			archive >> make_nvp("guid", guid);
			if (asset_modified_time <= meta_created_time)
			{
				Debug::Log("Load .meta file: %s", meta_path.c_str());
				archive.ToNextNode();
				auto importer = archive.DeserializeObject<AssetImporterType>();
				importer->setName(name);
				importer->m_guid = guid;
				importer->m_assetTimeStamp = meta_created_time;
				importer->m_assetPath = assetPath;
				return importer;
			}
		}
#else
		if (boost::filesystem::exists(meta_path))
		{
			uint32_t asset_modified_time = static_cast<uint32_t>(boost::filesystem::last_write_time(assetPath));
			std::ifstream fin(meta_path);
			MetaInputArchive archive(fin);
			uint32_t meta_created_time = archive.timeStamp();
			if (asset_modified_time <= meta_created_time)
			{
				auto importer = archive.DeserializeAssetImporter();
				importer->setName(name);
				importer->m_assetPath = assetPath;
				return FishEngine::As<AssetImporterType>( importer );
			}
		}
#endif
		Debug::Log("Generate .meta file: %s", meta_path.c_str());
		auto importer = std::make_shared<AssetImporterType>();
		importer->setName(name);
		// do not set assetTimeStamp here
		importer->m_assetPath = assetPath;
		return importer;
	}

	std::shared_ptr<AssetImporter> AssetImporter::GetAtPath(Path path)
	{
		AssetImporterPtr ret = nullptr;
		path.make_preferred();
		auto const & it = s_pathToImpoter.find(path);
		if (it != s_pathToImpoter.end())
		{
			return it->second;
		}

		auto ext = path.extension();
		auto type = Resources::GetAssetType(ext);
		if (type == AssetType::Texture)
		{
			auto importer = GetAssetImporter<TextureImporter>(path);
			s_pathToImpoter[path] = importer;
			//Timer t(path.string());
			auto texture = importer->Import(path);
			texture->setName(path.stem().string());
			s_objectInstanceIDToPath[texture->GetInstanceID()] = path;
			s_importerGUIDToObject[importer->GetGUID()] = texture;
			//t.StopAndPrint();
			ret = importer;
		}
		else if (ext == ".fbx" || ext == ".FBX" || ext == ".obj")
		{
			Timer t(path.string());
			auto importer = GetAssetImporter<FBXImporter>(path);
			s_pathToImpoter[path] = importer;
			auto modelPrefab = importer->Load(path);
			modelPrefab->setName(path.stem().string());
			modelPrefab->rootGameObject()->setName(path.stem().string());
			s_objectInstanceIDToPath[modelPrefab->GetInstanceID()] = path;
			s_objectInstanceIDToPath[modelPrefab->rootGameObject()->GetInstanceID()] = path;
			s_importerGUIDToObject[importer->GetGUID()] = modelPrefab->rootGameObject();
			ret = importer;
			t.StopAndPrint();

			//std::ofstream fout(path.string() + ".prefab");
			//FishEditor::SceneOutputArchive archive(fout);
			//archive.setSerializePrefab(true);
			//archive << modelPrefab;
		}
		else if (ext == ".mat")
		{
			auto importer = GetAssetImporter<NativeFormatImporter>(path);
			importer->Load(path);
			ret = importer;
		}

		if (ret != nullptr && ret->m_assetTimeStamp == 0)	// if the .meta file is newly created
		{
			uint32_t time_created = static_cast<uint32_t>(time(NULL));
			ret->m_assetTimeStamp = time_created;
			auto meta_path = path.string() + ".meta";
			std::ofstream fout(meta_path);
			AssetOutputArchive archive(fout);
			archive.SerializeAssetImporter(ret);
		}

		return ret;
	}
}

