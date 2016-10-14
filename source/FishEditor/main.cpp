#include <iostream>
#include <map>
#include <sstream>

#include "FishEditorWindow.hpp"

using namespace FishEditor;

#include "GameObject.hpp"
#include "RenderSystem.hpp"
#include "Debug.hpp"

#include "App.hpp"
#include "Input.hpp"
#include "EditorGUI.hpp"
#include "Camera.hpp"
#include "Time.hpp"
#include "Mesh.hpp"
#include "MeshFilter.hpp"
#include <MeshRenderer.hpp>
#include <SkinnedMeshRenderer.hpp>
#include "RenderSettings.hpp"
#include "Scene.hpp"
#include "Selection.hpp"
#include "EditorRenderSystem.hpp"
#include "Light.hpp"
#include <ModelImporter.hpp>
#include <CameraController.hpp>
#include <Bounds.hpp>
#include <ModelImporter.hpp>
#include <Gizmos.hpp>
#include <BoxCollider.hpp>
#include <SphereCollider.hpp>
#include <Rigidbody.hpp>
//#include <boost/type_traits.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <Serialization.hpp>
#include <CapsuleCollider.hpp>

using namespace std;
using namespace FishEngine;
using namespace FishEditor;

class ShowFPS : public Script
{
public:
    InjectClassName(ShowFPS);

    int m_fps = 0;
    
    virtual void OnInspectorGUI() override {
        m_fps = (int)floor(1.f / Time::deltaTime() + 0.5f);
        ImGui::Text("FPS: %d", m_fps);
    }

    virtual void Update() override {
        if (Input::GetKeyDown(KeyCode::A)) {
            Debug::Log("A pressed");
        }
        if (Input::GetKey(KeyCode::A)) {
            Debug::Log("A held");
        }
        if (Input::GetKeyUp(KeyCode::A)) {
            Debug::Log("A released");
        }

        //if (Input::GetMouseButtonDown(0)) {
        //    auto mp = Input::mousePosition();
        //    Debug::Log("Mouse Position: %lf, %lf, %lf", mp.x, mp.y, mp.z);
        //}
    }
};

class DeactiveSelf : public Script
{
public:
    InjectClassName(DeactiveSelf);

    bool m_active = true;
    
    virtual void OnInspectorGUI() override {
        ImGui::Checkbox("show", &m_active);
    }

    virtual void Update() override {
        if (m_active && !gameObject()->activeSelf()) {
            Debug::Log("show");
            gameObject()->SetActive(true);
        }
        if (!m_active &&  gameObject()->activeSelf()) {
            Debug::Log("hide");
            gameObject()->SetActive(false);
        }
    }
};

class VisualizeNormal : public Script
{
private:
    bool m_added = false;
    shared_ptr<MeshRenderer> m_meshRenderer = nullptr;
    Material::PMaterial m_material = nullptr;

public:
    InjectClassName(VisualizeNormal);

    bool m_visualizeNormal = true;

    virtual void Start() override {
        m_meshRenderer = gameObject()->GetComponent<MeshRenderer>();
        m_material = Material::builtinMaterial("VisualizeNormal");
    }
    
    virtual void OnInspectorGUI() override {
        ImGui::Checkbox("Visualize Normal##checkbox", &m_visualizeNormal);
    }
    
    virtual void Update() override {
        auto& materials = m_meshRenderer->materials();
        if (m_visualizeNormal) {
            if (!m_added) {
                m_meshRenderer->AddMaterial(m_material);
                m_added = true;
            }
        } else {
            if (materials[materials.size()-1] == m_material) {
                materials.pop_back();
                m_added = false;
            }
        }
    }
};

class TakeScreenShot : public Script
{
public:
    InjectClassName(TakeScreenShot);
    
