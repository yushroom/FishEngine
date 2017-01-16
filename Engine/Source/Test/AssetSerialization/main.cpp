#if 1

#include <FishEngine.hpp>
#include <iostream>
#include <Serialization.hpp>
#include <Serialization/archives/yaml.hpp>

#include <GameApp.hpp>
#include <RenderSettings.hpp>
#include <TextureImporter.hpp>

using namespace FishEngine;
using namespace std;

void DefaultScene()
{
#if FISHENGINE_PLATFORM_APPLE
	Debug::setColorMode(false);
#endif
	cout << "CWD: " << boost::filesystem::current_path() << endl;
	
	auto camera = Camera::Create();
	auto camera_go = Scene::CreateGameObject("Main Camera");
	camera_go->AddComponent(camera);
	camera_go->AddComponent<CameraController>();
	camera_go->transform()->setLocalPosition(0, 0, 5);
	camera_go->transform()->setLocalPosition(0, 1, -10);
	//camera_go->transform()->LookAt(0, 0, 0);
	camera_go->setTag("MainCamera");
	//camera_go->AddComponent<TakeScreenShot>();
	
	auto light_go = Scene::CreateGameObject("Directional Light");
	light_go->transform()->setPosition(0, 3, 0);
	light_go->transform()->setLocalEulerAngles(50, -30, 0);
	light_go->AddComponent(Light::Create());
	
	auto material = Material::InstantiateBuiltinMaterial("SkyboxProcedural");
	material->SetFloat("_AtmosphereThickness", 1.0);
	//material->SetFloat("_SunDisk", 2);
	material->SetFloat("_SunSize", 0.04f);
	material->SetVector4("_SkyTint", Vector4(0.5f, 0.5f, 0.5f, 1));
	material->SetVector4("_GroundColor", Vector4(.369f, .349f, .341f, 1));
	material->SetFloat("_Exposure", 1.3f);
	RenderSettings::setSkybox(material);
}

class AssetSerializationApp : public GameApp
{
public:
	virtual void Init() override
	{
		DefaultScene();
	}
	
	virtual void Update() override
	{
	}
};
		
int main()
{
	AssetSerializationApp app;
	app.Run();
	return 0;
}
		
#else

#include <FishEngine.hpp>
#include <iostream>
#include <Serialization.hpp>
#include <Serialization/archives/yaml.hpp>

#include <GameApp.hpp>
#include <RenderSettings.hpp>
#include <TextureImporter.hpp>

using namespace FishEngine;
using namespace std;

void DefaultScene()
{
#if FISHENGINE_PLATFORM_APPLE
    Debug::setColorMode(false);
#endif
	cout << "CWD: " << boost::filesystem::current_path() << endl;

	auto camera = Camera::Create();
	auto camera_go = Scene::CreateGameObject("Main Camera");
	camera_go->AddComponent(camera);
	camera_go->AddComponent<CameraController>();
	camera_go->transform()->setLocalPosition(0, 0, 5);
	camera_go->transform()->setLocalPosition(0, 1, -10);
	//camera_go->transform()->LookAt(0, 0, 0);
	camera_go->setTag("MainCamera");
	//camera_go->AddComponent<TakeScreenShot>();

	auto light_go = Scene::CreateGameObject("Directional Light");
	light_go->transform()->setPosition(0, 3, 0);
	light_go->transform()->setLocalEulerAngles(50, -30, 0);
	light_go->AddComponent(Light::Create());

	auto material = Material::InstantiateBuiltinMaterial("SkyboxProcedural");
	material->SetFloat("_AtmosphereThickness", 1.0);
	//material->SetFloat("_SunDisk", 2);
	material->SetFloat("_SunSize", 0.04f);
	material->SetVector4("_SkyTint", Vector4(0.5f, 0.5f, 0.5f, 1));
	material->SetVector4("_GroundColor", Vector4(.369f, .349f, .341f, 1));
	material->SetFloat("_Exposure", 1.3f);
	RenderSettings::setSkybox(material);
}

class AssetSerializationApp : public GameApp
{
public:
	virtual void Init() override
	{
		DefaultScene();
		std::ofstream fout("test.yaml");
		YAMLOutputArchive archive(fout);
		archive.SetManipulator(YAML::BeginDoc);
		archive.SetManipulator(YAML::BeginMap);
		archive << make_nvp("int", 3110103671);
		archive << make_nvp("bool", true);
		archive << make_nvp("bool", false);
		archive << make_nvp("float", 1.0f);
		archive << make_nvp("double", 2.0);
		archive << make_nvp("cstring", "Hello World!");
		archive << make_nvp("std::string", std::string("This is a std::string."));
		archive << make_nvp("empty list", std::vector<int>());
		archive << make_nvp("list", std::vector<int>{1, 2, 3, 4, 5});
		archive << make_nvp("empty map", std::map<int, int>());
		archive << make_nvp("Vector3", Vector3(1, 2, 3));
		archive.SetManipulator(YAML::EndMap);
		//archive << std::vector<std::string>{"eggs", "bread", "mild"};
		archive.SetManipulator(YAML::EndDoc);
		archive << std::make_shared<Transform>();
		auto cube = GameObject::CreatePrimitive(PrimitiveType::Cube);
		cube->transform()->SetParent(Camera::main()->transform());
		auto go = Camera::main()->gameObject();
		archive << go;
		auto importer = std::make_shared<TextureImporter>();
		archive << importer;
	}

	virtual void Update() override
	{
	}
};

int main()
{
	AssetSerializationApp app;
	app.Run();
	return 0;
}

#endif
