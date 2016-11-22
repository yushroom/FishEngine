#ifndef AssetImporter_hpp
#define AssetImporter_hpp

#include "FishEngine.hpp"
#include "Object.hpp"

#define BOOST_FILESYSTEM_NO_DEPRECATED
#include <boost/filesystem.hpp>

using Path = boost::filesystem::path;

namespace FishEngine
{
    class FE_EXPORT AssetImporter : public Object
    {
    public:
        AssetImporter() = default;
        virtual ~AssetImporter() {};
        
        //static PAssetImporter GetAtPath(const std::string& path);
        
        static Path assetsRootDirectory()
        {
            return s_assetsRootDirectory;
        }
        
    private:
        std::string m_assetBundleName;
        std::string m_assetBundelVariant;
        Path m_assetPath;
        static Path s_assetsRootDirectory;
    };
}

#endif /* AssetImporter_hpp */
