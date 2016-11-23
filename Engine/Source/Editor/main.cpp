//#include "TestScript.hpp"
#include "TextureImporter.hpp"

#include <iostream>
#include <Camera.hpp>
#include <Scene.hpp>
#include <Shader.hpp>
#include <Material.hpp>
#include <GameObject.hpp>
#include <Transform.hpp>
#include <CameraController.hpp>
#include <Light.hpp>
#include <RenderSettings.hpp>
#include <App.hpp>
#include <MeshRenderer.hpp>
#include <ModelImporter.hpp>

#include "FishEditorWindow.hpp"

using namespace std;
using namespace FishEngine;
using namespace FishEditor;


void DefaultScene()
{
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

    auto material = Material::builtinMaterial("SkyboxProcedural");
    material->SetFloat("_AtmosphereThickness", 1.0);
    //material->SetFloat("_SunDisk", 2);
    material->SetFloat("_SunSize", 0.04f);
    material->SetVector4("_SkyTint", Vector4(0.5f, 0.5f, 0.5f, 1));
    material->SetVector4("_GroundColor", Vector4(.369f, .349f, .341f, 1));
    material->SetFloat("_Exposure", 1.3f);
    RenderSettings::setSkybox(material);
}


class TestPBR : public App
{
public:

    virtual void Init() override
    {
        DefaultScene();
#if FISHENGINE_PLATFORM_WINDOWS
        const std::string root_dir = "D:/program/FishEngine/Assets/";
        const std::string skybox_dir = R"(D:\program\github\Cinder-Experiments\common\textures\)";
        const std::string concrete_texture_dir = R"(D:\program\FishEngine\Example\PBR\concrete_rebarconcrete_pbr\TGA\concrete_rebarconcrete_1k_TGA\)";
        auto shader = Shader::CreateFromFile(R"(D:\program\FishEngine\Example\PBR\PBR.surf)");
#else
        auto shader = Shader::CreateFromFile("/Users/yushroom/program/graphics/FishEngine/Example/PBR/PBR.surf");
        const std::string concrete_texture_dir = "/Users/yushroom/program/graphics/FishEngine/Example/PBR/concrete_rebarconcrete_pbr/TGA/concrete_rebarconcrete_1k_TGA/";
        const std::string root_dir = "/Users/yushroom/program/graphics/FishEngine/assets/";
        const std::string skybox_dir = "/Users/yushroom/program/github/Cinder-Experiments/common/textures/";
#endif
        const std::string models_dir = root_dir + "models/";
        const std::string textures_dir = root_dir + "textures/";

        TextureImporter importer;
        auto radiance_map = importer.FromFile(skybox_dir+"BolongaRadiance.dds");
        auto irradiance_map = importer.FromFile(skybox_dir+"BolongaIrradiance.dds");
        
        auto pisa_hdr = importer.FromFile(textures_dir + "pisa.hdr");

        auto material = Material::defaultMaterial();
        material->EnableKeyword(ShaderKeyword::AmbientIBL);
        material->SetTexture("AmbientCubemap", radiance_map);
        //material->SetTexture("IrradianceMap", irradiance_map);
        
#if 1
        material = Material::builtinMaterial("SkyboxCubed");
        material->SetTexture("_Tex", radiance_map);
#else
        auto skybox_panorama = Shader::CreateFromFile(root_dir + "../Engine/Shaders/SkyBox-Panorama.shader");
        material = Material::CreateMaterial();
        material->SetShader(skybox_panorama);
        material->SetTexture("_Tex", pisa_hdr);
#endif
        material->SetVector4("_Tint", Vector4::one);
        material->SetFloat("_Exposure", 1);
        material->SetFloat("_Rotation", 0);
        RenderSettings::setSkybox(material);

#if 1
        auto group = Scene::CreateGameObject("Group");

        for (int x = 0; x <= 3; ++x)
        {
            for (int y = -5; y <= 5; y++)
            {
                auto go = GameObject::CreatePrimitive(PrimitiveType::Sphere);
                go->transform()->SetParent(group->transform());
                go->transform()->setLocalPosition(y*1.2f, x*1.2f, 0);
                go->transform()->setLocalEulerAngles(0, 30, 0);
                go->transform()->setLocalScale(0.5f, 0.5f, 0.5f);
                auto material = (x%2 == 0) ?
                    Material::builtinMaterial("PBR") :
                    Material::builtinMaterial("PBR-Reference");
                material->EnableKeyword(ShaderKeyword::AmbientIBL);
                material->SetFloat("Metallic", x >= 2 ? 1.0f : 0.0f);
                material->SetFloat("Roughness", 0.1f*(y+5));
                material->SetFloat("Specular", 0.5);
                material->SetVector3("BaseColor", Vector3(1.f, 1.f, 1.f));
                material->SetTexture("AmbientCubemap", radiance_map);
                //material->SetTexture("IrradianceMap", irradiance_map);
                go->GetComponent<MeshRenderer>()->SetMaterial(material);
            }
        }
#else
        auto albedo_map = Texture::CreateFromFile(concrete_texture_dir + "concrete_rebarconcrete_1k_alb.tga");
        auto mask_map = Texture::CreateFromFile(concrete_texture_dir + "concrete_rebarconcrete_1k_mask.tga");
        auto gloss_map = Texture::CreateFromFile(concrete_texture_dir + "concrete_rebarconcrete_1k_g.tga");
        auto normal_map = Texture::CreateFromFile(concrete_texture_dir + "concrete_rebarconcrete_1k_n.tga");

        
        material = Material::CreateMaterial();
        material->SetShader(shader);
        material->SetFloat("Specular", 0.5f);
        material->SetTexture("AlbedoMap", albedo_map);
        material->SetTexture("GlossMap", gloss_map);
        material->SetTexture("MaskMap", mask_map);
        material->SetTexture("NormalMap", normal_map);
        material->SetTexture("RadianceMap", radiance_map);
        material->SetTexture("IrradianceMap", irradiance_map);
        auto go = GameObject::CreatePrimitive(PrimitiveType::Plane);
        go->GetComponent<MeshRenderer>()->SetMaterial(material);

        //ModelImporter importer;
        //importer.setImportNormals(ModelImporterNormals::Calculate);
        //auto mitsuba = importer.LoadFromFile(models_dir + "mitsuba-sphere.obj");
        //mitsuba->CreateGameObject()->GetComponent<MeshRenderer>()->SetMaterial(material);
#endif
    }
};

