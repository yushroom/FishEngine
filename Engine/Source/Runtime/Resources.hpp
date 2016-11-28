#ifndef Resources_hpp
#define Resources_hpp

#include "FishEngine.hpp"
#include <boost/filesystem.hpp>

namespace FishEngine
{
    typedef boost::filesystem::path Path;
    //// simple implmentation, remove it after c++17
    //class FE_EXPORT Path
    //{
    //public:
    //    Path(std::string path);

    //    Path parent_path() const
    //    {
    //        return boost::filesystem::path(m_pathStr).parent_path().string();
    //    }

    //    Path stem() const
    //    {
    //        return boost::filesystem::path(m_pathStr).stem().string();
    //    }

    //    Path extension() const
    //    {
    //        return boost::filesystem::path(m_pathStr).extension().string();
    //    }

    //private:
    //    std::string m_pathStr;
    //};

    enum class SystemDirectoryType
    {
        RootDir,
        ShaderDir,
        TextureDir,
        FontDir,
    };

    class FE_EXPORT Resources
    {
    public:
        Resources() = delete;

        //static Object Load(const std::string path);

        static Path shaderRootDirectory()
        {
            return s_rootSystemDirectory / "Engine/Shaders";
        }

        static Path shaderHeaderDirectory()
        {
            return s_rootSystemDirectory / "Engine/Shaders/include";
        }

        static Path textureRootDirectory()
        {
            return s_rootSystemDirectory / "Assets/Texture";
        }

        static Path modelRootDirectory()
        {
            return s_rootSystemDirectory / "Assets/Models";
        }

        static const Path& fontRootDirectory()
        {
            return s_rootSystemDirectory / "Assets/Fonts";
        }

        static void SetAssetsDirectory(const Path& path)
        {
            s_assetsDirectory = path;
        }

        static bool FindAssetFile(const std::string& filename, Path& out_path)
        {
        }

        static bool FindSystemFile(const std::string& filename, Path& out_path)
        {

        }

        static void Init();

    private:
        static Path s_assetsDirectory;
        static Path s_rootSystemDirectory;
        const static Path s_textureRootDirectory;
    };
}

#endif // Resources_hpp
