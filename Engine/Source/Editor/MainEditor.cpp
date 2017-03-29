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
	
	
	void InitializeScene_Sponza()
	{
		//QualitySettings::setShadowDistance(30);
		auto sponza_model = As<GameObject>(AssetDatabase::LoadAssetAtPath("Assets/sponza.fbx"));
		auto sponza_go = Object::Instantiate(sponza_model);
		
		
		auto shader1 = As<Shader>(AssetDatabase::LoadAssetAtPath("Assets/diffuse_mask_twosided.shader"));
		assert(shader1 != nullptr);
		auto createMaterial1 = [&sponza_go, &shader1] (const std::string& diffuse_tex, const std::string& mask_tex) -> MaterialPtr
		{
			auto mtl = Material::CreateMaterial();
			mtl->setName(diffuse_tex);
			mtl->setShader(shader1);
			auto diffuse = As<Texture>(AssetDatabase::LoadAssetAtPath("Assets/textures/"+ diffuse_tex + ".png"));
			assert(diffuse != nullptr);
			auto mask = As<Texture>(AssetDatabase::LoadAssetAtPath("Assets/textures/"+ mask_tex + ".png"));
			assert(mask != nullptr);
			mtl->SetTexture("DiffuseTex", diffuse);
			mtl->SetTexture("MaskTex", mask);
			mtl->SetFloat("Roughness", 0.5f);
			mtl->SetFloat("Specular", 0.5f);
			return mtl;
		};

		
		auto shader2 = As<Shader>(AssetDatabase::LoadAssetAtPath("Assets/diffuse_bump.shader"));
		assert(shader2 != nullptr);
		auto createMaterial2 = [&shader2] (const std::string& diffuse_tex) -> MaterialPtr
		{
			auto mtl = Material::CreateMaterial();
			mtl->setName(diffuse_tex);
			mtl->setShader(shader2);
			auto diffuse = As<Texture>(AssetDatabase::LoadAssetAtPath("Assets/textures/"+ diffuse_tex + ".png"));
			assert(diffuse != nullptr);
			mtl->SetFloat("Roughness", 0.5f);
			mtl->SetFloat("Specular", 0.5f);
			mtl->SetTexture("DiffuseTex", diffuse);
			return mtl;
		};
		
		
		auto ApplyMaterial = [&sponza_go](std::string const & go_name, MaterialPtr mtl)
		{
			auto go = FindNamedChild(sponza_go, go_name);
			assert(go != nullptr);
			go->GetComponent<MeshRenderer>()->SetMaterial(mtl);
		};
		
		
		
		std::string prefix = "sponza_";
		
		auto material = createMaterial2("background");
		for (auto i : {"03", "377", "378"})
		{
			ApplyMaterial(prefix + i, material);
		}
		
		material = createMaterial2("lion");
		ApplyMaterial("sponza_377", material);
		ApplyMaterial("sponza_378", material);
		
		material = createMaterial2("spnza_bricks_a_diff");
		for (auto i : {"05", "06", "34", "36", "66", "68", "69", "71", "72", "75", "116", "258", "379", "382"})
		{
			ApplyMaterial(prefix + i, material);
		}
		
		material = createMaterial2("sponza_arch_diff");
		for (auto i : {"07", "17", "20", "21", "37", "39", "41", "43", "45", "47", "49", "51", "53", "55", "56", "57", "58", "59", "60", "61", "62", "63", "64", "65", "67", "122", "123", "124"})
		{
			ApplyMaterial(prefix + i, material);
		}
		
		material = createMaterial2("sponza_ceiling_a_diff");
		for (auto i : {"08", "19", "35", "38", "40", "42", "44", "46", "48", "50", "52", "54"})
		{
			ApplyMaterial(prefix + i, material);
		}
		
		material = createMaterial2("sponza_column_a_diff");
		for (auto i : {"09", "10", "11", "12", "13", "14", "15", "16", "118", "119", "120", "121"})
		{
			ApplyMaterial(prefix + i, material);
		}
		
		
		material = createMaterial2("sponza_column_b_diff");
		for (int i = 125; i <= 256; ++i)
		{
			auto name = prefix + boost::lexical_cast<std::string>(i);
			ApplyMaterial(name, material);
		}
		
		material = createMaterial2("sponza_column_c_diff");
		for (int i = 22; i <= 33; ++i)
		{
			auto name = prefix + boost::lexical_cast<std::string>(i);
			ApplyMaterial(name, material);

		}
		for (int i = 76; i <= 115; ++i)
		{
			auto name = prefix + boost::lexical_cast<std::string>(i);
			ApplyMaterial(name, material);
		}
		
		material = createMaterial2("sponza_curtain_blue_diff");
		for (auto i : {"320", "326", "329"})
		{
			ApplyMaterial(prefix + i, material);
		}
		
		material = createMaterial2("sponza_curtain_diff");
		for (auto i : {"321", "323", "325", "328"})
		{
			ApplyMaterial(prefix + i, material);
		}
		
		material = createMaterial2("sponza_curtain_green_diff");
		for (auto i : {"322", "324", "327"})
		{
			ApplyMaterial(prefix + i, material);
		}
		
		material = createMaterial2("sponza_details_diff");
		for (auto i : {"70", "71", "72", "73", "74"})
		{
			ApplyMaterial(prefix + i, material);
		}
		
		material = createMaterial2("sponza_fabric_blue_diff");
		for (auto i : {"283", "286", "289"})
		{
			ApplyMaterial(prefix + i, material);
		}
		
		material = createMaterial2("sponza_fabric_diff");
		ApplyMaterial("sponza_284", material);
		ApplyMaterial("sponza_288", material);
		
		material = createMaterial2("sponza_fabric_green_diff");
		for (auto i : {"282", "285", "287"})
		{
			ApplyMaterial(prefix + i, material);
		}
		
		material = createMaterial2("sponza_flagpole_diff");
		for (int i = 259; i <= 274; ++i)
		{
			auto name = prefix + boost::lexical_cast<std::string>(i);
			ApplyMaterial(name, material);
		}
		for (int i = 290; i <= 319; ++i)
		{
			auto name = prefix + boost::lexical_cast<std::string>(i);
			ApplyMaterial(name, material);
		}
		
		material = createMaterial2("sponza_floor_a_diff");
		ApplyMaterial("sponza_18", material);
		ApplyMaterial("sponza_117", material);
		
		material = createMaterial2("sponza_roof_diff");
		ApplyMaterial("sponza_380", material);
		ApplyMaterial("sponza_381", material);
		
		
		material = createMaterial2("vase_dif");
		for (int i = 373; i <= 376; ++i)
		{
			auto name = prefix + boost::lexical_cast<std::string>(i);
			ApplyMaterial(name, material);
		}
		
		material = createMaterial2("vase_hanging");
		for (int i = 334; i <= 338; ++i)
		{
			auto name = prefix + boost::lexical_cast<std::string>(i);
			ApplyMaterial(name, material);
		}
		for (int i = 343; i <= 347; ++i)
		{
			auto name = prefix + boost::lexical_cast<std::string>(i);
			ApplyMaterial(name, material);
		}
		for (int i = 352; i <= 356; ++i)
		{
			auto name = prefix + boost::lexical_cast<std::string>(i);
			ApplyMaterial(name, material);
		}
		for (int i = 361; i <= 365; ++i)
		{
			auto name = prefix + boost::lexical_cast<std::string>(i);
			ApplyMaterial(name, material);
		}
		
		
		material = createMaterial1("sponza_thorn_diff", "sponza_thorn_mask");
		ApplyMaterial("sponza_00", material);
		for (int i = 275; i <= 281; ++i)
		{
			auto name = prefix + boost::lexical_cast<std::string>(i);
			ApplyMaterial(name, material);
		}
		
		
//		ApplyMateril1("mesh1", "vase_plant", "vase_plant_mask");
//		ApplyMateril1("mesh20", "chain_texture", "chain_texture_mask");
		
		auto transform = Camera::main()->gameObject()->transform();
		transform->setPosition(5, 8, 0);
		transform->setLocalEulerAngles(30, -90, 0);
		
		transform = Camera::mainGameCamera()->gameObject()->transform();
		transform->setPosition(5, 8, 0);
		transform->setLocalEulerAngles(30, -90, 0);
	}
	
	
	void InitializeScene_UnityChan()
	{
		auto light_go = Light::mainLight()->gameObject();
		//QualitySettings::setShadowDistance(20);
		light_go->transform()->setLocalEulerAngles(50, 150, 0);
		auto plane = GameObject::CreatePrimitive(PrimitiveType::Plane);
		
		//		auto sky_texture = texture_importer.FromFile(textures_root / "StPeters" / "DiffuseMap.dds");
		//		auto checkboard_texture = texture_importer.FromFile(textures_root / "checkboard.png");
		//		auto chan_texture_dir = example_root / "textures";
		As<Texture>(AssetDatabase::LoadAssetAtPath("Assets/textures/body_01.tag"));
		auto bodyTexture = As<Texture>(AssetDatabase::LoadAssetAtPath("Assets/textures/body_01.tga"));
		auto skinTexture = As<Texture>(AssetDatabase::LoadAssetAtPath("Assets/textures/skin_01.tga"));
		auto hairTexture = As<Texture>(AssetDatabase::LoadAssetAtPath("Assets/textures/hair_01.tga"));
		auto faceTexture = As<Texture>(AssetDatabase::LoadAssetAtPath("Assets/textures/face_00.tga"));
		auto eyelineTexture = As<Texture>(AssetDatabase::LoadAssetAtPath("Assets/textures/eyeline_00.tga"));
		auto eyeirisLTexture = As<Texture>(AssetDatabase::LoadAssetAtPath("Assets/textures/eye_iris_L_00.tga"));
		auto eyeirisRTexture = As<Texture>(AssetDatabase::LoadAssetAtPath("Assets/textures/eye_iris_R_00.tga"));
		auto cheekTexture = As<Texture>(AssetDatabase::LoadAssetAtPath("Assets/textures/cheek_00.tga"));
		
		//auto bodyTexture = Texture::CreateFromFile(chan_texture_dir + "cheek_00.tga");
		auto rolloffTexture = As<Texture>(AssetDatabase::LoadAssetAtPath("Assets/textures/FO_CLOTH1.tga"));
		auto rimLightTexture = As<Texture>(AssetDatabase::LoadAssetAtPath("Assets/textures/FO_RIM1.tga"));
		auto specularTexture = As<Texture>(AssetDatabase::LoadAssetAtPath("Assets/textures/body_01_SPEC.tga"));
		auto envTexture = As<Texture>(AssetDatabase::LoadAssetAtPath("Assets/textures/ENV2.tga"));
		auto normalMapTexture = As<Texture>(AssetDatabase::LoadAssetAtPath("Assets/textures/body_01_NRM.tga"));
		
		auto stageBaseTexture = As<Texture>(AssetDatabase::LoadAssetAtPath("Assets/textures/unitychan_tile3.png"));
		auto stageMaskTexture = As<Texture>(AssetDatabase::LoadAssetAtPath("Assets/textures/AlphaMask.png"));
		
		auto material = Material::InstantiateBuiltinMaterial("Diffuse");
		material->setMainTexture(stageBaseTexture);
		plane->GetComponent<Renderer>()->SetMaterial(material);
		
		auto chanMainShader = As<Shader>(AssetDatabase::LoadAssetAtPath("CharaMain.shader"));
		auto bodyMaterial = Material::CreateMaterial();
		bodyMaterial->setName("body");
		bodyMaterial->setShader(chanMainShader);
		bodyMaterial->SetVector4("_Color", Vector4(1, 1, 1, 1));
		bodyMaterial->SetFloat("_SpecularPower", 20.f);
		bodyMaterial->SetTexture("_MainTex", bodyTexture);
		bodyMaterial->SetTexture("_FalloffSampler", rolloffTexture);
		bodyMaterial->SetTexture("_RimLightSampler", rimLightTexture);
		bodyMaterial->SetTexture("_SpecularReflectionSampler", specularTexture);
		bodyMaterial->SetTexture("_EnvMapSampler", envTexture);
		bodyMaterial->SetTexture("_NormalMapSampler", normalMapTexture);
		
		constexpr float edgeThickness = 0.5f;
		auto texture2side = As<Shader>(AssetDatabase::LoadAssetAtPath("TextureDoubleSided.surf"));
		material = Material::CreateMaterial();
		material->setShader(texture2side);
		//material = bodyMaterial;
		//material = Material::builtinMaterial("SkinnedMesh");
		material->SetTexture("_MainTex", bodyTexture);
		auto outline_shader = As<Shader>(AssetDatabase::LoadAssetAtPath("Outline.shader"));
		auto outline_material = Material::CreateMaterial();
		outline_material->setShader(outline_shader);
		outline_material->setName("Outline");
		outline_material->SetVector4("_Color", Vector4(1, 1, 1, 1));
		outline_material->setMainTexture(bodyTexture);
		outline_material->SetFloat("_EdgeThickness", edgeThickness);
		
		auto model = As<GameObject>( AssetDatabase::LoadAssetAtPath("Assets/unitychan.fbx") );
		auto modelGO = Object::Instantiate(model);
		
		for (auto name : {"hairband", "button", "Leg", "Shirts", "shirts_sode", "shirts_sode_BK", "uwagi", "uwagi_BK", "hair_accce"}) {
			auto child = FindNamedChild(modelGO, name);
			assert(child != nullptr);
			auto renderer = child->GetComponent<Renderer>();
			renderer->SetMaterial(bodyMaterial);
			renderer->AddMaterial(outline_material);
			//renderer->setShadowCastingMode(ShadowCastingMode::Off);
		}
		
		material = Material::InstantiateBuiltinMaterial("Texture");
		material->SetTexture("_MainTex", skinTexture);
		material->setName("skin");
		for (auto name : {"skin"}) {
			auto child = FindNamedChild(modelGO, name);
			assert(child != nullptr);
			auto renderer = child->GetComponent<Renderer>();
			renderer->SetMaterial(material);
			renderer->AddMaterial(outline_material);
			//renderer->setShadowCastingMode(ShadowCastingMode::Off);
		}
		
		material = Material::InstantiateBuiltinMaterial("Texture");
		material->setName("face");
		material->SetTexture("_MainTex", faceTexture);
		for (auto name : {"MTH_DEF", "EYE_DEF", "head_back"}) {
			auto child = FindNamedChild(modelGO, name);
			assert(child != nullptr);
			auto renderer = child->GetComponent<Renderer>();
			renderer->SetMaterial(material);
			renderer->AddMaterial(outline_material);
			//renderer->setShadowCastingMode(ShadowCastingMode::Off);
		}
		
		material = Material::InstantiateBuiltinMaterial("Texture");
		material->setName("hair");
		material->SetTexture("_MainTex", hairTexture);
		for (auto name : {"hair_front", "hair_frontside", "tail", "tail_bottom"}) {
			auto child = FindNamedChild(modelGO, name);
			assert(child != nullptr);
			auto renderer = child->GetComponent<Renderer>();
			renderer->SetMaterial(material);
			//renderer->AddMaterial(outline_material);
			//renderer->setShadowCastingMode(ShadowCastingMode::Off);
		}
		
		material = Material::InstantiateBuiltinMaterial("Transparent");
		material->setName("eye_L1");
		material->SetTexture("_MainTex", eyeirisLTexture);
		for (auto name : {"eye_L_old"}) {
			auto child = FindNamedChild(modelGO, name);
			assert(child != nullptr);
			auto renderer = child->GetComponent<Renderer>();
			renderer->SetMaterial(material);
			renderer->setShadowCastingMode(ShadowCastingMode::Off);
			renderer->setReceiveShadows(false);
		}
		
		
		material = Material::InstantiateBuiltinMaterial("Transparent");
		material->setName("eye_R1");
		material->SetTexture("_MainTex", eyeirisRTexture);
		for (auto name : {"eye_R_old"}) {
			auto child = FindNamedChild(modelGO, name);
			assert(child != nullptr);
			auto renderer = child->GetComponent<Renderer>();
			renderer->SetMaterial(material);
			renderer->setShadowCastingMode(ShadowCastingMode::Off);
			renderer->setReceiveShadows(false);
		}
		
		material = Material::InstantiateBuiltinMaterial("Transparent");
		material->SetTexture("_MainTex", eyelineTexture);
		for (auto name : {"BLW_DEF", "EL_DEF"}) {
			auto child = FindNamedChild(modelGO, name);
			assert(child != nullptr);
			auto renderer = child->GetComponent<Renderer>();
			renderer->SetMaterial(material);
			//renderer->setShadowCastingMode(ShadowCastingMode::Off);
			//renderer->setReceiveShadows(false);
		}
		
		material = Material::InstantiateBuiltinMaterial("Transparent");
		material->setName("mat_cheek");
		material->SetTexture("_MainTex", cheekTexture);
		for (auto name : {"cheek"}) {
			auto child = FindNamedChild(modelGO, name);
			assert(child != nullptr);
			auto renderer = child->GetComponent<Renderer>();
			renderer->SetMaterial(material);
			renderer->setShadowCastingMode(ShadowCastingMode::Off);
		}
		
		material = Material::InstantiateBuiltinMaterial("Texture");
		material->setName("eyeline");
		material->SetTexture("_MainTex", eyelineTexture);
		for (auto name : {"eye_base_old"}) {
			auto child = FindNamedChild(modelGO, name);
			assert(child != nullptr);
			auto renderer = child->GetComponent<Renderer>();
			renderer->SetMaterial(material);
			renderer->setShadowCastingMode(ShadowCastingMode::Off);
			renderer->setReceiveShadows(false);
		}
	}
	
	
	void InitializeScene_PBR()
	{
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
	}
	
	
	void InitializeScene_TestShadow()
	{
		//QualitySettings::setShadowDistance(20);
		ModelImporter importer;
		auto model = As<GameObject>(AssetDatabase::LoadAssetAtPath("Assets/Terrain.obj"));
		auto terrainGO = Object::Instantiate(model);
		auto material = Material::InstantiateBuiltinMaterial("DebugCSM");
		//auto material = Material::defaultMaterial();
		//material->EnableKeyword(ShaderKeyword::Shadow);
		
		//TextureImporter texture_importer;
		//auto bakedAO = texture_importer.FromFile(root_dir + "bakedAO.jpg");
		//material->setMainTexture(bakedAO);
		//terrainGO->GetComponent<MeshRenderer>()->SetMaterial(material);
		terrainGO->GetComponentInChildren<Renderer>()->SetMaterial(material);
	}
	
	void InitializeScene_IllustrativeRendering()
	{
		auto light_go = Light::mainLight()->gameObject();
		//QualitySettings::setShadowDistance(20);
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
	}
	
	
	void InitializeScene_TestScript()
	{
		auto cube = GameObject::CreatePrimitive(PrimitiveType::Cube);
#if FISHENGINE_PLATFORM_WINDOWS
		FishEngine::Path shared_lib_path = Application::dataPath() / "../build/RelWithDebInfo/TestScript.dll";
#else
		FishEngine::Path shared_lib_path = Application::dataPath() / "../build/RelWithDebInfo/libTestScript.dylib";
#endif
		if ( !boost::filesystem::exists(shared_lib_path) )
		{
			Debug::LogError("%s not found", shared_lib_path.string().c_str());
			abort();
		}
		static auto createFunc = boost::dll::import<Script*(const char*)>(shared_lib_path, "CreateCustomScript");
		static auto deleteFunc = boost::dll::import<void(Script*)>(shared_lib_path, "DestroyCustomScript");
		auto rotator = std::shared_ptr<Script>(createFunc("Rotator"), [](auto s) {
			Debug::Log("Delete from dll");
			deleteFunc(s);
		});
		
		cube->AddComponent(rotator);
	}
	

	void DefaultScene()
	{
//	    auto camera = Camera::Create();
//	    auto camera_go = Scene::CreateGameObject("Main Camera");
//	    camera_go->AddComponent(camera);
//	    camera_go->AddComponent<CameraController>();
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
		
		auto projectName = Application::dataPath().parent_path().stem().string();
		if (projectName == "UnityChan")
		{
			InitializeScene_UnityChan();
		}
		else if (projectName == "Sponza")
		{
			InitializeScene_Sponza();
		}
		else if (projectName == "PBR")
		{
			InitializeScene_PBR();
		}
		else if (projectName == "Illustrative-Rendering")
		{
			InitializeScene_IllustrativeRendering();
		}
		else if (projectName == "TestShadow")
		{
			InitializeScene_TestShadow();
		}
		else if (projectName == "TestScript")
		{
			InitializeScene_TestScript();
		}

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
		mtl->SetVector4("DrawRectParameters", Vector4(-1, -1, 1, 1));
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
