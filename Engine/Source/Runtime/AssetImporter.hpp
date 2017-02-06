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
		virtual ~AssetImporter() = default;

		// noncopyable
		AssetImporter(AssetImporter const &) = delete;
		AssetImporter& operator=(AssetImporter const &) = delete;
		
		void SaveAndReimport();
		
		// Set the AssetBundle name and variant.
		void SetAssetBundleNameAndVariant(std::string const & assetBundleName, std::string const & assetBundleVariant);
		
		// Retrieves the asset importer for the asset at path.
		static std::shared_ptr<AssetImporter> GetAtPath(Path const & path);
		
	protected:

		// dirty flag for SaveAndReimport()
		Meta(NonSerializable)
		bool			m_isDirty = false;

		//Get or set the AssetBundle name.
		Meta(NonSerializable)
		std::string		m_assetBundleName;
		
		// Get or set the AssetBundle variant.
		Meta(NonSerializable)
		std::string		m_assetBundelVariant;
		
		// The path name of the asset for this importer. (Read Only)
		Meta(NonSerializable)
		Path			m_assetPath;
		
		// Get or set any user data.
		Meta(NonSerializable)
		std::string		m_userData;

		static std::map<boost::filesystem::path, std::shared_ptr<AssetImporter>> s_pathToImpoter;
	};
}

#endif /* AssetImporter_hpp */
