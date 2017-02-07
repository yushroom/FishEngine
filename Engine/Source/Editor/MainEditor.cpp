#include "MainEditor.hpp"

#include <QDir>
#include <QCoreApplication>

#include <Debug.hpp>
#include <Camera.hpp>
#include <GameObject.hpp>
#include <CameraController.hpp>
#include <ModelImporter.hpp>
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

#include "SceneViewEditor.hpp"
#include "Selection.hpp"
#include "EditorGUI.hpp"
#include "SceneViewEditor.hpp"
#include "Inspector.hpp"

#include <boost/filesystem.hpp>

#include <iostream>

//#include <Serialization/archives/binary.hpp>
#include <Serialization.hpp>
#include <Serialization/archives/yaml.hpp>

using namespace FishEngine;
using namespace std;

namespace FishEditor
{


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

#if 1

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
        RenderSystem::Init();

        m_mainSceneViewEditor->Init();
        DefaultScene();

        glClearColor(1.0f, 0.0f, 0.0f, 1);
        OnInitialized();
    }

    void MainEditor::Run()
    {
        auto go = Selection::activeGameObject();
        Inspector::Bind(go);

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
        auto quad = Model::builtinMesh(PrimitiveType::Quad);
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
}
