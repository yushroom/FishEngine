#ifndef AssetImporter_hpp
#define AssetImporter_hpp

#include "FishEngine.hpp"
#include "Object.hpp"
#include "Resources.hpp"
#include "ReflectClass.hpp"

namespace FishEngine
{
    class AssetImporter : public Object
    {
    public:
		InjectClassName(AssetImporter);

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
