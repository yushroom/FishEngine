#include "AssetImporter.hpp"

//#include <iostream>

#include <GameObject.hpp>
#include <Timer.hpp>

#include "TextureImporter.hpp"
#include "FBXImporter.hpp"

#include <Serialization.hpp>
//#include <Serialization/archives/yaml.hpp>
//#include <Serialization/archives/YAMLInputArchive.hpp>
#include "AssetArchive.hpp"

#include <boost/uuid/uuid_generators.hpp>


//namespace FishEngine
//{
//	inline YAMLInputArchive& operator >> (YAMLInputArchive& archive, FishEditor::TextureImporterPtr importer)
//	{
//		Load(archive, *importer);
//		return archive;
//	}
//}

using namespace FishEngine;

namespace FishEditor
{
	AssetImporter::AssetImporter()
		: m_guid(boost::uuids::random_generator()())
	{

	}

	//void AssetImporter::SaveAndReimport()
	//{
	//	AssetDatabase::ImportAsset(m_assetPath);
	//}

	template<class AssetImporterType>
	std::shared_ptr<AssetImporterType> GetAssetImporter(Path const & assetPath)
	{
		//bool need_generate = true;
		auto meta_path = assetPath.string() + ".meta";
		auto name = assetPath.stem().string();

#if 0
		if (boost::filesystem::exists(meta_path))
		{
			uint32_t asset_modified_time = static_cast<uint32_t>(boost::filesystem::last_write_time(assetPath));
			YAMLInputArchive archive{ meta_path };
			uint32_t meta_created_time;
			archive >> make_nvp("timeCreated", meta_created_time);
			if (asset_modified_time <= meta_created_time)
			{
				Debug::Log("Load .meta file: %s", meta_path.c_str());
				archive.ToNextNode();
				auto importer = archive.DeserializeObject<AssetImporterType>();
				importer->setName(name);
				return importer;
			}
		}
#endif
		Debug::Log("Generate .meta file: %s", meta_path.c_str());
		auto importer = std::make_shared<AssetImporterType>();
		importer->setName(name);
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
			//Timer t(path.string());
			auto importer = GetAssetImporter<TextureImporter>(path);
			//auto tex = importer->FromFile(path);
			//t.StopAndPrint();
			s_pathToImpoter[path] = importer;
			//Timer t(path.string());
			auto texture = importer->Import(path);
			texture->setName(path.stem().string());
			s_objectInstanceIDToPath[texture->GetInstanceID()] = path;
			s_importerGUIDToTexture[importer->GetGUID()] = texture;
			//t.StopAndPrint();
			//return importer;
			ret = importer;
		}
		else if (ext == ".fbx" || ext == ".FBX")
		{
			//auto importer = std::make_shared<FBXImporter>();
			auto importer = GetAssetImporter<FBXImporter>(path);
			s_pathToImpoter[path] = importer;
			auto model = importer->Load(path);
			model->setName(path.stem().string());
			s_objectInstanceIDToPath[model->GetInstanceID()] = path;
			s_importerGUIDToModel[importer->GetGUID()] = model;
			//return importer;
			ret = importer;
		}

		if (ret != nullptr)
		{
			auto meta_path = path.string() + ".meta";
			std::ofstream fout(meta_path);
			uint32_t time_created = static_cast<uint32_t>(time(NULL));
			AssetOutputArchive archive(fout);
			//YAMLOutputArchive archive(fout);
			//archive.SetManipulator(YAML::BeginMap);
			archive.BeginMap();
			archive << make_nvp("timeCreated", time_created);
			archive << make_nvp("guid", ret->GetGUID());
			//archive.SetManipulator(YAML::EndMap);
			archive.EndMap();
			archive << ret;
		}

		return ret;
	}


	std::map<boost::uuids::uuid, FishEngine::GameObjectPtr> AssetImporter::s_importerGUIDToModel;

	std::map<int, boost::filesystem::path> AssetImporter::s_objectInstanceIDToPath;

	std::map<boost::uuids::uuid, TexturePtr> AssetImporter::s_importerGUIDToTexture;

	std::map<boost::filesystem::path, std::shared_ptr<AssetImporter>> AssetImporter::s_pathToImpoter;

}