#if 0

class TestCSM : public App
{
    virtual void Init() override
    {
        DefaultScene();
#if FISHENGINE_PLATFORM_WINDOWS
        const std::string root_dir = R"(D:\program\FishEngine\Example\CascadedShadowMapping\)";
#else
        const std::string root_dir = "/Users/yushroom/program/graphics/FishEngine/assets/";
#endif
        ModelImporter importer;
        auto model = importer.LoadFromFile(root_dir + "Terrain.obj");
        auto terrainGO = model->CreateGameObject();
        auto material = Material::builtinMaterial("Diffuse");
        
        TextureImporter texture_importer;
        auto bakedAO = texture_importer.FromFile(root_dir + "bakedAO.jpg");
        material->setMainTexture(bakedAO);
        terrainGO->GetComponent<MeshRenderer>()->SetMaterial(material);
    }
};

class SimpleTest : public App
{
public:
    virtual void Init() override
    {
        DefaultScene();

#if FISHENGINE_PLATFORM_WINDOWS
        const std::string root_dir = "D:/program/FishEngine/Assets/";
#else
        const std::string root_dir = "/Users/yushroom/program/graphics/FishEngine/assets/";
#endif
        const std::string models_dir = root_dir + "models/";
        const std::string textures_dir = root_dir + "textures/";
        
        TextureImporter texture_importer;
        auto checkboardTexture = texture_importer.FromFile(textures_dir + "checkboard.png");

        auto checkboardMaterial = Material::builtinMaterial("Diffuse");
        checkboardMaterial->setMainTexture(checkboardTexture);

        auto ground = GameObject::CreatePrimitive(PrimitiveType::Cube);
        ground->transform()->setLocalScale(20, 1, 20);
        ground->GetComponent<MeshRenderer>()->SetMaterial(checkboardMaterial);
        //ground->AddComponent<TestGizmos>();

        {
            auto box = GameObject::CreatePrimitive(PrimitiveType::Cube);
            box->transform()->setLocalPosition(-0.47f, 1, -5.16f);
            box->transform()->setLocalEulerAngles(0, -50, 0);
            box->GetComponent<MeshRenderer>()->SetMaterial(checkboardMaterial);
        }
        {
            auto box = GameObject::CreatePrimitive(PrimitiveType::Cube);
            box->transform()->setLocalPosition(2.62f, 1, 0.66f);
            box->transform()->setLocalEulerAngles(0, 45.314f, 0);
            box->GetComponent<MeshRenderer>()->SetMaterial(checkboardMaterial);
        }
        {
            auto box = GameObject::CreatePrimitive(PrimitiveType::Cube);
            box->transform()->setLocalPosition(-6.9f, 1, -0.62f);
            box->transform()->setLocalEulerAngles(0, 19.311f, 0);
            box->GetComponent<MeshRenderer>()->SetMaterial(checkboardMaterial);
        }
        {
            auto box = GameObject::CreatePrimitive(PrimitiveType::Cube);
            box->transform()->setLocalPosition(-2.97f, 1, -2.25f);
            box->transform()->setLocalEulerAngles(0, -99, 0);
            box->GetComponent<MeshRenderer>()->SetMaterial(checkboardMaterial);
        }
        {
            auto box = GameObject::CreatePrimitive(PrimitiveType::Cube);
            box->transform()->setLocalPosition(-0.77f, 1, 4.77f);
            box->transform()->setLocalEulerAngles(0, -35.547f, 0);
            box->GetComponent<MeshRenderer>()->SetMaterial(checkboardMaterial);
        }
        auto cameraGO = Camera::mainGameCamera()->gameObject();
        //cameraGO->AddComponent<Serialize>();
    }
};


