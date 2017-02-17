#ifndef Resources_hpp
#define Resources_hpp

#include "FishEngine.hpp"

#define BOOST_FILESYSTEM_NO_DEPRECATED
#include <boost/filesystem.hpp>
#include <boost/uuid/uuid.hpp>

#include "ReflectClass.hpp"

namespace FishEngine
{
    typedef boost::filesystem::path Path;

	enum class AssetType
	{
		Unknown,
		Texture,
		Model,
		Shader,
		Material,
		Script,
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
