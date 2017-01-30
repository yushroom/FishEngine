#include "Resources.hpp"
#include <iostream>
#include <ctime>
#include "Debug.hpp"
#include "TextureImporter.hpp"
#include "ModelImporter.hpp"
#include "Timer.hpp"

#include "Serialization/archives/yaml.hpp"
#include "Serialization/archives/YAMLInputArchive.hpp"
#include "Serialization.hpp"

namespace FishEngine
{
    FishEngine::FileNode Resources::s_assetsDirectoryRootNode;

	enum class AssetType
	{
		Unknown,
		Texture,
		Model,
		Shader,
		Material,
		Script,
	};
	
	AssetType GetAssetType(Path const & ext)
	{
		//auto ext = path.extension();
		if (ext == ".jpg" || ext == ".png")
		{
			return AssetType::Texture;
		}
		else if (ext == ".obj" || ext == ".fbx")
		{
			return AssetType::Model;
		}
		else if (ext == ".shader")
		{
			return AssetType::Shader;
		}
		else if (ext == ".mat")
		{
			return AssetType::Material;
		}
		else if (ext == ".hpp" || ext == ".cpp")
		{
			return AssetType::Script;
		}
		return AssetType::Unknown;
	}
	
	template<class AssetImporterType>
	std::shared_ptr<AssetImporterType> GetAssetImporter(Path const & assetPath)
	{
		//bool need_generate = true;
		auto meta_path = assetPath.string() + ".meta";
#if 0
		if (boost::filesystem::exists(meta_path))
		{
			uint32_t asset_modified_time = static_cast<uint32_t>(boost::filesystem::last_write_time(assetPath));
			YAMLInputArchive archive{meta_path};
			uint32_t meta_created_time;
			archive >> make_nvp("timeCreated", meta_created_time);
			if (asset_modified_time <= meta_created_time)
			{
				Debug::Log("Load .meta file: %s", meta_path.c_str());
				archive.ToNextNode();
				return archive.DeserializeObject<AssetImporterType>();
			}
		}
#endif
		
		Debug::Log("Generate .meta file: %s", meta_path.c_str());
		auto importer = std::make_shared<AssetImporterType>();
		std::ofstream fout(meta_path);
		uint32_t time_created = static_cast<uint32_t>(time(NULL));
		YAMLOutputArchive archive(fout);
		archive.SetManipulator(YAML::BeginMap);
		archive << make_nvp("timeCreated", time_created);
		archive.SetManipulator(YAML::EndMap);
		archive << importer;
		return importer;
	}
	
    void FileNode::BuildNodeTree(const Path path)
    {
		this->path = path;
        if (boost::filesystem::is_directory(path))
        {
            for (auto& it : boost::filesystem::directory_iterator(path))
            {
                //std::cout << "\t" << it.path() << '\n';
				auto & p = it.path();
				//std::cout << p << std::endl;
				if (p.extension() != ".meta")
				{
					//std::cout << "[" << p.extension().string() << "]" << std::endl;
					children.emplace_back();
					children.back().parent = this;
					children.back().BuildNodeTree(it.path());
				}
            }
        }
		else
		{
			auto ext = path.extension();
			auto type = GetAssetType(ext);
			if (type == AssetType::Texture)
			{
				Timer t(path.string());
				auto importer = GetAssetImporter<TextureImporter>(path);
				auto tex = importer->FromFile(path);
				t.StopAndPrint();
				Resources::s_uuidToImporter[tex->GetGUID()] = importer;
				Resources::s_pathToAsset[path] = tex;
			}
			else if (type == AssetType::Model)
			{
				Timer t(path.string());
				auto importer = GetAssetImporter<ModelImporter>(path);
				auto model = importer->LoadFromFile(path);
				t.StopAndPrint();
				//Resources::s_uuidToImporter[importer->GetGUID()] = importer;
				//Resources::s_pathToAsset[path] = model;
			}
		}
    }

    void Resources::SetAssetsDirectory(const Path& path)
    {
        s_assetsDirectory = path;
		Debug::Log("%s", "Loading assets...");
		Timer t("Loadd all assets");
		t.Start();
        s_assetsDirectoryRootNode.BuildNodeTree(path);
		t.StopAndPrint();
    }

    void Resources::Init(Path const & path)
    {
		Path cwd = path;
		if (path.empty())
			cwd = boost::filesystem::current_path();
		std::cout << cwd << std::endl;
        bool found;
        do
        {
            //std::cout << cwd << std::endl;
            found = true;
            for (auto& d : { "Assets/Models", "Assets/Textures", "Assets/Fonts", "Engine/Shaders" })
            {
                auto test = cwd / d;
                if (!boost::filesystem::exists(test) || !boost::filesystem::is_directory(test))
                {
                    found = false;
                    break;
                }
            }
            if (found)
                break;
            cwd = cwd.parent_path();
        } while (cwd.has_parent_path());

        if (!found)
        {
            Debug::LogError("Can't set root dir.");
            abort();
        }

        s_rootSystemDirectory = cwd;
    }

	std::map<boost::uuids::uuid, std::shared_ptr<AssetImporter>> Resources::s_uuidToImporter;
	std::map<boost::filesystem::path, std::weak_ptr<Object>> Resources::s_pathToAsset;
	Path Resources::s_assetsDirectory;
    Path Resources::s_rootSystemDirectory;

}
