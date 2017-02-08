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
	AssetType Resources::GetAssetType(Path const & ext)
	{
		//auto ext = path.extension();
		if (ext == ".jpg" || ext == ".png" || ext == ".jpeg" || ext == ".tga")
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

    void Resources::SetAssetsDirectory(const Path& path)
    {
        s_assetsDirectory = path;
		//Debug::Log("%s", "Loading assets...");
		//Timer t("Loadd all assets");
		//t.Start();
        //s_assetsDirectoryRootNode.BuildNodeTree(path);
		//t.StopAndPrint();
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

	std::map<boost::uuids::uuid, std::shared_ptr<Object>> Resources::s_importerGUIDToAsset;

	std::map<boost::filesystem::path, std::weak_ptr<AssetImporter>> Resources::s_pathToImporter;

	//std::map<boost::uuids::uuid, std::shared_ptr<AssetImporter>> Resources::s_uuidToImporter;
	//std::map<boost::filesystem::path, std::weak_ptr<Object>> Resources::s_pathToAsset;
	Path Resources::s_assetsDirectory;
    Path Resources::s_rootSystemDirectory;

}
