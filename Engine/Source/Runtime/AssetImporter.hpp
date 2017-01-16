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
        
//        static Path assetsRootDirectory()
//        {
//            return s_assetsRootDirectory;
//        }
		
		virtual void SaveAndReimport() = 0;
		
		// Set the AssetBundle name and variant.
		void SetAssetBundleNameAndVariant(std::string const & assetBundleName, std::string const & assetBundleVariant);
		
		// Retrieves the asset importer for the asset at path.
		static std::shared_ptr<AssetImporter> GetAtPath(Path const & path);
		
    private:
		Meta(NonSerializable)
        std::string m_assetBundleName;
		
		Meta(NonSerializable)
        std::string m_assetBundelVariant;
		
		Meta(NonSerializable)
        Path		m_assetPath;
		
		Meta(NonSerializable)
		std::string m_userData;
		
		
		//static Path s_assetsRootDirectory;
    };
}

#endif /* AssetImporter_hpp */