class TestAnimation : public App
{
public:
    virtual void Init() override {
        
        DefaultScene();
        
        glCheckError();
#if FISHENGINE_PLATFORM_WINDOWS
        const std::string root_dir = R"(D:\program\FishEngine\assets\)";
#else
        const std::string root_dir = "/Users/yushroom/program/graphics/FishEngine/Assets/";
#endif
        const std::string models_dir = root_dir + "models/";
        const std::string textures_dir = root_dir + "textures/";
        //const std::string chan_dir = models_dir + "UnityChan/";
        const std::string chan_root_dir = root_dir + "../Example/UnityChan/assets/";
        
        auto sphere = Model::builtinModel(PrimitiveType::Sphere)->mainMesh();
        
        ModelImporter importer;
        importer.setFileScale(0.01f);
        //importer.setImportNormals(ModelImporterNormals::Calculate);
        //auto model = importer.LoadFBX(chan_root_dir + "models/unitychan.fbx");
        auto model = importer.LoadFromFile(chan_root_dir + "models/unitychan.fbx");
        //auto model = importer.LoadFromFile(chan_root_dir + "animations/boblampclean.md5mesh");
        
        
        ModelImporter importer2;
        //importer2.setImportNormals(ModelImporterNormals::Calculate);
        importer2.setFileScale(0.01f);
        //auto jump00Model = importer2.LoadFromFile(chan_roo t_dir + "animations/boblampclean.md5mesh");
        auto jump00Model = importer2.LoadFromFile(chan_root_dir + "animations/unitychan_RUN00_F.fbx");
        
        TextureImporter texture_importer;
        auto sky_texture = texture_importer.FromFile(textures_dir + "StPeters/DiffuseMap.dds");
        auto checkboard_texture = texture_importer.FromFile(textures_dir + "checkboard.png");
        std::string chan_texture_dir = chan_root_dir + "textures/";
        auto bodyTexture = texture_importer.FromFile(chan_texture_dir + "body_01.tga");
        auto skinTexture = texture_importer.FromFile(chan_texture_dir + "skin_01.tga");
        auto hairTexture = texture_importer.FromFile(chan_texture_dir + "hair_01.tga");
        auto faceTexture = texture_importer.FromFile(chan_texture_dir + "face_00.tga");
        auto eyelineTexture = texture_importer.FromFile(chan_texture_dir + "eyeline_00.tga");
        auto eyeirisLTexture = texture_importer.FromFile(chan_texture_dir + "eye_iris_L_00.tga");
        auto eyeirisRTexture = texture_importer.FromFile(chan_texture_dir + "eye_iris_R_00.tga");
        auto cheekTexture = texture_importer.FromFile(chan_texture_dir + "cheek_00.tga");
        
        //auto bodyTexture = Texture::CreateFromFile(chan_texture_dir + "cheek_00.tga");
        auto rolloffTexture = texture_importer.FromFile(chan_texture_dir + "FO_CLOTH1.tga");
        auto rimLightTexture = texture_importer.FromFile(chan_texture_dir + "FO_RIM1.tga");
        auto specularTexture = texture_importer.FromFile(chan_texture_dir + "body_01_SPEC.tga");
        auto envTexture = texture_importer.FromFile(chan_texture_dir + "ENV2.tga");
        auto normalMapTexture = texture_importer.FromFile(chan_texture_dir + "body_01_NRM.tga");
        
        auto stageBaseTexture = texture_importer.FromFile(chan_texture_dir + "unitychan_tile5.png");
        auto stageMaskTexture = texture_importer.FromFile(chan_texture_dir + "AlphaMask.png");

#if 0
        auto alphaMaskShader = make_shared<Shader>();
        alphaMaskShader->FromFile(chan_root_dir+"shaders/AlphaMask.vert", chan_root_dir+"shaders/AlphaMask.frag");
        auto stageMaterial = Material::CreateMaterial();
        stageMaterial->SetShader(alphaMaskShader);
        stageMaterial->SetTexture("_MainTex", stageBaseTexture);
        stageMaterial->SetTexture("_AlphaMask", stageMaskTexture);
#else
        auto stageMaterial = Material::builtinMaterial("Diffuse");
        stageMaterial->setMainTexture(stageBaseTexture);
#endif

        //GameObject::CreatePrimitive(BuiltinModelType::Cube)->CreateGameObject();
        
        auto chanMainShader = Shader::CreateFromFile(chan_root_dir+"shaders/CharaMain.shader");
        auto bodyMaterial = Material::CreateMaterial();
        bodyMaterial->setName("body");
        bodyMaterial->SetShader(chanMainShader);
        bodyMaterial->SetVector4("_Color", Vector4(1, 1, 1, 1));
        bodyMaterial->SetFloat("_SpecularPower", 20.f);
        bodyMaterial->SetTexture("_MainTex", bodyTexture);
        bodyMaterial->SetTexture("_FalloffSampler", rolloffTexture);
        bodyMaterial->SetTexture("_RimLightSampler", rimLightTexture);
        bodyMaterial->SetTexture("_SpecularReflectionSampler", specularTexture);
        bodyMaterial->SetTexture("_EnvMapSampler", envTexture);
        bodyMaterial->SetTexture("_NormalMapSampler", normalMapTexture);
        
        
        map<string, TexturePtr> textures;
        //textures["skyTex"] = sky_texture;
        
//        auto skyboxGO = Scene::CreateGameObject("SkyBox");
//        skyboxGO->transform()->setLocalScale(100, 100, 100);
//        auto meshFilter = make_shared<MeshFilter>(sphere);
//        auto material = Material::builtinMaterial("SkyboxProcedural");
//        material->SetFloat("_AtmosphereThickness", 1.0);
//        material->SetFloat("_SunDisk", 2);
//        material->SetFloat("_SunSize", 0.04f);
//        material->SetVector4("_SkyTint", Vector4(0.5f, 0.5f, 0.5f, 1));
//        material->SetVector4("_GroundColor", Vector4(.369f, .349f, .341f, 1));
//        material->SetFloat("_Exposure", 1.3f);
//        //material->SetTexture("_Tex", sky_texture);
//        //material->SetFloat("_Exposure", 1.3);
//        //material->SetFloat("_Rotation", 0);
//        //material->SetVector4("_Tint", Vector4(0.5f, 0.5f, 0.5f, 0.5f));
//        auto meshRenderer = make_shared<MeshRenderer>(material);
//        skyboxGO->AddComponent(meshFilter);
//        skyboxGO->AddComponent(meshRenderer);
        //auto skyboxGO = GameObject::CreatePrimitive(PrimitiveType::Sphere);
        //skyboxGO->setName("Skybox");
        //skyboxGO->transform()->setLocalScale(100, 100, 100);
        //auto material = Material::builtinMaterial("SkyboxProcedural");
        //material->SetFloat("_AtmosphereThickness", 1.0);
        //material->SetFloat("_SunDisk", 2);
        //material->SetFloat("_SunSize", 0.04f);
        //material->SetVector4("_SkyTint", Vector4(0.5f, 0.5f, 0.5f, 1));
        //material->SetVector4("_GroundColor", Vector4(.369f, .349f, .341f, 1));
        //material->SetFloat("_Exposure", 1.3f);
        //skyboxGO->GetComponent<MeshRenderer>()->SetMaterial(material);


        textures["AmbientCubemap"] = sky_texture;

        GameObjectPtr go;
        
        go = GameObject::CreatePrimitive(PrimitiveType::Plane);
        go->GetComponent<MeshRenderer>()->SetMaterial(stageMaterial);
        auto boxCollider = make_shared<BoxCollider>(Vector3::zero, Vector3(10, 0.01f, 10));
        go->AddComponent(boxCollider);
        //boxCollider->physicsShape();
        
        auto modelGO = model->CreateGameObject();
        //modelGO->AddComponent<TestGizmos>();
        auto capsuleCollider = make_shared<CapsuleCollider>(Vector3(0, 0.8f, 0), 1.6f, 0.25f);
        modelGO->AddComponent(capsuleCollider);
        modelGO->AddComponent<Rigidbody>();
        //capsuleCollider->physicsShape();
        
#if 1
        modelGO->AddComponent<Animator>();
        auto animator = modelGO->GetComponent<Animator>();
        animator->setAvatar(model->avatar());
        animator->m_animation = jump00Model->mainAnimation();
#endif
        
#if 0
        constexpr float edgeThickness = 0.5f;
        auto material = Material::builtinMaterial("TextureDoubleSided");
        //material = bodyMaterial;
        //material = Material::builtinMaterial("SkinnedMesh");
        material->SetTexture("_MainTex", bodyTexture);
        auto outline_material = Material::builtinMaterial("Outline");
        outline_material->setName("Outline");
        outline_material->SetVector4("_Color", Vector4(1, 1, 1, 1));
        outline_material->setMainTexture(bodyTexture);
        outline_material->SetFloat("_EdgeThickness", edgeThickness);

        textures["_MainTex"] = bodyTexture;
        material->BindTextures(textures);
        material->setName("body");
        //material->BindTextures("_MainTex", )
        for (auto name : {"hairband", "button", "Leg", "Shirts", "shirts_sode", "shirts_sode_BK", "uwagi", "uwagi_BK", "hair_accce"}) {
            auto child = FindNamedChild(modelGO, name);
            assert(child != nullptr);
            auto renderer = child->GetComponent<SkinnedMeshRenderer>();
            //renderer->setAvatar(jump00Model->avatar());
            //renderer->setRootBone(modelGO->transform());
            renderer->SetMaterial(bodyMaterial);
            renderer->AddMaterial(outline_material);
        }

        material = Material::builtinMaterial("Texture");
        material->SetTexture("_MainTex", skinTexture);
        material->setName("skin");
        for (auto name : {"skin"}) {
            auto child = FindNamedChild(modelGO, name);
            assert(child != nullptr);
            child->GetComponent<SkinnedMeshRenderer>()->SetMaterial(material);
            child->GetComponent<SkinnedMeshRenderer>()->AddMaterial(outline_material);
        }

        material = Material::builtinMaterial("Texture");
        material->setName("face");
        material->SetTexture("_MainTex", faceTexture);
        for (auto name : {"MTH_DEF", "EYE_DEF", "head_back"}) {
            auto child = FindNamedChild(modelGO, name);
            assert(child != nullptr);
            child->GetComponent<MeshRenderer>()->SetMaterial(material);
            child->GetComponent<MeshRenderer>()->AddMaterial(outline_material);
        }
        
        material = Material::builtinMaterial("Texture");
        material->setName("hair");
        material->SetTexture("_MainTex", hairTexture);
        for (auto name : {"hair_front", "hair_frontside", "tail", "tail_bottom"}) {
            auto child = FindNamedChild(modelGO, name);
            assert(child != nullptr);
            child->GetComponent<SkinnedMeshRenderer>()->SetMaterial(material);
            child->GetComponent<SkinnedMeshRenderer>()->AddMaterial(outline_material);
        }

        material = Material::builtinMaterial("Transparent");
        material->setName("eye_L1");
        material->SetTexture("_MainTex", eyeirisLTexture);
        for (auto name : {"eye_L_old"}) {
            auto child = FindNamedChild(modelGO, name);
            assert(child != nullptr);
            child->GetComponent<MeshRenderer>()->SetMaterial(material);
            //child->SetActive(false);
        }
        
        material = Material::builtinMaterial("Transparent");
        material->setName("eye_R1");
        material->SetTexture("_MainTex", eyeirisRTexture);
        for (auto name : {"eye_R_old"}) {
            auto child = FindNamedChild(modelGO, name);
            assert(child != nullptr);
            child->GetComponent<MeshRenderer>()->SetMaterial(material);
        }
        
        material = Material::builtinMaterial("Transparent");
        material->SetTexture("_MainTex", eyelineTexture);
        for (auto name : {"BLW_DEF", "EL_DEF"}) {
            auto child = FindNamedChild(modelGO, name);
            assert(child != nullptr);
            child->GetComponent<MeshRenderer>()->SetMaterial(material);
        }
        
        material = Material::builtinMaterial("Texture");
        material->setName("eyeline");
        material->SetTexture("_MainTex", eyelineTexture);
        for (auto name : {"eye_base_old"}) {
            auto child = FindNamedChild(modelGO, name);
            assert(child != nullptr);
            child->GetComponent<MeshRenderer>()->SetMaterial(material);
            //Selection::setActiveGameObject(child);
        }

        material = Material::builtinMaterial("Transparent");
        material->setName("mat_cheek");
        material->SetTexture("_MainTex", cheekTexture);
        for (auto name : {"cheek"}) {
            auto child = FindNamedChild(modelGO, name);
            assert(child != nullptr);
            child->GetComponent<SkinnedMeshRenderer>()->SetMaterial(material);
        }
        
#endif
        // Camera
        
        auto cameraGO = Camera::mainGameCamera()->gameObject();
        cameraGO->transform()->setLocalPosition(0, 0.8f, -2.6f);
        cameraGO->transform()->setLocalEulerAngles(0, 0, 0);
        cameraGO->AddComponent<ShowFPS>();
        cameraGO->AddComponent<TakeScreenShot>();
        auto s = cameraGO->AddComponent<EditorRenderSettings>();
        s->m_useGammaCorrection = false;
        //cameraGO->GetComponent<EditorRenderSettings>()->m_useGammaCorrection = false;

        Selection::setActiveGameObject(cameraGO);

        go = Scene::CreateGameObject("Test");
        go->AddComponent<TestExecutionOrder>();;
    }
};


