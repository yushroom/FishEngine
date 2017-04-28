#include "MainEditor.hpp"

#include <iostream>
//#include <boost/dll/import.hpp>
#include <boost/lexical_cast.hpp>
#include <QDir>
#include <QCoreApplication>

#include <Debug.hpp>
#include <Camera.hpp>
#include <GameObject.hpp>
#include <ModelImporter.hpp>
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
//#include <Prefab.hpp>
#include <Animation.hpp>
#include <AnimationClip.hpp>
#include <Time.hpp>
#include <AudioSystem.hpp>
#include <AudioSource.hpp>
#include <AudioClip.hpp>
#include <CapsuleCollider.hpp>
#include <Rigidbody.hpp>

#include "SceneViewEditor.hpp"
#include "Selection.hpp"
#include "EditorGUI.hpp"
#include "SceneViewEditor.hpp"
#include "AssetDataBase.hpp"
#include "EditorResources.hpp"
#include "ScriptManager.hpp"


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
		auto sponza_model = AssetDatabase::LoadAssetAtPath2<GameObject>("Assets/sponza.fbx");
		auto sponza_go = Object::Instantiate(sponza_model);
		
#if 1
		auto shader1 = AssetDatabase::LoadAssetAtPath2<Shader>("Assets/diffuse_mask_twosided.shader");
		assert(shader1 != nullptr);
		auto applyMaterial1 = [&sponza_go, &shader1] (const std::string& diffuse_tex, const std::string& mask_tex, MaterialPtr mtl)
		{
			//auto mtl = Material::CreateMaterial();
			//mtl->setName(diffuse_tex);
			mtl->setShader(shader1);
			auto diffuse = AssetDatabase::LoadAssetAtPath2<Texture>("Assets/textures/"+ diffuse_tex + ".png");
			assert(diffuse != nullptr);
			auto mask = AssetDatabase::LoadAssetAtPath2<Texture>("Assets/textures/"+ mask_tex + ".png");
			assert(mask != nullptr);
			mtl->SetTexture("DiffuseTex", diffuse);
			mtl->SetTexture("MaskTex", mask);
			mtl->SetFloat("Roughness", 0.5f);
			mtl->SetFloat("Specular", 0.5f);
			//return mtl;
		};

		
		auto shader2 = AssetDatabase::LoadAssetAtPath2<Shader>("Assets/diffuse_bump.shader");
		assert(shader2 != nullptr);
		auto createMaterial2 = [&shader2] (const std::string& diffuse_tex) -> MaterialPtr
		{
			auto mtl = Material::CreateMaterial();
			mtl->setName(diffuse_tex);
			mtl->setShader(shader2);
			auto diffuse = AssetDatabase::LoadAssetAtPath2<Texture>("Assets/textures/"+ diffuse_tex + ".png");
			assert(diffuse != nullptr);
			mtl->SetFloat("Roughness", 0.5f);
			mtl->SetFloat("Specular", 0.5f);
			mtl->SetTexture("DiffuseTex", diffuse);
			return mtl;
		};

		auto sponza_thron = FindNamedChild(sponza_go, "sponza_00")->GetComponent<Renderer>()->material();
		applyMaterial1("sponza_thorn_diff", "sponza_thorn_mask", sponza_thron);

		auto vase_plant = FindNamedChild(sponza_go, "sponza_01")->GetComponent<Renderer>()->material();
		applyMaterial1("vase_plant", "vase_plant_mask", vase_plant);

		//auto material = createMaterial2("background");
		//material = createMaterial2("lion");
		//material = createMaterial2("spnza_bricks_a_diff");
		//material = createMaterial2("sponza_arch_diff");
		//material = createMaterial2("sponza_ceiling_a_diff");
		//material = createMaterial2("sponza_column_a_diff");
		//material = createMaterial2("sponza_column_b_diff");
		//material = createMaterial2("sponza_column_c_diff");
		//material = createMaterial2("sponza_curtain_blue_diff");
		//material = createMaterial2("sponza_curtain_diff");
		//material = createMaterial2("sponza_curtain_green_diff");
		//material = createMaterial2("sponza_details_diff");
		//material = createMaterial2("sponza_fabric_blue_diff");
		//material = createMaterial2("sponza_fabric_diff");
		//material = createMaterial2("sponza_fabric_green_diff");
		//material = createMaterial2("sponza_flagpole_diff");
		//material = createMaterial2("sponza_floor_a_diff");
		//material = createMaterial2("sponza_roof_diff");
		//material = createMaterial2("vase_dif");
		//material = createMaterial2("vase_hanging");
