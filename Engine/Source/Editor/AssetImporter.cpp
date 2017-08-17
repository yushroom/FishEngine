#include "AssetImporter.hpp"

//#include <iostream>

#include <FishEngine/GameObject.hpp>
#include <FishEngine/Timer.hpp>
#include <FishEngine/Shader.hpp>
#include <FishEngine/AudioClip.hpp>

#include "TextureImporter.hpp"
#include "FBXImporter.hpp"
#include "NativeFormatImporter.hpp"
#include "ShaderImporter.hpp"
#include "DDSImporter.hpp"
#include "AudioImporter.hpp"

#include "AssetArchive.hpp"
#include "SceneArchive.hpp"

#include <boost/uuid/uuid_generators.hpp>


using namespace FishEngine;

namespace FishEditor
{
	std::map<FishEngine::GUID, AssetPtr> AssetImporter::s_importerGUIDToObject;
	std::map<int, FishEngine::Path> AssetImporter::s_objectInstanceIDToPath;
	std::map<FishEngine::Path, std::shared_ptr<AssetImporter>> AssetImporter::s_pathToImpoter;

	AssetImporter::AssetImporter()
		: m_guid(boost::uuids::random_generator()())
	{
		m_asset = std::make_shared<Asset>();
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
		LogInfo("Generate .meta file: " + meta_path);
		auto importer = MakeShared<AssetImporterType>();
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
			if (ext == ".dds")
			{
				auto importer = GetAssetImporter<DDSImporter>(path);
				//s_pathToImpoter[path] = importer;
				//Timer t(path.string());
				auto texture = importer->Load();
				texture->setName(path.stem().string());
				s_objectInstanceIDToPath[texture->GetInstanceID()] = path;
				importer->asset()->Add(texture);
				ret = importer;
				//t.StopAndPrint();
			}
			else
			{
				auto importer = GetAssetImporter<TextureImporter>(path);
				//s_pathToImpoter[path] = importer;
				//Timer t(path.string());
				auto texture = importer->Import(path);
				texture->setName(path.stem().string());
				//importer->asset()->Add(texture);
				s_objectInstanceIDToPath[texture->GetInstanceID()] = path;
				//t.StopAndPrint();
				ret = importer;
			}
		}
		else if (type == AssetType::Shader)
		{
			//auto shader = Shader::CreateFromFile(path);
			auto importer = GetAssetImporter<ShaderImporter>(path);
			//s_pathToImpoter[path] = importer;
			auto shader = importer->Load();
			shader->setName(path.stem().string());
			importer->asset()->Add(shader);
			s_objectInstanceIDToPath[shader->GetInstanceID()] = path;
			ret = importer;
		}
		else if (type == AssetType::Model)
		{
			Timer t(path.string());
			auto importer = GetAssetImporter<FBXImporter>(path);
			//s_pathToImpoter[path] = importer;
			auto modelPrefab = importer->Load(path);
			modelPrefab->setName(path.stem().string());
			modelPrefab->rootGameObject()->setName(path.stem().string());
			s_objectInstanceIDToPath[modelPrefab->GetInstanceID()] = path;
			s_objectInstanceIDToPath[modelPrefab->rootGameObject()->GetInstanceID()] = path;
			ret = importer;
			t.StopAndPrint();

			//std::ofstream fout(path.string() + ".prefab");
			//FishEditor::SceneOutputArchive archive(fout);
			//archive.setSerializePrefab(true);
			//archive << modelPrefab;
		}
		else if (type == AssetType::AudioClip)
		{
			auto importer = GetAssetImporter<AudioImporter>(path);
			auto clip = importer->Import(path);
			clip->setName(path.stem().string());
			importer->asset()->Add(clip);
			s_objectInstanceIDToPath[clip->GetInstanceID()] = path;
			ret = importer;
		}
		else if (type == AssetType::Material)
		{
			auto importer = GetAssetImporter<NativeFormatImporter>(path);
			auto material = importer->Load(path);
			material->setName(path.stem().string());
			importer->m_asset->Add(material);
			//s_pathToImpoter[path] = importer;
			s_objectInstanceIDToPath[material->GetInstanceID()] = path;
			ret = importer;
		}

		// TODO: move to ...
		if (ret != nullptr)
		{
			s_importerGUIDToObject[ret->GetGUID()] = ret->asset();
			s_pathToImpoter[path] = ret;
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