    virtual void OnInspectorGUI() override {
        if (EditorGUI::Button("Screen shot")) {
            auto tm = time(nullptr);
            ostringstream ss;
            ss << "./" << int(tm) << ".png";
            EditorRenderSystem::SaveScreenShot(ss.str());
            Debug::Log("Screen shot saved to %s", ss.str().c_str());
        }
    }
};
            
class DisplayMatrix : public Script {
public:
    InjectClassName(DisplayMatrix);
    
    Matrix4x4 localToWorld;
    Matrix4x4 worldToLocal;
    // Use this for initialization
    
    virtual void OnInspectorGUI() override {
        EditorGUI::Matrix4x4("localToWorld", localToWorld);
        EditorGUI::Matrix4x4("worldToLocal", worldToLocal);
    }
    
    virtual void Start () override {
        localToWorld = transform()->localToWorldMatrix();
        worldToLocal = transform()->worldToLocalMatrix();
    }
};

class EditorRenderSettings : public Script
{
public:
    InjectClassName(EditorRenderSettings);

    bool m_isWireFrameMode = false;
    bool m_useGammaCorrection = true;
    bool m_showShadowMap = false;
    bool m_highlightSelections = true;
    
    virtual void Start() override {
        EditorRenderSystem::setWireFrameMode(m_isWireFrameMode);
        EditorRenderSystem::setGammaCorrection(m_useGammaCorrection);
        EditorRenderSystem::setShowShadowMap(m_showShadowMap);
        EditorRenderSystem::setHightlightSelections(m_highlightSelections);
    }

    virtual void OnInspectorGUI() override {
        if (ImGui::Checkbox("Wire Frame", &m_isWireFrameMode)) {
            EditorRenderSystem::setWireFrameMode(m_isWireFrameMode);
        }
        if (ImGui::Checkbox("Gamma Correction", &m_useGammaCorrection)) {
            EditorRenderSystem::setGammaCorrection(m_useGammaCorrection);
        }
        if (ImGui::Checkbox("Show ShadowMap", &m_showShadowMap)) {
            EditorRenderSystem::setShowShadowMap(m_showShadowMap);
        }
        if (ImGui::Checkbox("Highlight Selections", &m_highlightSelections)) {
            EditorRenderSystem::setHightlightSelections(m_highlightSelections);
        }
    }
};
            
class Rotator : public Script {
public:
    InjectClassName(Rotator);
    
    bool rotating = false;
    float step = 1;
    
    virtual void Update() override {
        if (rotating)
            transform()->RotateAround(Vector3(0, 0, 0), Vector3::up, step);
    }
    
    virtual void OnInspectorGUI() override {
        ImGui::Checkbox("Rotating", &rotating);
    }
};
            
            
class DrawSkeleton : public Script {
public:
    InjectClassName(DrawSkeleton);
    
    bool draw = true;
    
    virtual void OnDrawGizmos() override {
        if (transform()->parent() == nullptr) {
            return;
        }
        Gizmos::DrawLine(transform()->parent()->position(), transform()->position());
    }

    virtual void Update() override {
    }
    
    virtual void OnInspectorGUI() override {
        ImGui::Checkbox("Draw", &draw);
    }
};

class TestAABB : public Script
{
public:
    InjectClassName(TestAABB);

    Bounds aabb{Vector3(0, 0, 0), Vector3(1, 1, 1)};
    //GameObject::PGameObject target;

    virtual void Start() override
    {

    }

    virtual void Update() override
    {
//        auto center = transform()->position();
//        auto cam_pos = Camera::main()->transform()->position();
//        center = Vector3::Normalize(center - cam_pos)
//        + cam_pos;
//        target->transform()->setPosition(center);
        
        if (Input::GetMouseButtonDown(0)) {
            //auto mp = Input::mousePosition();
            //Debug::Log("Mouse Post ion: %lf, %lf, %lf", mp.x, mp.y, mp.z);
            auto ray = Camera::main()->ScreenPointToRay(Input::mousePosition());
            //Debug::Log("Ray dir: %lf, %lf, %lf", ray.direction.x, ray.direction.y, ray.direction.z);
            if (aabb.IntersectRay(ray)) {
                Debug::Log("hit aabb");
            }
        }
    }
};

            
class TestGizmos : public Script
{
public:
    InjectClassName(TestGizmos);
    
