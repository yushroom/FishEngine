#ifndef AssetImporter_hpp
#define AssetImporter_hpp

#include "FishEditor.hpp"
#include <Object.hpp>
#include <Resources.hpp>
#include <ReflectClass.hpp>
#include <GUID.hpp>

namespace FishEditor
{
	class AssetImporter : public FishEngine::Object
	{
	public:
		InjectClassName(AssetImporter);

		AssetImporter();
		virtual ~AssetImporter() = default;

		// noncopyable
		AssetImporter(AssetImporter const &) = delete;
		AssetImporter& operator=(AssetImporter const &) = delete;
		
		FishEngine::Path assetPath() const
		{
			return m_assetPath;
		}
		
		uint32_t assetTimeStamp() const
		{
			return m_assetTimeStamp;
		}
		
		
		void SaveAndReimport();
		
		// Set the AssetBundle name and variant.
		void SetAssetBundleNameAndVariant(std::string const & assetBundleName, std::string const & assetBundleVariant);
		
		// Retrieves the asset importer for the asset at path.
		static AssetImporterPtr GetAtPath(FishEngine::Path path);

		FishEngine::GUID GetGUID() const
		{
			return m_guid;
		}
		
	protected:

		friend class FishEditor::AssetDatabase;
		friend class FishEditor::SceneOutputArchive;
		
		virtual void Reimport() { abort(); }
		
		bool IsNewlyCreated() const
		{
			return m_assetTimeStamp == 0;
		}
		
		virtual void BuildFileIDToRecycleName() { };
		
		template<class AssetImporterType>
		static std::shared_ptr<AssetImporterType> GetAssetImporter(FishEngine::Path const & assetPath);

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
		
		Meta(NonSerializable)
		uint32_t						m_assetTimeStamp = 0;
		
		// Get or set any user data.
		Meta(NonSerializable)
		std::string						m_userData;

		Meta(NonSerializable)
		FishEngine::GUID				m_guid;

		// fileID starts with ClassID
		std::map<int, std::string>		m_fileIDToRecycleName;
		
		Meta(NonSerializable)
		std::map<int, int>				m_classIDCount;	// count for each ClassID

		Meta(NonSerializable)
		std::map<std::string, int>		m_recycleNameToFileID;

		//Meta(NonSerializable)
		//int								m_nextNodeFileID = 100000;
		

	public:
		//static std::map<FishEngine::GUID, FishEngine::TexturePtr> s_importerGUIDToTexture;
		//static std::map<FishEngine::GUID, FishEngine::GameObjectPtr> s_importerGUIDToGameObject;
		static std::map<FishEngine::GUID, FishEngine::ObjectPtr> s_importerGUIDToObject;
		static std::map<boost::filesystem::path, AssetImporterPtr> s_pathToImpoter;
		static std::map<int, boost::filesystem::path> s_objectInstanceIDToPath;
	};
}

#endif /* AssetImporter_hpp */
