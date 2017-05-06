#include <Serialization/Archive.hpp>
#include <Serialization/YAMLArchive.hpp>

#include "AssetImporter.hpp"

namespace FishEditor
{
	class MetaInputArchive : public FishEngine::YAMLInputArchive
	{
	public:
		MetaInputArchive(std::istream & is) : YAMLInputArchive(is) { }

		virtual ~MetaInputArchive() = default;

		uint32_t timeStamp() const;

		AssetImporterPtr DeserializeAssetImporter();
	};

	class MaterialArchive : public FishEngine::YAMLInputArchive
	{
	public:
		MaterialArchive(std::istream & is) : YAMLInputArchive(is) { }

		virtual ~MaterialArchive() = default;

		FishEngine::MaterialPtr DeserializeMaterial();
	};

	class AssetInputArchive : public FishEngine::YAMLInputArchive
	{
	public:
		AssetInputArchive(std::istream & is) : YAMLInputArchive(is) { }

		virtual ~AssetInputArchive() = default;

		std::vector<FishEngine::ObjectPtr> LoadAll();

	protected:
		bool m_isSingle = true;
	};


	class AssetOutputArchive : public FishEngine::YAMLOutputArchive
	{
	public:
		AssetOutputArchive(std::ostream & os) : YAMLOutputArchive(os)
		{

		}

		virtual ~AssetOutputArchive() = default;

		void SerializeAssetImporter (AssetImporterPtr const & importer)
		{
			SerializeAssetImporter(*importer);
		}
		
		void SerializeAssetImporter (AssetImporter const & importer)
		{
			uint32_t time_created = static_cast<uint32_t>(time(NULL));
			BeginMap(3);
			//SerializeNVP(FishEngine::make_nvp("timeCreated", time_created));
			//SerializeNVP(FishEngine::make_nvp("guid", importer->GetGUID()));
			(*this) << FishEngine::make_nvp("timeCreated", time_created);
			(*this) << FishEngine::make_nvp("guid", importer.GetGUID());
			m_emitter << importer.ClassName();
			BeginMap(1);	// do not know map size
			importer.Serialize(*this);
			EndMap();
			EndMap();
		}

	protected:

		virtual void SerializeObject(FishEngine::ObjectPtr const & obj) override
		{
			// use SerializeAssetImporter
			abort();
		}
	};
}
