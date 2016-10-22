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
    Bounds                      Scene::m_bounds;
    SceneOctree                 Scene::m_octree(Bounds(), 16);

    PGameObject Scene::CreateGameObject(const std::string& name)
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
        UpdateBounds();
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

        // Destroy game objects
        for (auto& g : m_gameObjectsToBeDestroyed) {
            DestroyImmediate(g);
        }
        m_gameObjectsToBeDestroyed.clear(); // release (the last) strong refs, game objects should be destroyed automatically.

        //m_bounds = Bounds();
        
        for (auto& go : m_gameObjects) {
            if (!go->activeInHierarchy()) continue;
            go->Update();
        }
        
        UpdateBounds();
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

        for (auto& go : m_gameObjects)
        {
            if (!go->activeInHierarchy()) continue;

            PMesh mesh;
            if (go->GetComponent<MeshRenderer>())
            {
                auto meshFilter = go->GetComponent<MeshFilter>();
                if (meshFilter != nullptr)
                {
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
            if (renderer != nullptr)
            {
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


    void Scene::Destroy(PGameObject obj, const float t /*= 0.0f*/)
    {
        m_gameObjectsToBeDestroyed.push_back(obj);
    }


    void Scene::Destroy(PScript s, const float t /*= 0.0f*/)
    {
        m_scriptsToBeDestroyed.push_back(s);
    }


    void Scene::Destroy(PComponent c, const float t /*= 0.0f*/)
    {
        m_componentsToBeDestroyed.push_back(c);
    }


    void Scene::DestroyImmediate(PGameObject g)
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

    void Scene::DestroyImmediate(PComponent c)
    {
        c->gameObject()->RemoveComponent(c);
    }


    void Scene::DestroyImmediate(PScript s)
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

    PGameObject Scene::Find(const std::string& name)
    {
        for (auto& go : m_gameObjects) {
            if (go->name() == name) {
                return go;
            }
        }
        return nullptr;
    }
    
    void Scene::UpdateBounds()
    {
        m_bounds = Bounds();
        for (auto& go : m_gameObjects)
        {
            if (go->transform()->parent() == nullptr)
            {
                Bounds bound;
                auto rend = go->GetComponent<MeshRenderer>();
                if (rend == nullptr)
                {
                    auto srend = go->GetComponent<SkinnedMeshRenderer>();
                    if (srend == nullptr)
                        continue;
                    bound = srend->bounds();
                }
                else {
                    bound = rend->bounds();
                }
                m_bounds.Encapsulate(bound);
            }
        }
    }
    
    PGameObject Scene::IntersectRay(const Ray& ray)
    {
        if (!m_bounds.IntersectRay(ray))
            return nullptr;
        
        PGameObject selected = nullptr;
        for (auto& go : m_gameObjects)
        {
            if (go->transform()->parent() == nullptr)
            {
                Bounds bound;
                auto rend = go->GetComponent<MeshRenderer>();
                if (rend == nullptr)
                {
                    auto srend = go->GetComponent<SkinnedMeshRenderer>();
                    if (srend == nullptr)
                        continue;
                    bound = srend->bounds();
                }
                else {
                    bound = rend->bounds();
                }
                //m_bounds.Encapsulate(bound);

                float tmin = Mathf::Infinity;
                float t = Mathf::Infinity;
                if (bound.IntersectRay(ray, &t))
                {
                    if (t < tmin)
                    {
                        tmin = t;
                        selected = go;
                    }
                }
            }
        }
        
        return selected;
    }
}

