#ifndef Resources_hpp
#define Resources_hpp

#include "FishEngine.hpp"

#include "Path.hpp"
//#include <boost/uuid/uuid.hpp>

#include "ReflectClass.hpp"

namespace FishEngine
{
	enum class AssetType
	{
		Unknown,
		Texture,
		Model,
		Mesh,
		Shader,
		Material,
		Script,
		Prefab,
		AudioClip,
	};

	enum class SystemDirectoryType
	{
		RootDir,
		ShaderDir,
		TextureDir,
		FontDir,
	};

	class FE_EXPORT Meta(NonSerializable) Resources
	{
	public:
		Resources() = delete;

		//static Object Load(const std::string path);

		static AssetType GetAssetType(Path const & ext);

	};
}

#endif // Resources_hpp
