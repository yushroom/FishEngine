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
    //Debug::setColorMode(false);
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
		YAML::Emitter emitter;
		//emitter << YAML::LocalTag("u") << "tag:FishEngine:";
		//emitter.SetStringFormat(YAML::Auto);
		//emitter << "%YAML 1.1";
		//emitter << "%TAG !u! tag:FishEngine:";
		YAMLOutputArchive archive(emitter);
		emitter << YAML::BeginDoc;
		emitter << YAML::LocalTag("u", "29");
		emitter << YAML::Newline;
		archive << Vector3(1, 2, 3);
		emitter << YAML::EndDoc;
		emitter << YAML::BeginDoc;
		//emitter << YAML::LocalTag("u", "28");
		archive << Transform();
		emitter << YAML::EndDoc;
		//emitter << YAML::BeginDoc; 
		auto cube = GameObject::CreatePrimitive(PrimitiveType::Cube);
		cube->transform()->SetParent(Camera::main()->transform());
		auto go = Camera::main()->gameObject();
		archive << go;
		auto importer = std::make_shared<TextureImporter>();
		archive << importer;
		if (!emitter.good())
		{
			Debug::LogWarning("%s", emitter.GetLastError().c_str());
		}
		std::cout << emitter.c_str() << std::endl;
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
