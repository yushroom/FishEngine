#ifndef AssetImporter_hpp
#define AssetImporter_hpp

#include "FishEditor.hpp"
#include <Object.hpp>
#include <Resources.hpp>
#include <ReflectClass.hpp>
#include <boost/uuid/uuid.hpp>

namespace FishEditor
{
	using GUID = boost::uuids::uuid;

	class AssetImporter : public FishEngine::Object
	{
	public:
		InjectClassName(AssetImporter);

		AssetImporter();
		virtual ~AssetImporter() = default;

		// noncopyable
		AssetImporter(AssetImporter const &) = delete;
		AssetImporter& operator=(AssetImporter const &) = delete;
		
		void SaveAndReimport();
		
		// Set the AssetBundle name and variant.
		void SetAssetBundleNameAndVariant(std::string const & assetBundleName, std::string const & assetBundleVariant);
		
		// Retrieves the asset importer for the asset at path.
		static AssetImporterPtr GetAtPath(FishEngine::Path path);

		GUID GetGUID() const
		{
			return m_guid;
		}
		
	protected:

		friend class FishEditor::AssetDatabase;
		friend class FishEditor::SceneOutputArchive;

		// dirty flag for SaveAndReimport()
		Meta(NonSerializable)
		bool							m_isDirty = false;

		//Get or set the AssetBundle name.
		Meta(NonSerializable)
		std::string						m_assetBundleName;
		
		// Get or set the AssetBundle variant.
		Meta(NonSerializable)
		std::string						m_assetBundelVariant;
		
		// The path name of the asset for this importer. (Read Only)
		Meta(NonSerializable)
		FishEngine::Path				m_assetPath;
		
		// Get or set any user data.
		Meta(NonSerializable)
		std::string						m_userData;

		Meta(NonSerializable)
		GUID m_guid;

		std::map<int, std::string>		m_fileIDToRecycleName;

		Meta(NonSerializable)
		std::map<std::string, int>		m_recycleNameToFileID;

		Meta(NonSerializable)
		int								m_nextNodeFileID = 100000;

	public:
		static std::map<boost::uuids::uuid, FishEngine::TexturePtr> s_importerGUIDToTexture;
		static std::map<boost::uuids::uuid, FishEngine::GameObjectPtr> s_importerGUIDToModel;
		static std::map<boost::filesystem::path, AssetImporterPtr> s_pathToImpoter;
		static std::map<int, boost::filesystem::path> s_objectInstanceIDToPath;
	};
}

#endif /* AssetImporter_hpp */