    virtual void OnDrawGizmos() override {
        Gizmos::setColor(Color::red);
        Gizmos::DrawWireSphere(Vector3(1, 1, 1), 1.f);
        
        Gizmos::setColor(Color::blue);
        Gizmos::DrawLine(Vector3(1, 1, 1), Vector3(2, 1, 1));
        
        Gizmos::setColor(Color::green);
        Gizmos::DrawWireCube(Vector3(0, 1, 1), Vector3(1, 2, 3));
        
        Gizmos::setColor(Color::black);
        Gizmos::DrawWireCapsule(Vector3::zero, 0.5f, 2.f);
    }
};

void DefaultScene()
{
    auto camera = Camera::Create(60.0f, Screen::aspect(), 0.3f, 1000.f);
    auto camera_go = Scene::CreateGameObject("Main Camera");
    camera_go->AddComponent(camera);
    camera_go->AddComponent<CameraController>();
    camera_go->transform()->setLocalPosition(0, 0, 5);
    camera_go->transform()->setLocalPosition(0, 1, -10);
    //camera_go->transform()->LookAt(0, 0, 0);
    camera_go->SetTag("MainCamera");
    
    auto light_go = Scene::CreateGameObject("Directional Light");
    light_go->transform()->setPosition(0, 3, 0);
    light_go->transform()->setLocalEulerAngles(50, -30, 0);
    //auto euler = light_go->transform()->localEulerAngles();
    light_go->AddComponent(Light::Create());
    light_go->AddComponent<Rotator>();
}
            
