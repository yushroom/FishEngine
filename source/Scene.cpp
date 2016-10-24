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
#include "Frustum.hpp"
#include "Gizmos.hpp"


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
        
        //UpdateBounds();
    }

    void Scene::RenderShadow(PLight& light)
    {
        // get compact light frustum
        auto    camera          = Camera::mainGameCamera();
        //auto    world_to_camera = camera->transform()->worldToLocalMatrix();
        auto    camera_to_world = camera->transform()->localToWorldMatrix();
        Vector3 camera_pos      = camera->transform()->position();
        Vector3 light_dir       = light->transform()->forward();
        
//        Frustum view_frustum;  // in camera's local space
//        //view_frustrum.center    = camera_pos;
//        view_frustrum.aspect    = camera->aspect();
//        view_frustrum.fov       = camera->fieldOfView();
//        view_frustrum.minRange  = camera->nearClipPlane();
//        view_frustrum.maxRange  = camera->farClipPlane();
        
        Frustum view_frustum = camera->frustum();
        
        UpdateBounds(); // get bounding box of the whole scene
        Bounds bound_in_camera_space = camera->transform()->InverseTransformBounds(m_bounds);
        auto pmin = bound_in_camera_space.min();
        auto pmax = bound_in_camera_space.max();
        if (view_frustum.minRange < pmin.z)
        {
            view_frustum.minRange = pmin.z;
        }
        if (view_frustum.maxRange > pmax.z)
        {
            view_frustum.maxRange = pmax.z;
        }
        
        Gizmos::setMatrix(camera_to_world);
        Gizmos::setColor(Color::cyan);
        Gizmos::DrawFrustum(view_frustum);
        Gizmos::setMatrix(Matrix4x4::identity);
        
        Matrix4x4 light_view_matrix = Matrix4x4::LookAt(camera_pos, camera_pos+light_dir, camera->transform()->up());
        Vector3 local_corners_of_view_frustum[8];
        view_frustum.getLocalCorners(local_corners_of_view_frustum);
        auto& world_to_light = light_view_matrix;
        auto light_to_world = light_view_matrix.inverse();
        Bounds aabb;    // the bounding box of view frustum in light's local space
        for (auto corner : local_corners_of_view_frustum)
        {
            corner = camera_to_world.MultiplyPoint(corner); // -> world space
            corner = world_to_light.MultiplyPoint(corner);  // -> light's local space
            aabb.Encapsulate(corner);
        }
        
        // light frustum
        Gizmos::setMatrix(light_to_world);
        Gizmos::setColor(Color::magenta);
        Gizmos::DrawWireCube(aabb.center(), aabb.size());
        Gizmos::DrawLine(aabb.center(), aabb.center()+Vector3(0, 0, 1)*aabb.extents());
        Gizmos::setMatrix(Matrix4x4::identity);
        
        
        Vector3 new_light_position = aabb.center();
        new_light_position.z -= aabb.extents().z + light->shadowNearPlane();
        new_light_position = light_to_world.MultiplyPoint(new_light_position);
        
        Gizmos::DrawWireSphere(new_light_position, 0.5f);
        
        light->m_viewMatrixForShadowMap = Matrix4x4::LookAt(new_light_position, new_light_position+light_dir, camera->transform()->up());
        //auto dir = light->m_viewMatrixForShadowMap.MultiplyVector(0, 0, 1);
        const auto& ext = aabb.extents();
        light->m_projectMatrixForShadowMap = Matrix4x4::Ortho(-ext.x, ext.x, -ext.y, ext.y, light->shadowNearPlane(), ext.z*2+light->shadowNearPlane());
        
        
        //glCullFace(GL_FRONT);
        auto shadow_map_material = Material::builtinMaterial("ShadowMap");
        auto& view = light->m_viewMatrixForShadowMap;
        auto& proj = light->m_projectMatrixForShadowMap;

        auto shadowMap = light->m_shadowMap;
        glViewport(0, 0, shadowMap->width(), shadowMap->height());
        glBindFramebuffer(GL_FRAMEBUFFER, shadowMap->depthBufferFBO());
        glClear(GL_DEPTH_BUFFER_BIT);

        //shader->Use();
        auto light_vp = light->m_projectMatrixForShadowMap * light->m_viewMatrixForShadowMap;
        
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
                    auto shader = shadow_map_material->shader();
                    shader->Use();
                    //Pipeline::perDrawUniformData.MATRIX_MVP = proj * view * go->transform()->localToWorldMatrix();
                    //Pipeline::BindPerDrawUniforms();
                    auto mvp = light_vp * go->transform()->localToWorldMatrix();
                    shader->BindUniformMat4("MATRIX_LIGHT_MVP", mvp);
                    shader->CheckStatus();
                    mesh->Render();
                    continue;
                }
            }

            auto renderer = go->GetComponent<SkinnedMeshRenderer>();
            if (renderer != nullptr)
            {
                mesh = renderer->sharedMesh();
                auto shader = shadow_map_material->shader()->m_skinnedShader;
                shader->Use();
                //Pipeline::perDrawUniformData.MATRIX_MVP = proj * view * go->transform()->localToWorldMatrix();
                //Pipeline::BindPerDrawUniforms();
                auto mvp = light_vp * go->transform()->localToWorldMatrix();
                shader->BindUniformMat4("MATRIX_LIGHT_MVP", mvp);
                if (renderer->m_avatar != nullptr)
                    shader->m_skinnedShader->BindMatrixArray("BoneTransformations", renderer->m_matrixPalette);
                shader->CheckStatus();
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


    void Scene::Render()
    {
        std::vector<std::shared_ptr<GameObject>> transparentQueue;
        for (auto& go : Scene::m_gameObjects) {
            if (!go->activeInHierarchy()) continue;
            std::shared_ptr<Renderer> renderer = go->GetComponent<MeshRenderer>();
            if (renderer == nullptr) {
                renderer = go->GetComponent<SkinnedMeshRenderer>();
                if (renderer == nullptr)
                    continue;
            }
            bool isTransparent = renderer->material()->shader()->IsTransparent();
            if (isTransparent) {
                transparentQueue.push_back(go);
                continue;
            }
            renderer->Render();
        }
        
        /************************************************************************/
        /* Transparent                                                          */
        /************************************************************************/
        for (auto& go : transparentQueue) {
            std::shared_ptr<Renderer> renderer = go->GetComponent<MeshRenderer>();
            if (renderer == nullptr) {
                renderer = go->GetComponent<SkinnedMeshRenderer>();
            }
            renderer->Render();
        }
        
        transparentQueue.clear();
    }

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

