#include "Resources.hpp"

namespace FishEngine
{
	AssetType Resources::GetAssetType(Path const & ext)
	{
		//auto ext = path.extension();
		if (ext == ".jpg" || ext == ".png" || ext == ".jpeg" || ext == ".tga" || ext == ".dds" || ext == ".psd")
		{
			return AssetType::Texture;
		}
		else if (ext == ".obj" || ext == ".fbx")
		{
			return AssetType::Model;
		}
		else if (ext == ".shader" || ext == ".surf")
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
		else if (ext == ".prefab")
		{
			return AssetType::Prefab;
		}
		else if (ext == ".mp3")
		{
			return AssetType::AudioClip;
		}
		return AssetType::Unknown;
	}
}