//class TestPBR : public App
//{
//public:
//
//    virtual void Init() override {
//        glCheckError();
//#if FISHENGINE_PLATFORM_WINDOWS
//        const std::string root_dir = "../../assets/";
//#else
//        const std::string root_dir = "/Users/yushroom/program/graphics/FishEngine/assets/";
//#endif
//        const std::string models_dir = root_dir + "models/";
//        const std::string textures_dir = root_dir + "textures/";
//        
//        //auto mitsuba = Mesh::CreateFromObjFile(models_dir + "mitsuba-sphere.obj");
//        ModelImporter importer;
//        auto mitsuba = importer.LoadFromFile(models_dir + "mitsuba-sphere.obj");
//        mitsuba->CreateGameObject();
//        //auto boblampclean = Mesh::CreateFromObjFile(models_dir + "boblampclean.md5mesh");
//
//        auto sky_texture = Texture::CreateFromFile(textures_dir + "StPeters/DiffuseMap.dds");
//        //auto sky_texture = Texture::CreateFromFile(textures_dir + "uffizi_cross_filtered_y.dds");
//        auto checkboard_texture = Texture::CreateFromFile(textures_dir + "checkboard.png");
//        //auto head_diffuse = Texture::CreateFromFile(models_dir + "head/lambertian.jpg");
//        //auto head_normalmap = Texture::CreateFromFile(models_dir + "head/NormalMap_RG16f_1024_mipmaps.dds");
//        
//        map<string, Texture::PTexture> textures;
//        textures["skyTex"] = sky_texture;
//        
//        auto skyboxGO = Scene::CreateGameObject("SkyBox");
//        skyboxGO->transform()->setLocalScale(20, 20, 20);
//        auto meshFilter = make_shared<MeshFilter>(Model::builtinModel(BuiltinModelTyep::Sphere)->mainMesh());
//        auto material = Material::builtinMaterial("SkyBox");
//        material->BindTextures(textures);
//        auto meshRenderer = make_shared<MeshRenderer>(material);
//        skyboxGO->AddComponent(meshFilter);
//        skyboxGO->AddComponent(meshRenderer);
//        
//        //textures.clear();
//        //textures["diffuseMap"] = head_diffuse;
//        //textures["normalMap"] = head_normalmap;
//
////        auto headGO = Scene::CreateGameObject();
////        headGO->transform()->setScale(10, 10, 10);
////        auto meshFilter1 = make_shared<MeshFilter>(headModel);
////        auto material1 = Material::builtinMaterial("NormalMap");
////        material1->BindTextures(textures);
////        auto meshRenderer1 = make_shared<MeshRenderer>(material1);
////        headGO->AddComponent(meshFilter1);
////        headGO->AddComponent(meshRenderer1);
////        headGO->AddScript(make_shared<VisualizeNormal>());
////        //headGO->AddScript(make_shared<DeactiveSelf>());
//        textures["AmbientCubemap"] = sky_texture;
//        
////        auto go = Scene::CreateGameObject("Sphere");
////        //go->transform()->setScale(20, 20, 20);
////        meshFilter = make_shared<MeshFilter>(mitsuba);
////        material = Material::builtinMaterial("PBR");
////        material->SetVector3("albedo", Vector3(0.972f, 0.960f, 0.915f));
////        material->BindTextures(textures);
////        meshRenderer = make_shared<MeshRenderer>(material);
////        go->AddComponent(meshFilter);
////        go->AddComponent(meshRenderer);
////        go->AddScript(make_shared<VisualizeNormal>());
//        //go->AddScript(make_shared<DisplayMatrix>());
//        //go->AddScript(make_shared<DeactiveSelf>());
//        //go->SetActive(false);
//        //Scene::SelectGameObject(go.get());
//
//        
//        auto group = Scene::CreateGameObject("Group");
//        
//        auto create_sphere = [&sphere, &textures, &group](
//                                                //std::shared_ptr<GameObject>& parent,
//                                                int x, int y) {
//            auto go = Scene::CreateGameObject("Sphere");
//            go->transform()->SetParent(group->transform());
//            go->transform()->setLocalPosition(x*1.2f, y*1.2f, 0);
//            //go->transform()->setPosition(0, 0, 2);
//            go->transform()->setLocalEulerAngles(0, 30, 0);
//            go->transform()->setLocalScale(0.5f, 0.5f, 0.5f);
//            auto meshFilter = make_shared<MeshFilter>(sphere);
//            auto material = Material::builtinMaterial("PBR");
//            material->BindTextures(textures);
//            material->SetFloat("metallic", 0.1f*x);
//            material->SetFloat("roughness", 0.1f*y);
//            material->SetVector3("albedo", Vector3(1.f, 0.6f, 0.6f));
//            auto meshRenderer = make_shared<MeshRenderer>(material);
//            go->AddComponent(meshFilter);
//            go->AddComponent(meshRenderer);
//            //go->AddScript(make_shared<DisplayMatrix>());
//            //go->transform()->SetParent(parent->transform());
//            return go;
//        };
//
//        
//        for (int x = 0; x < 11; ++x) {
//            for (int y = 0; y < 11; y++) {
//                create_sphere(x, y);
//            }
//        }
//        //create_sphere(0, 0);
//
////        auto child0 = create_cube(go);
////        auto child1 = create_cube(child0);
////        auto child2 = create_cube(child1);
//
////        go = Scene::CreateGameObject("Plane");
////        meshFilter = make_shared<MeshFilter>(plane);
////        material = Material::builtinMaterial("Diffuse");
////        textures.clear();
////        textures["diffuseMap"] = checkboard_texture;
////        material->BindTextures(textures);
////        meshRenderer = make_shared<MeshRenderer>(material);
////        go->AddComponent(meshFilter);
////        go->AddComponent(meshRenderer);
//        //go->AddScript(make_shared<VisualizeNormal>());
//        //go->AddScript(make_shared<DisplayMatrix>());
//        
//        auto cameraGO = Scene::mainCamera()->gameObject();
//        cameraGO->transform()->setPosition(6, 6, -12);
//        cameraGO->transform()->LookAt(6, 6, 0);
//        //cameraGO->transform()->setPosition(0, 0, -5);
//        //cameraGO->transform()->LookAt(0, 0, 0);
//        //cameraGO->transform()->LookAt(0, 0, 0);
//        cameraGO->AddScript(make_shared<ShowFPS>());
//        cameraGO->AddScript(make_shared<TakeScreenShot>());
//        //cameraGO->AddScript(make_shared<RenderSettings>());
//        //cameraGO->AddScript(make_shared<DisplayMatrix>());
//        cameraGO->AddScript(make_shared<EditorRenderSettings>());
//        Selection::setActiveGameObject(cameraGO);
//
//        auto go = Scene::CreateGameObject("Directional Light");
//        go->transform()->setPosition(6, 5, -10);
//        go->transform()->LookAt(6, 0, 0);
//        //go->transform()->setPosition(6, 5, -10);
//        //go->transform()->LookAt(0, 0, 0);
//        go->AddComponent(Light::Create());
//        go->AddScript(make_shared<Rotator>());
//        
//        
//        //auto child0 = Scene::CreateGameObject("child0");
//        //child0->transform()->SetParent(go->transform());
//        //auto child1 = Scene::CreateGameObject("child1");
//        //child1->transform()->SetParent(go->transform());
//        //auto child3 = Scene::CreateGameObject("child3");
//        //child3->transform()->SetParent(child0->transform());
//    }
//};

