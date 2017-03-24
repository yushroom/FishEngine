#include "MainEditor.hpp"

#include <iostream>
#include <boost/dll/import.hpp>
#include <boost/lexical_cast.hpp>
#include <QDir>
#include <QCoreApplication>

#include <Debug.hpp>
#include <Camera.hpp>
#include <GameObject.hpp>
#include <CameraController.hpp>
#include <ModelImporter.hpp>
#include <TextureImporter.hpp>
#include <Shader.hpp>
#include <Scene.hpp>
#include <Screen.hpp>
#include <Graphics.hpp>
#include <Light.hpp>
#include <RenderSettings.hpp>
#include <QualitySettings.hpp>
#include <MeshRenderer.hpp>
#include <SkinnedMeshRenderer.hpp>
#include <Material.hpp>
#include <PhysicsSystem.hpp>
#include <Application.hpp>
#include <Prefab.hpp>

#include "SceneViewEditor.hpp"
#include "Selection.hpp"
#include "EditorGUI.hpp"
#include "SceneViewEditor.hpp"
#include "AssetDataBase.hpp"
#include "EditorResources.hpp"

using namespace FishEngine;
using namespace std;

namespace FishEditor
{
	GameObjectPtr FindNamedChild(const GameObjectPtr & root, const std::string& name)
	{
		auto& children = root->transform()->children();
		for (auto const & g : children) {
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
#elif 0
		auto model = As<GameObject>( AssetDatabase::LoadAssetAtPath("Assets/testFBX.fbx") );
		Object::Instantiate(model);
	
		model = As<GameObject>( AssetDatabase::LoadAssetAtPath("Assets/unitychan.fbx") );
		model = Object::Instantiate(model);
		
#if FISHENGINE_PLATFORM_WINDOWS
		FishEngine::Path shared_lib_path = Application::dataPath() / "../build/RelWithDebInfo/Sponza.dll";
#else
		FishEngine::Path shared_lib_path = Application::dataPath() / "../build/RelWithDebInfo/libSponza.dylib";
#endif
		if ( !boost::filesystem::exists(shared_lib_path) )
		{
			Debug::LogError("%s not found", shared_lib_path.string().c_str());
			abort();
		}
		//FishEngine::Path shared_lib_path("/Users/yushroom/FishEngine/Projects/Sponza/build");
		//shared_lib_path /= "Sponza.dll";
		static auto createFunc = boost::dll::import<Script*(const char*)>(shared_lib_path, "CreateCustomScript");
		static auto deleteFunc = boost::dll::import<void(Script*)>(shared_lib_path, "DestroyCustomScript");
		auto rotator = std::shared_ptr<Script>(createFunc("Rotator"), [](auto s) {
			deleteFunc(s);
		});

		model->AddComponent(rotator);
#elif 0
		QualitySettings::setShadowDistance(20);
		light_go->transform()->setLocalEulerAngles(50, 150, 0);
		auto plane = GameObject::CreatePrimitive(PrimitiveType::Plane);

		auto model = As<GameObject>(AssetDatabase::LoadAssetAtPath("Assets/heavy_reference.fbx"));
		model = Object::Instantiate(model);
		{
			auto tex = As<Texture>(AssetDatabase::LoadAssetAtPath("Assets/ground.jpg"));
			assert(tex != nullptr);
			auto material = Material::InstantiateBuiltinMaterial("Diffuse");
			material->setMainTexture(tex);
			plane->GetComponent<MeshRenderer>()->SetMaterial(material);
		}

		auto albedo_shader = As<Shader>(AssetDatabase::LoadAssetAtPath("Assets/Albedo.surf"));
		{
			auto tex = As<Texture>(AssetDatabase::LoadAssetAtPath("Assets/hvyweapon_red.tga"));
			assert(tex != nullptr);
			//auto material = Material::InstantiateBuiltinMaterial("Diffuse");
			auto material = Material::CreateMaterial();
			material->setShader(albedo_shader);
			material->setMainTexture(tex);
			auto renderers = model->GetComponentsInChildren<SkinnedMeshRenderer>();
			for (auto & r : renderers)
			{
				r->SetMaterial(material);
			}
		}
		{
			auto tex = As<Texture>(AssetDatabase::LoadAssetAtPath("Assets/heavy_head.tga"));
			assert(tex != nullptr);
			//auto material = Material::InstantiateBuiltinMaterial("Diffuse");
			auto material = Material::CreateMaterial();
			material->setShader(albedo_shader);
			material->setMainTexture(tex);
			FindNamedChild(model, "head_mesh")->GetComponent<SkinnedMeshRenderer>()->SetMaterial(material);
		}
#else
		auto envmap = As<Texture>(AssetDatabase::LoadAssetAtPath("Assets/uffizi_cross.dds"));
		assert(envmap != nullptr);
		auto filtered_envmap = As<Texture>(AssetDatabase::LoadAssetAtPath("Assets/uffizi_cross_128_filtered.dds"));
		assert(filtered_envmap != nullptr);
		RenderSettings::setAmbientCubemap(filtered_envmap);
		auto skybox = Material::InstantiateBuiltinMaterial("SkyboxCubed");
		skybox->SetTexture("_Tex", envmap);
		skybox->SetVector4("_Tint", Vector4::one);
		skybox->SetFloat("_Exposure", 1);
		skybox->SetFloat("_Rotation", 0);
		RenderSettings::setSkybox(skybox);
		
		auto group = GameObject::Create();
		group->setName("Group");
		Scene::AddGameObject(group);
	
//		for (int x = -5; x < 5; ++x)
//		{
//			for (int y = -5; y <= 5; y++)
//			{
//				auto go = GameObject::CreatePrimitive(PrimitiveType::Sphere);
//				go->transform()->SetParent(group->transform());
//				go->transform()->setLocalPosition(y*0.6f, x*0.6f, 0);
//				go->transform()->setLocalEulerAngles(0, 30, 0);
//				go->transform()->setLocalScale(0.5f, 0.5f, 0.5f);
//				MaterialPtr material;
////				if (x == 0)
////				{
//					material = Material::InstantiateBuiltinMaterial("PBR");
//					material->setName("PBR" + boost::lexical_cast<std::string>(y+5));
//				//}
////				else
////				{
////					material = Material::InstantiateBuiltinMaterial("PBR-Reference");
////					material->setName("PBR-Reference" + boost::lexical_cast<std::string>(y + 5));
////				}
//				material->EnableKeyword(ShaderKeyword::AmbientIBL);
//				//material->DisableKeyword(ShaderKeyword::Shadow);
//				material->SetFloat("Metallic", 0.1f*(x+5));
//				material->SetFloat("Roughness", 0.1f*(y+5));
//				material->SetFloat("Specular", 0.5);
//				material->SetVector3("BaseColor", Vector3(1.f, 1.f, 1.f));
//				auto renderer = go->GetComponent<MeshRenderer>();
//				renderer->SetMaterial(material);
//				renderer->setShadowCastingMode(ShadowCastingMode::Off);
//			}
//		}
		
		for (int x = 0; x < 2; ++x)
		{
			for (int y = -5; y <= 5; y++)
			{
				auto go = GameObject::CreatePrimitive(PrimitiveType::Sphere);
				go->transform()->SetParent(group->transform());
				go->transform()->setLocalPosition(y*0.6f, x*0.6f, 0);
				go->transform()->setLocalEulerAngles(0, 30, 0);
				go->transform()->setLocalScale(0.5f, 0.5f, 0.5f);
				MaterialPtr material;
				if (x == 0)
				{
					material = Material::InstantiateBuiltinMaterial("PBR");
					material->setName("PBR" + boost::lexical_cast<std::string>(y+5));
				}
				else
				{
					material = Material::InstantiateBuiltinMaterial("PBR-Reference");
					material->setName("PBR-Reference" + boost::lexical_cast<std::string>(y + 5));
				}
				material->EnableKeyword(ShaderKeyword::AmbientIBL);
				//material->DisableKeyword(ShaderKeyword::Shadow);
				material->SetFloat("Metallic", 0.0f);
				material->SetFloat("Roughness", 0.1f*(y+5));
				material->SetFloat("Specular", 0.5);
				material->SetVector3("BaseColor", Vector3(1.f, 1.f, 1.f));
				auto renderer = go->GetComponent<MeshRenderer>();
				renderer->SetMaterial(material);
				renderer->setShadowCastingMode(ShadowCastingMode::Off);
			}
		}
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

		if (Application::isPlaying())
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
		
		Application::s_isPlaying = true;
		PhysicsSystem::Init();
		//m_inPlayMode = true;
		Camera::setMainCamera(nullptr);
		//Camera::m_mainCamera = nullptr;
		Scene::Start();
	}

	void MainEditor::Stop()
	{
		//m_inPlayMode = false;
		Application::s_isPlaying = false;
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