class TestPhysics : public App
{
public:
    virtual void Init() override
    {
        DefaultScene();

#if FISHENGINE_PLATFORM_WINDOWS
        const std::string root_dir = "D:/program/FishEngine/Assets/";
#else
        const std::string root_dir = "/Users/yushroom/program/graphics/FishEngine/assets/";
#endif
        const std::string models_dir = root_dir + "Models/";
        const std::string textures_dir = root_dir + "Textures/";
        
        TextureImporter texture_importer;
        auto checkboard_texture = texture_importer.FromFile(textures_dir + "checkboard.png");
        
        auto material = Material::builtinMaterial("Diffuse");
        material->setMainTexture(checkboard_texture);

        auto go = GameObject::CreatePrimitive(PrimitiveType::Cube);
        //go->transform()->setLocalEulerAngles(0, 0, 10);
        go->transform()->setPosition(0, -1.0f-0.1f*0.5f, 0);
        go->transform()->setLocalScale(10, 0.1f, 10);
        auto boxCollider = make_shared<BoxCollider>(Vector3::zero, Vector3::one);
        go->AddComponent(boxCollider);
        auto renderer = go->GetComponent<MeshRenderer>();
        renderer->SetMaterial(material);
        
        auto sphereGO = GameObject::CreatePrimitive(PrimitiveType::Sphere);
        sphereGO->transform()->setPosition(0, 0, 0);
        auto sphereCollider = make_shared<SphereCollider>(Vector3::zero, 0.5f);
        sphereGO->AddComponent(sphereCollider);
        sphereGO->AddComponent<Rigidbody>();
        
        auto cameraGO = Camera::mainGameCamera()->gameObject();
        cameraGO->transform()->setLocalPosition(0, 2, -10);
        cameraGO->transform()->setLocalEulerAngles(0, 0, 0);
        auto s = cameraGO->AddComponent<EditorRenderSettings>();
        s->m_useGammaCorrection = false;
    }
};