template<typename T>
void RecursivelyAddScript(const std::shared_ptr<FishEngine::Transform>& t)
{
    t->gameObject()->AddComponent<T>();
    for (auto& c : t->children()) {
        RecursivelyAddScript<T>(c.lock());
    }
}


std::shared_ptr<GameObject> FindNamedChild(const std::shared_ptr<GameObject> & root, const std::string& name)
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


class TestAnimation : public App
{
public:
    virtual void Init() override {
        
        DefaultScene();
        
        glCheckError();
#if FISHENGINE_PLATFORM_WINDOWS
        const std::string root_dir = "../../assets/";
#else
        const std::string root_dir = "/Users/yushroom/program/graphics/FishEngine/assets/";
#endif
        const std::string models_dir = root_dir + "models/";
        const std::string textures_dir = root_dir + "textures/";
        //const std::string chan_dir = models_dir + "UnityChan/";
        const std::string chan_root_dir = root_dir + "../example/UnityChan/assets/";
        
        auto sphere = Model::builtinModel(BuiltinModelType::Sphere)->mainMesh();
        
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
        auto sky_texture = Texture::CreateFromFile(textures_dir + "StPeters/DiffuseMap.dds");
        auto checkboard_texture = Texture::CreateFromFile(textures_dir + "checkboard.png");
        std::string chan_texture_dir = chan_root_dir + "textures/";
        auto bodyTexture = Texture::CreateFromFile(chan_texture_dir + "body_01.tga");
        auto skinTexture = Texture::CreateFromFile(chan_texture_dir + "skin_01.tga");
        auto hairTexture = Texture::CreateFromFile(chan_texture_dir + "hair_01.tga");
        auto faceTexture = Texture::CreateFromFile(chan_texture_dir + "face_00.tga");
        auto eyelineTexture = Texture::CreateFromFile(chan_texture_dir + "eyeline_00.tga");
        auto eyeirisLTexture = Texture::CreateFromFile(chan_texture_dir + "eye_iris_L_00.tga");
        auto eyeirisRTexture = Texture::CreateFromFile(chan_texture_dir + "eye_iris_R_00.tga");
        auto cheekTexture = Texture::CreateFromFile(chan_texture_dir + "cheek_00.tga");
        
        //auto bodyTexture = Texture::CreateFromFile(chan_texture_dir + "cheek_00.tga");
        auto rolloffTexture = Texture::CreateFromFile(chan_texture_dir + "FO_CLOTH1.tga");
        auto rimLightTexture = Texture::CreateFromFile(chan_texture_dir + "FO_RIM1.tga");
        auto specularTexture = Texture::CreateFromFile(chan_texture_dir + "body_01_SPEC.tga");
        auto envTexture = Texture::CreateFromFile(chan_texture_dir + "ENV2.tga");
        auto normalMapTexture = Texture::CreateFromFile(chan_texture_dir + "body_01_NRM.tga");
        
        auto stageBaseTexture = Texture::CreateFromFile(chan_texture_dir + "unitychan_tile5.png");
        auto stageMaskTexture = Texture::CreateFromFile(chan_texture_dir + "AlphaMask.png");

#if 0
        auto alphaMaskShader = make_shared<Shader>();
        alphaMaskShader->FromFile(chan_root_dir+"shaders/AlphaMask.vert", chan_root_dir+"shaders/AlphaMask.frag");
        auto stageMaterial = Material::CreateMaterial();
        stageMaterial->SetShader(alphaMaskShader);
        stageMaterial->SetTexture("_MainTex", stageBaseTexture);
        stageMaterial->SetTexture("_AlphaMask", stageMaskTexture);
#else
        auto stageMaterial = Material::builtinMaterial("Diffuse");
        stageMaterial->SetTexture("diffuseMap", stageBaseTexture);
#endif

        //Model::builtinModel(BuiltinModelType::Cube)->CreateGameObject();
        
        auto chanMainShader = make_shared<Shader>();
        chanMainShader->FromFile(chan_root_dir+"shaders/CharaMain.vert", chan_root_dir+"shaders/CharaMain.frag");
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
        
        
        map<string, Texture::PTexture> textures;
        //textures["skyTex"] = sky_texture;
        
        auto skyboxGO = Scene::CreateGameObject("SkyBox");
        skyboxGO->transform()->setLocalScale(20, 20, 20);
        auto meshFilter = make_shared<MeshFilter>(sphere);
        auto material = Material::builtinMaterial("SkyBox");
        material->SetTexture("skyTex", sky_texture);
        auto meshRenderer = make_shared<MeshRenderer>(material);
        skyboxGO->AddComponent(meshFilter);
        skyboxGO->AddComponent(meshRenderer);

        textures["AmbientCubemap"] = sky_texture;

        std::shared_ptr<GameObject> go;
        
        go = Model::builtinModel(BuiltinModelType::Plane)->CreateGameObject();
        go->transform()->GetChild(0)->gameObject()->GetComponent<MeshRenderer>()->SetMaterial(stageMaterial);
        auto boxCollider = make_shared<BoxCollider>(Vector3::zero, Vector3(1, 0.1f, 1));
        go->AddComponent(boxCollider);
        boxCollider->physicsShape();
        
        auto modelGO = model->CreateGameObject();
        //modelGO->AddComponent<TestGizmos>();
        auto capsuleCollider = make_shared<CapsuleCollider>(Vector3(0, 0.8f, 0), 1.6f, 0.5f);
        modelGO->AddComponent(capsuleCollider);
        modelGO->AddComponent<Rigidbody>();
        capsuleCollider->physicsShape();
        
#if 1
        modelGO->AddComponent<Animator>();
        auto animator = modelGO->GetComponent<Animator>();
        animator->setAvatar(model->avatar());
        animator->m_animation = jump00Model->mainAnimation();
#endif
        
        constexpr float edgeThickness = 0.5f;
        material = Material::builtinMaterial("TextureDoubleSided");
        //material = bodyMaterial;
        //material = Material::builtinMaterial("SkinnedMesh");
        material->SetTexture("DiffuseMap", bodyTexture);
        auto outline_material = Material::builtinMaterial("Outline");
        outline_material->setName("Outline");
        outline_material->SetVector4("_Color", Vector4(1, 1, 1, 1));
        outline_material->SetTexture("_MainTex", bodyTexture);
        outline_material->SetFloat("_EdgeThickness", edgeThickness);

        textures["DiffuseMap"] = bodyTexture;
        material->BindTextures(textures);
        material->setName("body");
        //material->BindTextures("DiffuseMap", )
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
        material->SetTexture("DiffuseMap", skinTexture);
        material->setName("skin");
        for (auto name : {"skin"}) {
            auto child = FindNamedChild(modelGO, name);
            assert(child != nullptr);
            child->GetComponent<SkinnedMeshRenderer>()->SetMaterial(material);
            child->GetComponent<SkinnedMeshRenderer>()->AddMaterial(outline_material);
        }

        material = Material::builtinMaterial("Texture");
        material->setName("face");
        material->SetTexture("DiffuseMap", faceTexture);
        for (auto name : {"MTH_DEF", "EYE_DEF", "head_back"}) {
            auto child = FindNamedChild(modelGO, name);
            assert(child != nullptr);
            child->GetComponent<MeshRenderer>()->SetMaterial(material);
            child->GetComponent<MeshRenderer>()->AddMaterial(outline_material);
        }
        
        material = Material::builtinMaterial("Texture");
        material->setName("hair");
        material->SetTexture("DiffuseMap", hairTexture);
        for (auto name : {"hair_front", "hair_frontside", "tail", "tail_bottom"}) {
            auto child = FindNamedChild(modelGO, name);
            assert(child != nullptr);
            child->GetComponent<SkinnedMeshRenderer>()->SetMaterial(material);
            child->GetComponent<SkinnedMeshRenderer>()->AddMaterial(outline_material);
        }

        material = Material::builtinMaterial("Transparent");
        material->setName("eye_L1");
        material->SetTexture("DiffuseMap", eyeirisLTexture);
        for (auto name : {"eye_L_old"}) {
            auto child = FindNamedChild(modelGO, name);
            assert(child != nullptr);
            child->GetComponent<MeshRenderer>()->SetMaterial(material);
            //child->SetActive(false);
        }
        
        material = Material::builtinMaterial("Transparent");
        material->setName("eye_R1");
        material->SetTexture("DiffuseMap", eyeirisRTexture);
        for (auto name : {"eye_R_old"}) {
            auto child = FindNamedChild(modelGO, name);
            assert(child != nullptr);
            child->GetComponent<MeshRenderer>()->SetMaterial(material);
        }
        
        material = Material::builtinMaterial("Transparent");
        material->SetTexture("DiffuseMap", eyelineTexture);
        for (auto name : {"BLW_DEF", "EL_DEF"}) {
            auto child = FindNamedChild(modelGO, name);
            assert(child != nullptr);
            child->GetComponent<MeshRenderer>()->SetMaterial(material);
        }
        
        material = Material::builtinMaterial("Texture");
        material->setName("eyeline");
        material->SetTexture("DiffuseMap", eyelineTexture);
        for (auto name : {"eye_base_old"}) {
            auto child = FindNamedChild(modelGO, name);
            assert(child != nullptr);
            child->GetComponent<MeshRenderer>()->SetMaterial(material);
            //Selection::setActiveGameObject(child);
        }

        material = Material::builtinMaterial("Transparent");
        material->setName("mat_cheek");
        material->SetTexture("DiffuseMap", cheekTexture);
        for (auto name : {"cheek"}) {
            auto child = FindNamedChild(modelGO, name);
            assert(child != nullptr);
            child->GetComponent<SkinnedMeshRenderer>()->SetMaterial(material);
        }
        
        // Camera
        
        auto cameraGO = Camera::mainGameCamera()->gameObject();
        cameraGO->transform()->setLocalPosition(0, 0.8f, -2.6f);
        cameraGO->transform()->setLocalEulerAngles(0, 0, 0);
        cameraGO->AddComponent<ShowFPS>();
        cameraGO->AddComponent<TakeScreenShot>();
        //cameraGO->AddScript(make_shared<RenderSettings>());
        //cameraGO->AddScript(make_shared<DisplayMatrix>());
        auto s = make_shared<EditorRenderSettings>();
        s->m_useGammaCorrection = false;
        cameraGO->AddComponent(s);
        //cameraGO->GetComponent<EditorRenderSettings>()->m_useGammaCorrection = false;

        Selection::setActiveGameObject(cameraGO);
    }
};

