#ifndef AssetImporter_hpp
#define AssetImporter_hpp

#include "FishEngine.hpp"
#include "Object.hpp"

namespace FishEngine {
    class AssetImporter : public Object
    {
    public:
        AssetImporter() = default;
        virtual ~AssetImporter() {};
        
        typedef std::shared_ptr<AssetImporter> PAssetImporter;
        
        static PAssetImporter GetAtPath(const std::string& path);
        
    private:
        std::string m_assetBundleName;
        std::string m_assetBundelVariant;
        std::string m_assetPath;
    };
}

#endif /* AssetImporter_hpp */
