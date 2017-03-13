#include "MainEditor.hpp"

#include <QDir>
#include <QCoreApplication>

#include <Debug.hpp>
#include <Camera.hpp>
#include <GameObject.hpp>
#include <CameraController.hpp>
#include <ModelImporter.hpp>
#include <Shader.hpp>
#include <Scene.hpp>
#include <Screen.hpp>
#include <Graphics.hpp>
#include <Light.hpp>
#include <RenderSettings.hpp>
#include <QualitySettings.hpp>
#include <MeshRenderer.hpp>
#include <Material.hpp>
#include <PhysicsSystem.hpp>
#include <Application.hpp>
#include <Prefab.hpp>

#include "SceneViewEditor.hpp"
#include "Selection.hpp"
#include "EditorGUI.hpp"
#include "SceneViewEditor.hpp"
//#include "Inspector.hpp"
#include "AssetDataBase.hpp"
#include "EditorResources.hpp"

#include <boost/filesystem.hpp>

#include <iostream>

using namespace FishEngine;
using namespace std;

namespace FishEditor
{

	GameObjectPtr FindNamedChild(const GameObjectPtr & root, const std::string& name)
	{
		auto& children = root->transform()->children();
		for (auto& c : children) {
			const auto& g = c.lock();
			//Debug::Log("Name: %s", g->name().c_str());
			if (g->name() == name) {
				return g->gameObject();
			}
			auto r = FindNamedChild(g->gameObject(), name);
			if (r != nullptr) {
				return r;
			}
		}
		return nullptr;
	}

void DefaultScene()
{
#if 1
	cout << "CWD: " << boost::filesystem::initial_path() << endl;

//    auto camera = Camera::Create();
//    auto camera_go = Scene::CreateGameObject("Main Camera");
//    camera_go->AddComponent(camera);
//    camera_go->AddComponent<CameraController>();
	auto camera_go = Scene::CreateCamera();
	camera_go->setName("Main Camera");
	auto camera = camera_go->GetComponent<Camera>();

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

#if 0

	//auto go = GameObject::CreatePrimitive(PrimitiveType::Cube);
	QualitySettings::setShadowDistance(20);
	{
#if FISHENGINE_PLATFORM_WINDOWS
	const std::string root_dir = R"(D:\program\FishEngine\Example\CascadedShadowMapping\)";
#else
	const std::string root_dir = "/Users/yushroom/program/graphics/FishEngine/Example/CascadedShadowMapping/";
#endif
	ModelImporter importer;
	auto model = importer.LoadFromFile(root_dir + "Terrain.obj");
	auto terrainGO = model->CreateGameObject();
	auto material = Material::InstantiateBuiltinMaterial("DebugCSM");
	//auto material = Material::defaultMaterial();
	//material->EnableKeyword(ShaderKeyword::Shadow);

	//TextureImporter texture_importer;
	//auto bakedAO = texture_importer.FromFile(root_dir + "bakedAO.jpg");
	//material->setMainTexture(bakedAO);
	terrainGO->GetComponent<MeshRenderer>()->SetMaterial(material);
	}
	
#elif 0
	
	QualitySettings::setShadowDistance(30);
	Path sponza_root = Resources::exampleRootDirectory() / "Sponza";
	Path sponza_assets_root = sponza_root / "crytek-sponza";
	Resources::SetAssetsDirectory(sponza_root);
	
	ModelImporter importer;
	importer.setFileScale(0.01f);
	//auto sponza_model = importer.LoadFromFile(sponza_assets_root / "sponza.obj");
	//auto sponza_go = sponza_model->CreateGameObject();

	//auto handle = std::async(std::launch::async,
	//	[&importer, &sponza_assets_root]() {
	//	return importer.LoadFromFile(sponza_assets_root / "sponza.obj");
	//}
	//);

	//TextureImporter tex_importer;
	Path textures_root = sponza_assets_root / "textures";
	auto shader1 = Shader::CreateFromFile(sponza_root / "diffuse_mask_twosided.shader");

	auto sponza_model = importer.LoadFromFile(sponza_assets_root / "sponza.obj");
	//auto sponza_model = handle.get();
	auto sponza_go = sponza_model->CreateGameObject();

	auto ApplyMateril1 = [&sponza_go, &shader1, &textures_root]
	(const char* go_name, const std::string& diffuse_tex, const std::string& mask_tex)
	{
		auto mesh0 = FindNamedChild(sponza_go, go_name);
		auto mtl = mesh0->GetComponent<MeshRenderer>()->material();
		mtl->SetShader(shader1);
		auto diffuse = AssetDatabase::LoadAssetAtPath<Texture>(textures_root / (diffuse_tex + ".png"));
		auto mask = AssetDatabase::LoadAssetAtPath<Texture>(textures_root / (mask_tex + ".png"));
		//auto diffuse = tex_importer.FromFile(textures_root / (diffuse_tex + ".png"));
		//auto mask = tex_importer.FromFile(textures_root / (mask_tex + ".png"));
		mtl->SetTexture("DiffuseTex", diffuse);
		mtl->SetTexture("MaskTex", mask);
	};

	ApplyMateril1("mesh0", "sponza_thorn_diff", "sponza_thorn_mask");
	ApplyMateril1("mesh1", "vase_plant", "vase_plant_mask");
	ApplyMateril1("mesh20", "chain_texture", "chain_texture_mask");

	auto shader2 = Shader::CreateFromFile(sponza_root / "diffuse_bump.shader");
	auto ApplyMateril2 = [&sponza_go, &shader2, &textures_root]
	(const char* go_name, const std::string& diffuse_tex)
	{
		auto mesh0 = FindNamedChild(sponza_go, go_name);
		auto mtl = mesh0->GetComponent<MeshRenderer>()->material();
		mtl->SetShader(shader2);
		auto diffuse = AssetDatabase::LoadAssetAtPath<Texture>(textures_root / (diffuse_tex + ".png"));
		//auto diffuse = tex_importer.FromFile(textures_root / (diffuse_tex + ".png"));
		mtl->SetTexture("DiffuseTex", diffuse);
	};

	ApplyMateril2("mesh2", "vase_round");
	ApplyMateril2("mesh3", "background");
	ApplyMateril2("mesh4", "spnza_bricks_a_diff");
	ApplyMateril2("mesh5", "sponza_arch_diff");
	ApplyMateril2("mesh6", "sponza_ceiling_a_diff");
	ApplyMateril2("mesh7", "sponza_column_a_diff");
	ApplyMateril2("mesh8", "sponza_floor_a_diff");
	ApplyMateril2("mesh9", "sponza_column_c_diff");
	ApplyMateril2("mesh10", "sponza_details_diff");
	ApplyMateril2("mesh11", "sponza_column_b_diff");
	ApplyMateril2("mesh13", "sponza_flagpole_diff");
	ApplyMateril2("mesh14", "sponza_fabric_green_diff");
	ApplyMateril2("mesh15", "sponza_fabric_blue_diff");
	ApplyMateril2("mesh16", "sponza_fabric_diff");
	ApplyMateril2("mesh17", "sponza_curtain_blue_diff");
	ApplyMateril2("mesh18", "sponza_curtain_diff");
	ApplyMateril2("mesh19", "sponza_curtain_green_diff");
	ApplyMateril2("mesh21", "vase_hanging");
	ApplyMateril2("mesh22", "vase_dif");
	ApplyMateril2("mesh23", "lion");
	ApplyMateril2("mesh24", "sponza_roof_diff");

	auto transform = Camera::main()->gameObject()->transform();
	transform->setPosition(5, 8, 0);
	transform->setLocalEulerAngles(30, -90, 0);

	transform = Camera::mainGameCamera()->gameObject()->transform();
	transform->setPosition(5, 8, 0);
	transform->setLocalEulerAngles(30, -90, 0);
#else
	GameObject::CreatePrimitive(PrimitiveType::Cube);
	auto plane = GameObject::CreatePrimitive(PrimitiveType::Plane);
	plane->transform()->setLocalScale(10);
	plane->transform()->setLocalPosition(0, -5, 0);
	GameObjectPtr model = AssetDatabase::LoadAssetAtPath<Prefab>("Assets/testFBX.fbx")->rootGameObject();
	Scene::AddGameObject(model);
	
	model = AssetDatabase::LoadAssetAtPath<Prefab>("Assets/unitychan.fbx")->rootGameObject();
	Scene::AddGameObject(model);
	model->transform()->setLocalScale(0.1f);
//	auto sphere = model->transform()->children().front().lock()->gameObject();
//	sphere->GetComponent<MeshRenderer>()->SetMaterial(Material::defaultMaterial());
#endif

#endif

}