std::shared_ptr<GameObject> CreateCube()
{
    auto model = Model::builtinModel(BuiltinModelType::Cube);
    auto go = model->CreateGameObject();
    auto collider = make_shared<BoxCollider>(Vector3::zero, Vector3::one);
    go->AddComponent(collider);
    go->AddComponent<Rigidbody>();
    collider->physicsShape();
    return go;
}
            
class TestPhysics : public App
{
public:
    virtual void Init() override
    {
        DefaultScene();
        
        auto model = Model::builtinModel(BuiltinModelType::Cube);
        auto go = model->CreateGameObject();
        //go->transform()->setLocalEulerAngles(0, 0, 10);
        go->transform()->setPosition(0, -1.0f-0.1f*0.5f, 0);
        go->transform()->setLocalScale(10, 0.1f, 10);
        auto boxCollider = make_shared<BoxCollider>(Vector3::zero, Vector3::one);
        go->AddComponent(boxCollider);
        boxCollider->physicsShape();
        
        auto sphereModel = Model::builtinModel(BuiltinModelType::Sphere);
        auto sphereGO = sphereModel->CreateGameObject();
        sphereGO->transform()->setPosition(0, 0, 0);
        auto sphereCollider = make_shared<SphereCollider>(Vector3::zero, 0.5f);
        sphereGO->AddComponent(sphereCollider);
        sphereGO->AddComponent<Rigidbody>();
        sphereCollider->physicsShape();
        
        //auto cubeGO = CreateCube();
        
        auto cameraGO = Camera::mainGameCamera()->gameObject();
        cameraGO->transform()->setLocalPosition(0, 2, -10);
        cameraGO->transform()->setLocalEulerAngles(0, 0, 0);
        //cameraGO->AddComponent(make_shared<TestGizmos>());
    }
};
            
