#include <TextureImporter.hpp>
#include <iostream>

using namespace std;
using namespace FishEngine;

void GenerateMetaFiles(FishEngine::Path const & path)
{
	assert(boost::filesystem::exists(path));
	auto ext = path.extension();
	if (ext == ".png")
	{
		cout << path.stem() << " is png";
	}
}

int main()
{
	Resources::SetAssetsDirectory(R"(D:\program\FishEngine\Engine\Source\Test\AssetSerialization\Test\Assets)");
	TextureImporter importer;

	return 0;
}
