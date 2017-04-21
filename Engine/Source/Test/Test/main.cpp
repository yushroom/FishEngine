#include <algorithm>
#include <iostream>
#include <sstream>

#include <Camera.hpp>
#include <GameApp.hpp>
#include <Scene.hpp>
#include <Material.hpp>
#include <GameObject.hpp>
#include <Light.hpp>
#include <RenderSettings.hpp>

using namespace std;
using namespace FishEngine;

class TestApp : public FishEngine::GameApp
{
public:
	virtual void Init() override
	{
		auto camera_go = Scene::CreateCamera();
		camera_go->setName("Main Camera");
		auto camera = camera_go->GetComponent<Camera>();
		
		camera_go->transform()->setLocalPosition(0, 1, -10);
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
	
	virtual void Update() override
	{
		
	}
};

int main()
{
	TestApp app;
	//app.Init();
	app.Run();
	return 0;
}
