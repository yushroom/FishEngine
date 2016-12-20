#include <iostream>
#include <future>

#include "FishEngine.hpp"
#include "GameApp.hpp"
#include "GameObject.hpp"
#include "RenderSystem.hpp"
#include "RenderSettings.hpp"
#include "Scene.hpp"
#include "Shader.hpp"
#include "Material.hpp"
#include "Camera.hpp"
#include "Resources.hpp"
#include "QualitySettings.hpp"
#include "Light.hpp"
#include "CameraController.hpp"
#include "ModelImporter.hpp"
#include "TextureImporter.hpp"
#include "MeshRenderer.hpp"
#include "Timer.hpp"

using namespace FishEngine;
using namespace std;

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

    auto material = Material::InstantiateBuiltinMaterial("SkyboxProcedural");
    material->SetFloat("_AtmosphereThickness", 1.0);
    //material->SetFloat("_SunDisk", 2);
    material->SetFloat("_SunSize", 0.04f);
    material->SetVector4("_SkyTint", Vector4(0.5f, 0.5f, 0.5f, 1));
    material->SetVector4("_GroundColor", Vector4(.369f, .349f, .341f, 1));
    material->SetFloat("_Exposure", 1.3f);
    RenderSettings::setSkybox(material);
}

GameObjectPtr FindNamedChild(const GameObjectPtr & root, const std::string& name)
{
    auto& children = root->transform()->children();
    for (auto& c : children)
    {
        const auto& g = c.lock();
        //Debug::Log("Name: %s", g->name().c_str());
        if (g->name() == name)
        {
            return g->gameObject();
        }
        auto r = FindNamedChild(g->gameObject(), name);
        if (r != nullptr)
        {
            return r;
        }
    }
    return nullptr;
}


class TestDeferredRenderingApp : public GameApp
{
public:
    virtual void Init() override
    {
        Timer timer("load");
        QualitySettings::setShadowDistance(30);
        Path sponza_root = Resources::exampleRootDirectory() / "Sponza";
        Path sponza_assets_root = sponza_root / "crytek-sponza";
        Resources::SetAssetsDirectory(sponza_root);
        ModelImporter importer;
        importer.setFileScale(0.01f);

        auto handle = std::async(std::launch::async, 
            [&importer, &sponza_assets_root]() { return importer.LoadFromFile(sponza_assets_root / "sponza.obj"); }
        );

        TextureImporter tex_importer;
        Path textures_root = sponza_assets_root / "textures";
        auto shader1 = Shader::CreateFromFile(sponza_root / "diffuse_mask_twosided.shader");

        DefaultScene();

        auto transform = Camera::main()->gameObject()->transform();
        transform->setPosition(5, 8, 0);
        transform->setLocalEulerAngles(30, -90, 0);

        transform = Camera::mainGameCamera()->gameObject()->transform();
        transform->setPosition(5, 8, 0);
        transform->setLocalEulerAngles(30, -90, 0);

        //auto sponza_model = importer.LoadFromFile(sponza_assets_root / "sponza.obj");
        auto sponza_model = handle.get();
        auto sponza_go = sponza_model->CreateGameObject();

        timer.StopAndPrint();

#define LOAD_COMPRESSED_DIFFUSE_MAP 1

        auto ApplyMateril1 = [&sponza_go, &tex_importer, &shader1, &textures_root]
        (const char* go_name, const std::string& diffuse_tex, const std::string& mask_tex)
        {
            auto mesh0 = FindNamedChild(sponza_go, go_name);
            auto mtl = mesh0->GetComponent<MeshRenderer>()->material();
            mtl->SetShader(shader1);
#if LOAD_COMPRESSED_DIFFUSE_MAP
            auto diffuse = tex_importer.FromFile(textures_root / (diffuse_tex + "_bc1.dds"));
#else
            auto diffuse = tex_importer.FromFile(textures_root / (diffuse_tex + ".png"));
#endif
            auto mask = tex_importer.FromFile(textures_root / (mask_tex + ".png"));
            mtl->SetTexture("DiffuseTex", diffuse);
            mtl->SetTexture("MaskTex", mask);
        };

        ApplyMateril1("mesh0", "sponza_thorn_diff", "sponza_thorn_mask");
        ApplyMateril1("mesh1", "vase_plant", "vase_plant_mask");
        ApplyMateril1("mesh20", "chain_texture", "chain_texture_mask");

        auto shader2 = Shader::CreateFromFile(sponza_root / "diffuse_bump.shader");
        auto ApplyMateril2 = [&sponza_go, &tex_importer, &shader2, &textures_root]
        (const char* go_name, const std::string& diffuse_tex)
        {
            auto mesh0 = FindNamedChild(sponza_go, go_name);
            auto mtl = mesh0->GetComponent<MeshRenderer>()->material();
            mtl->SetShader(shader2);
#if LOAD_COMPRESSED_DIFFUSE_MAP
            auto diffuse = tex_importer.FromFile(textures_root / (diffuse_tex + "_bc1.dds"));
#else
            auto diffuse = tex_importer.FromFile(textures_root / (diffuse_tex + ".png"));
#endif
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
    }

    virtual void Update() override
    {

    }
};


int main()
{
    TestDeferredRenderingApp app;
    return app.Run();
}