class Shadertoy : public App
{
public:
    virtual void Init() override
    {
        DefaultScene();

#if FISHENGINE_PLATFORM_WINDOWS
        const std::string root_dir = "../../assets/";
#else
        const std::string root_dir = "/Users/yushroom/program/graphics/FishEngine/assets/";
#endif
        const std::string models_dir = root_dir + "models/";
        const std::string textures_dir = root_dir + "textures/";
        //auto checkboardTexture = Texture::CreateFromFile(textures_dir + "checkboard.png");

        auto planeGO = GameObject::CreatePrimitive(PrimitiveType::Quad);
        
        auto material = Material::CreateMaterial();
        auto shader = Shader::CreateFromFile(root_dir + "../example/Shadertoy/assets/shaders/Sea.vsfs");
        material->SetShader(shader);
        //material->SetTexture("screenTexture", checkboardTexture);
        planeGO->GetComponent<MeshRenderer>()->SetMaterial(material);
    }
};

class TestSerialization : public App
{
public:
    virtual void Init() override
    {
        DefaultScene();

        auto cameraGO = Camera::mainGameCamera()->gameObject();
        //cameraGO->AddComponent<Serialize>();
        std::stringstream ss;
        {
            cereal::JSONOutputArchive oa(ss);
            oa << *cameraGO;
        }
        {
            cereal::JSONInputArchive ia(ss);
            GameObject go("lll");
            ia >> go;
            cereal::JSONOutputArchive oa(ss);
            oa << *cameraGO;
        }
        auto xml = ss.str();
        cout << xml << endl;
    }
};