	//bool MainEditor::m_inPlayMode = false;

	Action MainEditor::OnInitialized;
	std::unique_ptr<SceneViewEditor>  MainEditor::m_mainSceneViewEditor;

	void MainEditor::Init()
	{
		m_mainSceneViewEditor = std::make_unique<SceneViewEditor>();
		//Screen::set(width(), height());
		//PhysicsSystem::Init();
		EditorResources::Init();
		RenderSystem::Init();

		ModelImporter::Init();

		m_mainSceneViewEditor->Init();
		DefaultScene();

		glClearColor(1.0f, 0.0f, 0.0f, 1);
		OnInitialized();
	}

	void MainEditor::Run()
	{
		GLint framebuffer; // qt's framebuffer
		glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &framebuffer);
		
		//Input::Update();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (Applicaiton::isPlaying())
		{
			Scene::Update();
			PhysicsSystem::FixedUpdate();
		}
		else
		{
			m_mainSceneViewEditor->Update();
		}
		m_mainSceneViewEditor->Render();

		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		//Graphics::Blit()
		glViewport(0, 0, Screen::width(), Screen::height());
		auto quad = Mesh::builtinMesh(PrimitiveType::ScreenAlignedQuad);
		auto mtl = Material::builtinMaterial("DrawQuad");
		mtl->setMainTexture(m_mainSceneViewEditor->m_colorBuffer);
		Graphics::DrawMesh(quad, mtl);
		//Debug::Log("paintGL");

		Input::Update();
	}

	void MainEditor::Play()
	{
//		ofstream fout("temp.yaml");
//		YAMLOutputArchive archive(fout);
//		for (auto const & go : Scene::GameObjects())
//		{
//			archive << go;
//		}
		
		Applicaiton::s_isPlaying = true;
		PhysicsSystem::Init();
		//m_inPlayMode = true;
		Camera::setMainCamera(nullptr);
		//Camera::m_mainCamera = nullptr;
		Scene::Start();
	}

	void MainEditor::Stop()
	{
		//m_inPlayMode = false;
		Applicaiton::s_isPlaying = false;
		//Camera::m_mainCamera = EditorGUI::m_mainSceneViewEditor->camera();
		Camera::setMainCamera(m_mainSceneViewEditor->camera());
		PhysicsSystem::Clean();
	}

	void MainEditor::Resize(int width, int height)
	{
		float ratio = Screen::pixelsPerPoint();
		m_mainSceneViewEditor->Resize(width*ratio, height*ratio);
	}

	void MainEditor::Clean()
	{
	}

	void MainEditor::NewScene()
	{

	}
}