void test()
{
    cout << std::is_base_of<Component, Mesh>::value;
    {
        Vector3 v(1, 2, 3);
        Vector3 v2;
        std::stringstream ss;
        boost::archive::xml_oarchive oa(ss);
        oa << BOOST_SERIALIZATION_NVP(v);
        
        boost::archive::xml_iarchive ia(ss);
        ia >> BOOST_SERIALIZATION_NVP(v2);
        auto xml = ss.str();
        cout << xml << endl;
    }
    {
        Transform t;
        t.setLocalPosition(1, 2, 3);
        t.setLocalEulerAngles(10, 15, 27);
        t.setLocalScale(1.1f, 2.2f, 3.3f);
        Transform t2;
        std::stringstream ss;
        boost::archive::xml_oarchive oa(ss);
        oa << BOOST_SERIALIZATION_NVP(t);
        
        boost::archive::xml_iarchive ia(ss);
        ia >> BOOST_SERIALIZATION_NVP(t2);
        auto xml = ss.str();
        cout << xml << endl;
    }
    {
        std::stringstream ss;
        boost::archive::xml_oarchive oa(ss);
        boost::archive::xml_iarchive ia(ss);
        
        Camera camera2(60.0f, Screen::aspect(), 0.3f, 1000.f);
        oa << BOOST_SERIALIZATION_NVP(camera2);
        auto xml = ss.str();
        cout << xml << endl;
    }
}

int main()
{
    //test();
    FishEditorWindow::AddApp(make_shared<TestAnimation>());
    //FishEditorWindow::AddApp(make_shared<TestPhysics>());
    FishEditorWindow::Init();
    FishEditorWindow::Run();
    FishEditorWindow::Clean();
    return 0;
}