#endif
		
		
//		ApplyMateril1("mesh1", "vase_plant", "vase_plant_mask");
//		ApplyMateril1("mesh20", "chain_texture", "chain_texture_mask");
		
		auto transform = Camera::main()->gameObject()->transform();
		transform->setPosition(5, 8, 0);
		transform->setLocalEulerAngles(30, -90, 0);
		
		transform = Camera::mainGameCamera()->gameObject()->transform();
		transform->setPosition(5, 8, 0);
		transform->setLocalEulerAngles(30, -90, 0);
		
		transform = Light::mainLight()->gameObject()->transform();
		transform->setLocalEulerAngles(50, -75, 0);
	}
	
	
	void InitializeScene_UnityChan()
	{
		auto light_go = Light::mainLight()->gameObject();
		//QualitySettings::setShadowDistance(20);
		light_go->transform()->setLocalEulerAngles(50, 150, 0);
		auto plane = GameObject::CreatePrimitive(PrimitiveType::Plane);

		auto cube = GameObject::CreatePrimitive(PrimitiveType::Cube);
		cube->transform()->setLocalScale(10, 0.1f, 10);
		cube->transform()->setLocalPosition(0, -0.051f, 0);
		
		auto bodyTexture = AssetDatabase::LoadAssetAtPath2<Texture>("Assets/Textures/body_01.tga");
		auto skinTexture = AssetDatabase::LoadAssetAtPath2<Texture>("Assets/Textures/skin_01.tga");
		auto hairTexture = AssetDatabase::LoadAssetAtPath2<Texture>("Assets/Textures/hair_01.tga");
		auto faceTexture = AssetDatabase::LoadAssetAtPath2<Texture>("Assets/Textures/face_00.tga");
		auto eyelineTexture = AssetDatabase::LoadAssetAtPath2<Texture>("Assets/Textures/eyeline_00.tga");
		auto eyeirisLTexture = AssetDatabase::LoadAssetAtPath2<Texture>("Assets/Textures/eye_iris_L_00.tga");
		auto eyeirisRTexture = AssetDatabase::LoadAssetAtPath2<Texture>("Assets/Textures/eye_iris_R_00.tga");
		auto cheekTexture = AssetDatabase::LoadAssetAtPath2<Texture>("Assets/Textures/cheek_00.tga");
		
		auto rolloffTexture = AssetDatabase::LoadAssetAtPath2<Texture>("Assets/Textures/FO_CLOTH1.tga");
		auto rimLightTexture = AssetDatabase::LoadAssetAtPath2<Texture>("Assets/Textures/FO_RIM1.tga");
		auto specularTexture = AssetDatabase::LoadAssetAtPath2<Texture>("Assets/Textures/body_01_SPEC.tga");
		auto envTexture = AssetDatabase::LoadAssetAtPath2<Texture>("Assets/Textures/ENV2.tga");
		auto normalMapTexture = AssetDatabase::LoadAssetAtPath2<Texture>("Assets/Textures/body_01_NRM.tga");
		
		auto stageBaseTexture = AssetDatabase::LoadAssetAtPath2<Texture>("Assets/Textures/unitychan_tile3.png");
		auto stageMaskTexture = AssetDatabase::LoadAssetAtPath2<Texture>("Assets/Textures/AlphaMask.png");
		
		auto material = Material::InstantiateBuiltinMaterial("Diffuse");
		material->setMainTexture(stageBaseTexture);
		plane->GetComponent<Renderer>()->SetMaterial(material);
		
		auto chanMainShader = AssetDatabase::LoadAssetAtPath2<Shader>("Assets/CharaMain.shader");
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
		auto texture2side = AssetDatabase::LoadAssetAtPath2<Shader>("Assets/TextureDoubleSided.surf");
		material = Material::CreateMaterial();
		material->setShader(texture2side);
		//material = bodyMaterial;
		//material = Material::builtinMaterial("SkinnedMesh");
		material->SetTexture("_MainTex", bodyTexture);
		auto outline_shader = AssetDatabase::LoadAssetAtPath2<Shader>("Assets/Outline.shader");
		auto outline_material = Material::CreateMaterial();
		outline_material->setShader(outline_shader);
		outline_material->setName("Outline");
		outline_material->SetVector4("_Color", Vector4(1, 1, 1, 1));
		outline_material->setMainTexture(bodyTexture);
		outline_material->SetFloat("_EdgeThickness", edgeThickness);
		
		auto model = AssetDatabase::LoadAssetAtPath2<GameObject>("Assets/unitychan.fbx");
		auto modelGO = Object::Instantiate(model);
		auto unitychan_contrl = ScriptManager::GetInstance().CreateScript("UnityChanControlScriptWithRgidBody");
		modelGO->AddComponent(unitychan_contrl);

		//auto collider = modelGO->AddComponent<CapsuleCollider>();
		//collider->setCenter(Vector3(0, 0.75, 0));
		//collider->setRadius(0.5f);
		//collider->setHeight(1.5f);
		//modelGO->AddComponent<Rigidbody>();
		
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

		//auto model_run = AssetDatabase::LoadAssetAtPath2<GameObject>("Assets/unitychan_RUN00_F.fbx");
		auto animationClip = AssetDatabase::LoadAssetAtPath2<AnimationClip>("Assets/unitychan_RUN00_F.fbx");
		auto animation = modelGO->GetComponent<Animation>();
		animation->m_clip = animationClip;
	}
	

	void InitializeScene_UnityChan_crs()
	{
		auto model = AssetDatabase::LoadAssetAtPath2<GameObject>("Assets/UnityChan/CandyRockStar/CandyRockStar.fbx");
		auto modelGO = Object::Instantiate(model);
		auto animation = modelGO->AddComponent<Animation>();
		animation->m_clip = AssetDatabase::LoadAssetAtPath2<AnimationClip>("Assets/UnityChan/Animations/C86unitychan_001_SAK01_Final.fbx");

		auto unite_in_the_sky = AssetDatabase::LoadAssetAtPath2<AudioClip>("Assets/UniteInTheSky/Unite In The Sky (full).mp3");
		auto audio_source = modelGO->AddComponent<AudioSource>();
		audio_source->setClip(unite_in_the_sky);

		auto stage_model = AssetDatabase::LoadAssetAtPath2<GameObject>("Assets/UnityChanStage/Models/stage.fbx");
		auto stage_go = Object::Instantiate(stage_model);

		auto Light_Decay = AssetDatabase::LoadAssetAtPath2<Texture>("Assets/UnityChanStage/Effects/Light Beam/Light Decay.psd");
		auto Smoke_L = AssetDatabase::LoadAssetAtPath2<Texture>("Assets/UnityChanStage/Effects/Light Beam/Smoke L.psd");
		auto Smoke_S = AssetDatabase::LoadAssetAtPath2<Texture>("Assets/UnityChanStage/Effects/Light Beam/Smoke S.psd");
		auto light_beam_shader = AssetDatabase::LoadAssetAtPath2<Shader>("Assets/UnityChanStage/Effects/Shaders/Light Beam.shader");
		auto light_beam_material = Material::CreateMaterial();
		light_beam_material->setShader(light_beam_shader);
		light_beam_material->setName("LightBeam");
		light_beam_material->setColor(Color(201.0f / 255.0f, 1.0f, 180 / 255.0f));
		light_beam_material->setMainTexture(Light_Decay);
		light_beam_material->SetTexture("_NoiseTex1", Smoke_L);
		light_beam_material->SetTexture("_NoiseTex2", Smoke_S);
		light_beam_material->SetVector4("_NoiseScale", Vector4(0.3f, 0.3f, 0.3f, 0.3f));
		light_beam_material->SetVector4("_NoiseSpeed", Vector4(0.03f, -0.0931f, -0.012f, 0.0435f));
		auto light_beam_model = AssetDatabase::LoadAssetAtPath2<GameObject>("Assets/UnityChanStage/Effects/Light Beam/Light Beam.fbx");
		auto light_beam_go = Object::Instantiate(light_beam_model);
		light_beam_go->GetComponentInChildren<Renderer>()->SetMaterial(light_beam_material);

#define MATERIAL(shader_name, mat_name) \
		auto shader_name = AssetDatabase::LoadAssetAtPath2<Shader>("Assets/UnityChan/CandyRockStar/Shader/" #shader_name ".shader"); \
		MaterialPtr mat_name = Material::CreateMaterial(); \
		mat_name->setShader(shader_name); \
		mat_name->setName(#mat_name); \

		MATERIAL(Unitychan_chara_fuku_ds, body);
		MATERIAL(Unitychan_chara_fuku, Effector);
		MATERIAL(Unitychan_chara_akarami_blend, body_trans);
		MATERIAL(Unitychan_chara_hada, skin);
		MATERIAL(Unitychan_chara_hair_ds, hair);
		MATERIAL(Unitychan_chara_eye_blend, eye_L1);
		MATERIAL(Unitychan_chara_eye, eyebase);
		MATERIAL(Unitychan_chara_eyelash_blend, eyeline);
#undef MATERIAL

		MaterialPtr face = Material::CreateMaterial();
		face->setShader(Unitychan_chara_hada);
		face->setName("face");

		MaterialPtr mat_cheek = Material::CreateMaterial();
		mat_cheek->setShader(Unitychan_chara_akarami_blend);
		mat_cheek->setName("mat_cheek");

		MaterialPtr eye_R1 = Material::CreateMaterial();
		eye_R1->setShader(Unitychan_chara_eye_blend);
		eye_R1->setName("eye_R1");

		Path texture_root = "Assets/UnityChan/CandyRockStar/Textures";
		auto body_00   = AssetDatabase::LoadAssetAtPath2<Texture>( texture_root / "body_00.tga" );
		auto FO_CLOTH1 = AssetDatabase::LoadAssetAtPath2<Texture>( texture_root / "FO_CLOTH1.tga" );
		auto FO_RIM1   = AssetDatabase::LoadAssetAtPath2<Texture>(texture_root / "FO_RIM1.tga");
		auto body_00_SPEC = AssetDatabase::LoadAssetAtPath2<Texture>(texture_root / "body_00_SPEC.tga");
		auto ENV2 = AssetDatabase::LoadAssetAtPath2<Texture>(texture_root / "ENV2.tga");
		auto body_00_NRM = AssetDatabase::LoadAssetAtPath2<Texture>(texture_root / "body_00_NRM.tga");

		auto body_01 = AssetDatabase::LoadAssetAtPath2<Texture>(texture_root / "body_01.tga");
		auto body_01_NRM = AssetDatabase::LoadAssetAtPath2<Texture>(texture_root / "body_01_NRM.tga");
		auto body_01_SPEC = AssetDatabase::LoadAssetAtPath2<Texture>(texture_root / "body_01_SPEC.tga");

		auto skin_00 = AssetDatabase::LoadAssetAtPath2<Texture>(texture_root / "skin_00.tga");
		auto FO_SKIN1 = AssetDatabase::LoadAssetAtPath2<Texture>(texture_root / "FO_SKIN1.tga");

		auto cheek_00 = AssetDatabase::LoadAssetAtPath2<Texture>(texture_root / "cheek_00.tga");

		auto face_00 = AssetDatabase::LoadAssetAtPath2<Texture>(texture_root / "face_00.tga");

		auto hair_01 = AssetDatabase::LoadAssetAtPath2<Texture>(texture_root / "hair_01.tga");
		auto hair_01_NRM = AssetDatabase::LoadAssetAtPath2<Texture>(texture_root / "hair_01_NRM.tga");
		auto hair_01_SPEC = AssetDatabase::LoadAssetAtPath2<Texture>(texture_root / "hair_01_SPEC.tga");

		auto eye_iris_L_00 = AssetDatabase::LoadAssetAtPath2<Texture>(texture_root / "eye_iris_L_00.tga");
		auto eye_iris_R_00 = AssetDatabase::LoadAssetAtPath2<Texture>(texture_root / "eye_iris_R_00.tga");
		auto eyeline_00 = AssetDatabase::LoadAssetAtPath2<Texture>(texture_root / "eyeline_00.tga");

		body->setMainTexture(body_00);
		body->setColor(Color::white);
		body->SetVector4("_ShadowColor", Vector4(0.8f, 0.8f, 1.0f, 1.0f));
		body->SetFloat("_SpecularPower", 20.0f);
		body->SetFloat("_EdgeThickness", 0.5f);
		body->SetTexture("_FalloffSampler", FO_CLOTH1);
		body->SetTexture("_RimLightSampler", FO_RIM1);
		body->SetTexture("_SpecularReflectionSampler", body_00_SPEC);
		body->SetTexture("_EnvMapSampler", ENV2);
		body->SetTexture("_NormalMapSampler", body_00_NRM);
		for (auto & name : { "BodyParts_new01", "Globe_new01", "HairBand_DS_new01", "Jacket_BK_new01",
			"Jacket_FR_new01", "jacketEri_new01", "JacketSode_new01", "Kneeso_new01", "Mizugi_new01",
			"Pants_new01", "Shoes_new01" }) {
			auto child = FindNamedChild(modelGO, name);
			assert(child != nullptr);
			auto renderer = child->GetComponent<Renderer>();
			renderer->SetMaterial(body);
		}

		Effector->setColor(Color::white);
		Effector->SetVector4("_ShadowColor", Vector4(0.8f, 0.8f, 1.0f, 1.0f));
		Effector->SetFloat("_SpecularPower", 20.0f);
		Effector->SetFloat("_EdgeThickness", 0.5f);
		Effector->setMainTexture(body_01);
		Effector->SetTexture("_FalloffSampler", FO_CLOTH1);
		Effector->SetTexture("_RimLightSampler", FO_RIM1);
		Effector->SetTexture("_SpecularReflectionSampler", body_01_SPEC);
		Effector->SetTexture("_EnvMapSampler", ENV2);
		Effector->SetTexture("_NormalMapSampler", body_01_NRM);
		for (auto & name : { "Effector_new01" })
		{
			auto child = FindNamedChild(modelGO, name);
			assert(child != nullptr);
			auto renderer = child->GetComponent<Renderer>();
			renderer->SetMaterial(Effector);
		}

		body_trans->SetVector4("_Color", Vector4(1, 1, 1, 1));
		body_trans->SetVector4("_ShadowColor", Vector4(0.8f, 0.8f, 1, 1));
		body_trans->SetFloat("_EdgeThickness", 1);
		body_trans->setMainTexture(body_00);
		body_trans->SetTexture("_FalloffSampler", FO_CLOTH1);
		body_trans->SetTexture("_RimLightSampler", FO_RIM1);
		for (auto & name : { "JacketFrill_DS_new01", "PantsFrill_DS_new01" })
		{
			auto child = FindNamedChild(modelGO, name);
			assert(child != nullptr);
			auto renderer = child->GetComponent<Renderer>();
			renderer->SetMaterial(body_trans);
		}

		mat_cheek->SetVector4("_Color", Vector4(1, 1, 1, 1));
		mat_cheek->SetVector4("_ShadowColor", Vector4(0.8f, 0.8f, 1, 1));
		mat_cheek->SetFloat("_EdgeThickness", 1);
		mat_cheek->setMainTexture(cheek_00);
		mat_cheek->SetTexture("_FalloffSampler", FO_SKIN1);
		mat_cheek->SetTexture("_RimLightSampler", FO_RIM1);
		for (auto & name : { "cheek" })
		{
			auto child = FindNamedChild(modelGO, name);
			assert(child != nullptr);
			auto renderer = child->GetComponent<Renderer>();
			renderer->SetMaterial(mat_cheek);
		}

		skin->SetVector4("_Color", Vector4(1, 1, 1, 1));
		skin->SetVector4("_ShadowColor", Vector4(0.8f, 0.8f, 1, 1));
		skin->SetFloat("_EdgeThickness", 1);
		skin->setMainTexture(skin_00);
		skin->SetTexture("_FalloffSampler", FO_SKIN1);
		skin->SetTexture("_RimLightSampler", FO_RIM1);
		for (auto & name : { "Skin" })
		{
			auto child = FindNamedChild(modelGO, name);
			assert(child != nullptr);
			auto renderer = child->GetComponent<Renderer>();
			renderer->SetMaterial(skin);
		}

		face->SetVector4("_Color", Vector4(1, 1, 1, 1));
		face->SetVector4("_ShadowColor", Vector4(0.8f, 0.8f, 1, 1));
		face->SetFloat("_EdgeThickness", 1);
		face->setMainTexture(face_00);
		face->SetTexture("_FalloffSampler", FO_SKIN1);
		face->SetTexture("_RimLightSampler", FO_RIM1);
		for (auto & name : { "EYE_DEF", "head_back", "MTH_DEF" })
		{
			auto child = FindNamedChild(modelGO, name);
			assert(child != nullptr);
			auto renderer = child->GetComponent<Renderer>();
			renderer->SetMaterial(face);
		}

		hair->setMainTexture(hair_01);
		hair->setColor(Color::white);
		hair->SetVector4("_ShadowColor", Vector4(0.8f, 0.8f, 1.0f, 1.0f));
		hair->SetFloat("_SpecularPower", 20.0f);
		hair->SetFloat("_EdgeThickness", 0.5f);
		hair->SetTexture("_FalloffSampler", FO_CLOTH1);
		hair->SetTexture("_RimLightSampler", FO_RIM1);
		hair->SetTexture("_SpecularReflectionSampler", hair_01_SPEC);
		hair->SetTexture("_EnvMapSampler", ENV2);
		hair->SetTexture("_NormalMapSampler", body_00_NRM);
		for (auto & name : { "HairBack_DS_new01", "HairFront_DS_new01", "HairSide_DS_new01" }) {
			auto child = FindNamedChild(modelGO, name);
			assert(child != nullptr);
			auto renderer = child->GetComponent<Renderer>();
			renderer->SetMaterial(hair);
		}

		eye_L1->SetVector4("_Color", Vector4(1, 1, 1, 1));
		eye_L1->SetVector4("_ShadowColor", Vector4(0.8f, 0.8f, 1, 1));
		eye_L1->SetFloat("_EdgeThickness", 1);
		eye_L1->setMainTexture(eye_iris_L_00);
		eye_L1->SetTexture("_FalloffSampler", FO_SKIN1);
		eye_L1->SetTexture("_RimLightSampler", FO_RIM1);
		for (auto & name : { "eye_L_old" }) {
			auto child = FindNamedChild(modelGO, name);
			assert(child != nullptr);
			auto renderer = child->GetComponent<Renderer>();
			renderer->SetMaterial(eye_L1);
		}


		eye_R1->SetVector4("_Color", Vector4(1, 1, 1, 1));
		eye_R1->SetVector4("_ShadowColor", Vector4(0.8f, 0.8f, 1, 1));
		eye_R1->SetFloat("_EdgeThickness", 1);
		eye_R1->setMainTexture(eye_iris_R_00);
		eye_R1->SetTexture("_FalloffSampler", FO_SKIN1);
		eye_R1->SetTexture("_RimLightSampler", FO_RIM1);
		for (auto & name : { "eye_R_old" }) {
			auto child = FindNamedChild(modelGO, name);
			assert(child != nullptr);
			auto renderer = child->GetComponent<Renderer>();
			renderer->SetMaterial(eye_R1);
		}


		eyebase->SetVector4("_Color", Vector4(1, 1, 1, 1));
		eyebase->SetVector4("_ShadowColor", Vector4(0.8f, 0.8f, 1, 1));
		eyebase->SetFloat("_EdgeThickness", 1);
		eyebase->setMainTexture(eyeline_00);
		eyebase->SetTexture("_FalloffSampler", FO_SKIN1);
		eyebase->SetTexture("_RimLightSampler", FO_RIM1);
		for (auto & name : { "eye_base_old" }) {
			auto child = FindNamedChild(modelGO, name);
			assert(child != nullptr);
			auto renderer = child->GetComponent<Renderer>();
			renderer->SetMaterial(eyebase);
		}

		eyeline->SetVector4("_Color", Vector4(1, 1, 1, 1));
		eyeline->SetVector4("_ShadowColor", Vector4(0.8f, 0.8f, 1, 1));
		eyeline->SetFloat("_EdgeThickness", 1);
		eyeline->setMainTexture(eyeline_00);
		eyeline->SetTexture("_FalloffSampler", FO_SKIN1);
		eyeline->SetTexture("_RimLightSampler", FO_RIM1);
		for (auto & name : { "BLW_DEF" }) {
			auto child = FindNamedChild(modelGO, name);
			assert(child != nullptr);
			auto renderer = child->GetComponent<Renderer>();
			renderer->SetMaterial(eyeline);
		}
	}
	
	
	void InitializeScene_PBR()
	{
		auto envmap = AssetDatabase::LoadAssetAtPath2<Texture>("Assets/uffizi_cross.dds");
		assert(envmap != nullptr);
		auto filtered_envmap = AssetDatabase::LoadAssetAtPath2<Texture>("Assets/uffizi_cross_128_filtered.dds");
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
		auto model = AssetDatabase::LoadAssetAtPath2<GameObject>("Assets/Terrain.obj");
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
		
		auto model = AssetDatabase::LoadAssetAtPath2<GameObject>("Assets/heavy_reference.fbx");
		model = Object::Instantiate(model);
		{
			auto tex = AssetDatabase::LoadAssetAtPath2<Texture>("Assets/ground.jpg");
			assert(tex != nullptr);
			auto material = Material::InstantiateBuiltinMaterial("Diffuse");
			material->setMainTexture(tex);
			plane->GetComponent<MeshRenderer>()->SetMaterial(material);
		}
		
		auto albedo_shader = AssetDatabase::LoadAssetAtPath2<Shader>("Assets/Albedo.surf");
		{
			auto tex = AssetDatabase::LoadAssetAtPath2<Texture>("Assets/hvyweapon_red.tga");
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
			auto tex = AssetDatabase::LoadAssetAtPath2<Texture>("Assets/heavy_head.tga");
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
		//ScriptManager::GetInstance().BuildScriptsInProject();
		auto rotator = ScriptManager::GetInstance().CreateScript("Rotator");
		cube->AddComponent(rotator);
	}


	void InitializeScene_TestAudio()
	{
		auto c_ogg = AssetDatabase::LoadAssetAtPath2<AudioClip>("Assets/c.ogg");
		auto swish = AssetDatabase::LoadAssetAtPath2<AudioClip>("Assets/swish.wav");
		//AudioSource::PlayClipAtPoint(c_ogg, Vector3::zero);
		auto go = Scene::CreateGameObject("Aduio");
		auto audio_source = go->AddComponent<AudioSource>();
		audio_source->setClip(swish);
	}

	void InitializeScene_Empty()
	{
		auto whiteCube = GameObject::CreatePrimitive(PrimitiveType::Cube);
		auto blackSperer = GameObject::CreatePrimitive(PrimitiveType::Sphere);
		

		auto white = Material::InstantiateBuiltinMaterial("Diffuse");
		white->setMainTexture(Texture2D::whiteTexture());
		whiteCube->GetComponent<Renderer>()->SetMaterial(white);

		auto black = Material::InstantiateBuiltinMaterial("Diffuse");
		black->setMainTexture(Texture2D::blackTexture());
		blackSperer->GetComponent<Renderer>()->SetMaterial(black);
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
		else if (projectName == "UnityChan-crs")
		{
			InitializeScene_UnityChan_crs();
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
		else if (projectName == "TestAudio")
		{
			InitializeScene_TestAudio();
		}
		else if (projectName == "Empty")
		{
			InitializeScene_Empty();
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
			static auto time = std::chrono::high_resolution_clock::now();
			auto now = std::chrono::high_resolution_clock::now();
			auto elapse = now - time;
			auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(elapse).count();
			time = now;
			Time::m_deltaTime = ms / 1000.0f;
			Scene::Update();
			PhysicsSystem::FixedUpdate();
			AudioSystem::Update();
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
		Time::m_time = 0;
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