class TestParallaxMap : public App
{
public:
    virtual void Init() override
    {
        DefaultScene();
        auto quadGO = GameObject::CreatePrimitive(PrimitiveType::Quad);
        quadGO->transform()->setLocalScale(4);
        quadGO->transform()->setLocalPosition(0, 1, 0);
        auto shader = Shader::CreateFromFile(Resources::shaderRootDirectory() + "ParallaxMap.vsfs");
        auto material = Material::CreateMaterial();
        material->SetShader(shader);
        auto current_path = boost::filesystem::current_path();
        cout << current_path << endl;
#if FISHENGINE_PLATFORM_WINDOWS
        const std::string sample_root_dir = "../../Example/ParallaxMapping/";
#else
        const std::string sample_root_dir = "/Users/yushroom/program/graphics/FishEngine/Example/ParallaxMapping/";
#endif
        const std::string texture_dir = sample_root_dir + "textures/";

        TextureImporter texture_importer;
        auto texture_bricks2_disp = texture_importer.FromFile(texture_dir + "bricks2_disp.jpg");
        auto texture_bricks2_normal = texture_importer.FromFile(texture_dir + "bricks2_normal.jpg");
        auto texture_bricks2 = texture_importer.FromFile(texture_dir + "bricks2.jpg");

        material->SetTexture("diffuseMap", texture_bricks2);
        material->SetTexture("normalMap", texture_bricks2_normal);
        material->SetTexture("depthMap", texture_bricks2_disp);
        material->SetFloat("heightScale", 0.1f);

        quadGO->GetComponent<MeshRenderer>()->SetMaterial(material);

        auto cubeGO = GameObject::CreatePrimitive(PrimitiveType::Cube);
    }
};


