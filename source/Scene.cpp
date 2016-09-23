#include "Scene.hpp"
#include "RenderSystem.hpp"
#include "MeshRenderer.hpp"
#include "CameraController.hpp"
#include "Light.hpp"
#include "Mesh.hpp"
#include "MeshFilter.hpp"

NAMESPACE_FISHENGINE_BEGIN

std::list<std::shared_ptr<GameObject>>      Scene::m_gameObjects;
std::vector<std::shared_ptr<GameObject>>    Scene::m_gameObjectsToBeDestroyed;
std::vector<std::shared_ptr<Script>>        Scene::m_scriptsToBeDestroyed;
std::vector<std::shared_ptr<Component>>     Scene::m_componentsToBeDestroyed;

std::shared_ptr<Camera> Scene::m_mainCamera = nullptr;

//std::shared_ptr<GameObject> Scene::m_activeGameObject = nullptr;
//GameObject* Scene::m_activeGameObject = nullptr;

std::shared_ptr<GameObject> FishEngine::Scene::CreateGameObject(const std::string& name)
{
    auto go = std::make_shared<GameObject>(name);
    go->transform()->m_gameObject = go;
    m_gameObjects.push_back(go);
    return go;
}

void Scene::Init() {
    int width = RenderSystem::width();
    int height = RenderSystem::height();
    m_mainCamera = std::make_shared<Camera>(60.0f, float(width) / height, 0.3f, 1000.f);
    //auto camera_go = std::make_shared<GameObject>("MainCamera");
    auto camera_go = Scene::CreateGameObject("MainCamera");
    camera_go->AddScript(std::make_shared<CameraController>());
    camera_go->AddComponent(m_mainCamera);
    camera_go->transform()->setLocalPosition(0, 0, 5);
    camera_go->transform()->LookAt(0, 0, 0);
    //m_gameObjects.push_back(camera_go);
    camera_go->SetTag("MainCamera");
    
    //m_activeGameObject = camera_go.get();
    //SelectGameObject(camera_go.get());
}

void Scene::Start() {
    for (auto& go : m_gameObjects) {
        go->Start();
    }
}

void Scene::Update() {
    // Destroy components
    for (auto & c : m_componentsToBeDestroyed) {
        c->gameObject()->RemoveComponent(c);
    }
    m_componentsToBeDestroyed.clear();

    // Destroy scripts
    for (auto & s : m_scriptsToBeDestroyed) {
        s->gameObject()->RemoveScript(s);
    }
    m_scriptsToBeDestroyed.clear();

    // Destroy gameobjcets
    for (auto& g : m_gameObjectsToBeDestroyed) {
        m_gameObjects.remove(g);
        auto t = g->transform();
        t->SetParent(nullptr);  // remove from parent
        
        // remove children
        for (auto& c : t->m_children) {
            m_gameObjects.remove(c.lock()->gameObject());
        }
    }
    m_gameObjectsToBeDestroyed.clear(); // release (the last) strong refs, gameobjects should be destroyed automatically.

    for (auto& go : m_gameObjects) {
        if (!go->activeInHierarchy()) continue;
        go->Update();
    }
}

void Scene::RenderShadow(std::shared_ptr<Light>& light)
{
    auto m = Material::builtinMaterial("ShadowMap");
    auto shader = m->shader();
    ShaderUniforms uniforms;
    auto view = light->gameObject()->transform()->worldToLocalMatrix();
    auto proj = Matrix4x4::Ortho(-10.f, 10.f, -10.f, 10.f, light->shadowNearPlane(), 100.f);
    
    auto shadowMap = light->m_shadowMap;
    glViewport(0, 0, shadowMap->width(), shadowMap->height());
    glBindFramebuffer(GL_FRAMEBUFFER, shadowMap->depthBufferFBO());
    glClear(GL_DEPTH_BUFFER_BIT);
    
    for (auto& go : m_gameObjects) {
        if (!go->activeInHierarchy()) continue;
        auto renderer = go->GetComponent<MeshRenderer>();
        auto meshFilter = go->GetComponent<MeshFilter>();
        if (renderer != nullptr && meshFilter != nullptr) {
            //renderer->Render();
            //auto shader = m->shader();
            //shader->Use();
            //shader->PreRender();
            uniforms.mat4s["MATRIX_MVP"] = proj * view * go->transform()->localToWorldMatrix();
            shader->BindUniforms(uniforms);
            //m->Update();
            shader->CheckStatus();
            meshFilter->mesh()->Render();
            //shader->PostRender();
        }
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Scene::Render()
{
    for (auto& go : m_gameObjects) {
        if (!go->activeInHierarchy()) continue;
        auto renderer = go->GetComponent<MeshRenderer>();
        if (renderer != nullptr) {
            renderer->Render();
        }
    }
}

GameObject::PGameObject FishEngine::Scene::Find(const std::string& name)
{
    for (auto& go : m_gameObjects) {
        if (go->name() == name) {
            return go;
        }
    }
    return nullptr;
}


NAMESPACE_FISHENGINE_END

