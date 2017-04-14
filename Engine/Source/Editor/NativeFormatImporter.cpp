#include "NativeFormatImporter.hpp"
#include "AssetArchive.hpp"
#include "Material.hpp"


FishEngine::ObjectPtr FishEditor::NativeFormatImporter::Load(FishEngine::Path const & path)
{
	std::ifstream fin(path.string());
	MaterialArchive archive(fin);
	auto material = archive.DeserializeMaterial();
	m_asset->Add( material );
	return material;
}
