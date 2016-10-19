#include "Scene.hpp"
#include "GameObject.hpp"
#include "Camera.hpp"
#include "RenderSystem.hpp"
#include "MeshRenderer.hpp"
#include "CameraController.hpp"
#include "Light.hpp"
#include "Mesh.hpp"
#include "MeshFilter.hpp"
#include "Pipeline.hpp"
#include "SkinnedMeshRenderer.hpp"


namespace FishEngine
{
    std::list<PGameObject>      Scene::m_gameObjects;
    std::vector<PGameObject>    Scene::m_gameObjectsToBeDestroyed;
    std::vector<PScript>        Scene::m_scriptsToBeDestroyed;
    std::vector<PComponent>     Scene::m_componentsToBeDestroyed;

    //PGameObject> Scene::m_activeGameObject = nullptr;
    //GameObject* Scene::m_activeGameObject = nullptr;

    PGameObject FishEngine::Scene::CreateGameObject(const std::string& name)
    {
        auto go = std::make_shared<GameObject>(name);
        go->transform()->m_gameObject = go;
        m_gameObjects.push_back(go);
        return go;
    }

    void Scene::Init() {

    }

    void Scene::Start() {
        for (auto& go : m_gameObjects) {
            if (go->activeInHierarchy())
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
            DestroyImmediate(g);
        }
        m_gameObjectsToBeDestroyed.clear(); // release (the last) strong refs, gameobjects should be destroyed automatically.

        for (auto& go : m_gameObjects) {
            if (!go->activeInHierarchy()) continue;
            go->Update();
        }
    }

    void Scene::RenderShadow(PLight& light)
    {
        //glCullFace(GL_FRONT);
        auto m = Material::builtinMaterial("ShadowMap");
        auto shader = m->shader();
        //shader->Use();
        //ShaderUniforms uniforms;
        auto view = light->transform()->worldToLocalMatrix();
        auto proj = Matrix4x4::Ortho(-10.f, 10.f, -10.f, 10.f, light->shadowNearPlane(), 100.f);
        //auto proj = Camera::main()->projectionMatrix();

        auto shadowMap = light->m_shadowMap;
        glViewport(0, 0, shadowMap->width(), shadowMap->height());
        glBindFramebuffer(GL_FRAMEBUFFER, shadowMap->depthBufferFBO());
        glClear(GL_DEPTH_BUFFER_BIT);

        for (auto& go : m_gameObjects) {
            if (!go->activeInHierarchy()) continue;

            // TODO: remove this line
            if ("Skybox" == go->name()) {
                Debug::Log("here");
            }

            PMesh mesh;
            if (go->GetComponent<MeshRenderer>()) {
                auto meshFilter = go->GetComponent<MeshFilter>();
                if (meshFilter != nullptr) {
                    mesh = meshFilter->mesh();
                    shader->Use();
                    Pipeline::perDrawUniformData.MATRIX_MVP = proj * view * go->transform()->localToWorldMatrix();
                    Pipeline::BindPerDrawUniforms();
                    shader->CheckStatus();
                    mesh->Render();
                    continue;
                }
            }
            auto renderer = go->GetComponent<SkinnedMeshRenderer>();
            if (renderer != nullptr) {
                mesh = renderer->sharedMesh();
                shader->m_skinnedShader->Use();
                Pipeline::perDrawUniformData.MATRIX_MVP = proj * view * go->transform()->localToWorldMatrix();
                Pipeline::BindPerDrawUniforms();
                if (renderer->m_avatar != nullptr)
                    shader->m_skinnedShader->BindMatrixArray("BoneTransformations", renderer->m_matrixPalette);
                shader->m_skinnedShader->CheckStatus();
                mesh->Render();
            }
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Scene::OnDrawGizmos()
    {
        for (auto& go : m_gameObjects) {
            go->OnDrawGizmos();
        }
    }


    void FishEngine::Scene::Destroy(PGameObject obj, const float t /*= 0.0f*/)
    {
        m_gameObjectsToBeDestroyed.push_back(obj);
    }


    void FishEngine::Scene::Destroy(PScript s, const float t /*= 0.0f*/)
    {
        m_scriptsToBeDestroyed.push_back(s);
    }


    void FishEngine::Scene::Destroy(PComponent c, const float t /*= 0.0f*/)
    {
        m_componentsToBeDestroyed.push_back(c);
    }


    void FishEngine::Scene::DestroyImmediate(PGameObject g)
    {
        auto t = g->transform();
        // remove children
        while (!t->m_children.empty())
        {
            auto c = t->m_children.back();
            t->m_children.pop_back();
            DestroyImmediate(c.lock()->gameObject());
        }
        t->SetParent(nullptr);  // remove from parent
        m_gameObjects.remove(g);
    }

    void FishEngine::Scene::DestroyImmediate(PComponent c)
    {
        c->gameObject()->RemoveComponent(c);
    }


    void FishEngine::Scene::DestroyImmediate(PScript s)
    {
        s->gameObject()->RemoveScript(s);
    }


    //void Scene::Render()
    //{
    //    for (auto& go : m_gameObjects) {
    //        if (!go->activeInHierarchy()) continue;
    //        auto renderer = go->GetComponent<MeshRenderer>();
    //        if (renderer != nullptr) {
    //            renderer->Render();
    //        }
    //    }
    //}

    PGameObject FishEngine::Scene::Find(const std::string& name)
    {
        for (auto& go : m_gameObjects) {
            if (go->name() == name) {
                return go;
            }
        }
        return nullptr;
    }
}