class CharacterThirdPerson : public App
{
public:
    virtual void Init() override
    {
        DefaultScene();
        cout << "current working dir: " << boost::filesystem::current_path() << endl;
#if FISHENGINE_PLATFORM_WINDOWS
        const std::string sample_root_dir = "D:/program/FishEngine/Example/CharacterThirdPerson/";
#else
        const std::string sample_root_dir = "/Users/yushroom/program/graphics/FishEngine/Example/CharacterThirdPerson/";
#endif
        ModelImporter importer;
        importer.setFileScale(0.01f);
        auto model = importer.LoadFromFile(sample_root_dir + "Models/Ethan.fbx");
        auto modelGO = model->CreateGameObject();
    }
};

#endif
 
int main()
{
    FishEditorWindow::AddApp(make_shared<TestPBR>());
    //FishEditorWindow::AddApp(make_shared<TestCSM>());
    //FishEditorWindow::AddApp(make_shared<TestAnimation>());
    //FishEditorWindow::AddApp(make_shared<Shadertoy>());
    //FishEditorWindow::AddApp(make_shared<TestPhysics>());
    //FishEditorWindow::AddApp(make_shared<SimpleTest>());
    //FishEditorWindow::AddApp(make_shared<TestSerialization>());
    //FishEditorWindow::AddApp(make_shared<TestParallaxMap>());
    //FishEditorWindow::AddApp(make_shared<CharacterThirdPerson>());
    FishEditorWindow::Init();
    //test();
    //shared_ptr<Object> p = make_shared<Camera>();
    //Debug::LogWarning("%s", typeid(p).name());
    //Debug::LogWarning("%s", typeid(Camera).name());
    FishEditorWindow::Run();
    FishEditorWindow::Clean();
    return 0;
}
