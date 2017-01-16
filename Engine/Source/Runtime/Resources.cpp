#include "Resources.hpp"
#include "Debug.hpp"
#include <iostream>
#include "TextureImporter.hpp"
#include "ModelImporter.hpp"
#include "Timer.hpp"

#include "Serialization/archives/yaml.hpp"
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
		else if (ext == ".hpp")
		{
			return AssetType::Script;
		}
		return AssetType::Unknown;
	}
	
	void GenerateMetaFile(Path const & path, std::shared_ptr<AssetImporter> importer)
	{
		auto meta_path = path.string() + ".meta";
		std::ofstream fout(meta_path);
		YAMLOutputArchive archive(fout);
		archive << importer;
	}
	
    void FileNode::BuildNodeTree(const Path path)
    {
		this->path = path;
        if (boost::filesystem::is_directory(path))
        {
            for (auto& it : boost::filesystem::directory_iterator(path))
            {
                //std::cout << "\t" << it.path() << '\n';
                children.emplace_back();
                children.back().parent = this;
                children.back().BuildNodeTree(it.path());
            }
        }
		else
		{
			auto ext = path.extension();
			auto type = GetAssetType(ext);
			if (type == AssetType::Texture)
			{
				auto importer = std::make_shared<TextureImporter>();
				importer->FromFile(path);
				GenerateMetaFile(path, importer);
			}
			else if (type == AssetType::Model)
			{
				auto importer = std::make_shared<ModelImporter>();
				importer->LoadFromFile(path);
				GenerateMetaFile(path, importer);
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

    void Resources::Init()
    {
        auto cwd = boost::filesystem::current_path();
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

    Path Resources::s_assetsDirectory;
    Path Resources::s_rootSystemDirectory;

}
