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
#include "Shader.hpp"
#include "QualitySettings.hpp"
//#include "Serialization.hpp"
//#include "Serialization/archives/yaml.hpp"
#include "Camera.hpp"

#include "GLEnvironment.hpp"

namespace FishEngine
{
	std::list<GameObjectPtr>      Scene::m_gameObjects;
	std::vector<GameObjectPtr>    Scene::m_gameObjectsToBeDestroyed;
	std::vector<ComponentPtr>     Scene::m_componentsToBeDestroyed;
	Bounds                      Scene::m_bounds;
	//SceneOctree                 Scene::m_octree(Bounds(), 16);

	GameObjectPtr Scene::CreateGameObject(const std::string& name)
	{
		//auto go = std::make_shared<GameObject>(name);
		auto go = GameObject::Create();
		go->setName(name);
		go->transform()->m_gameObject = go;
		m_gameObjects.push_back(go);
		return go;
	}

	GameObjectPtr Scene::CreateCamera()
	{
		auto camera_go = Scene::CreateGameObject("Camera");
		//auto camera_go = GameObject::Create();
		//camera_go->setName("Camera");
		//camera_go->AddComponent<Camera>();
		auto camera = Camera::Create();	// make sure camera is in Camera::allcameras
		camera_go->AddComponent(camera);
		camera_go->AddComponent<CameraController>();
		return camera_go;
	}

	void Scene::Init()
	{

	}

	void Scene::Start()
	{
		for (auto& go : m_gameObjects) {
			if (go->activeInHierarchy())
				go->Start();
		}
		UpdateBounds();
	}

	void Scene::Update()
	{
		// Destroy components
		for (auto & c : m_componentsToBeDestroyed)
		{
			c->gameObject()->RemoveComponent(c);
		}
		m_componentsToBeDestroyed.clear();

		// Destroy game objects
		for (auto& g : m_gameObjectsToBeDestroyed)
		{
			DestroyImmediate(g);
		}
		m_gameObjectsToBeDestroyed.clear(); // release (the last) strong refs, game objects should be destroyed automatically.

		for (auto& go : m_gameObjects)
		{
			if (!go->activeInHierarchy()) continue;
			go->Update();
		}
		
		//UpdateBounds();
	}

	void Scene::RenderShadow(LightPtr const & light)
	{
		if (light == nullptr)
		{
			return;
		}
		

#define DEBUG_SHADOW 1
		auto    camera = Camera::main();
		auto    camera_to_world = camera->transform()->localToWorldMatrix();
		float   near = camera->nearClipPlane();
		//float   far = camera->farClipPlane();
		float  far = std::min( QualitySettings::shadowDistance(), camera->farClipPlane() );

		Vector3 light_dir = light->transform()->forward();
		Frustum total_frustum = camera->frustum();

		constexpr float splits[] = { 0, 1.0f / 15.0f, 3.0f / 15.0f, 7.0f / 15.0f, 1 };

		for (int i = 0; i < 4; ++i)
		{
#if 0
			// From GPU Gem 3. Chap 10 "Practical Split Scheme".
			const float split_lambda = 0.5f;
			float split_near = near;
			if (i > 0)
				split_near = Mathf::Lerp(near + i / 4.0f * (far - near), near * Mathf::Pow(far / near, i / 4.0f), split_lambda);
			float split_far = far;
			if (i < 4 - 1)
				split_far = Mathf::Lerp(near + (i + 1) / 4.0f * (far - near), near*Mathf::Pow(far / near, (i + 1) / 4.0f), split_lambda);
#else
			// From Unity
			// 0:    0 ~ 1/15,  1/15
			// 1: 1/15 ~ 3/15,  2/15
			// 2: 3/15 ~ 7/15,  4/15
			// 3: 7/15 ~ 1,     8/15
			float split_near = Mathf::Lerp(near, far, splits[i]);
			float split_far = Mathf::Lerp(near, far, splits[i+1]);
#endif

			Frustum frustum = total_frustum;
			frustum.minRange = split_near;
			frustum.maxRange = split_far;

			Gizmos::setMatrix(camera_to_world);
			Gizmos::setColor(Color::cyan * (i / 3.0f));
			Gizmos::DrawFrustum(frustum);
			Gizmos::setMatrix(Matrix4x4::identity);

			Vector3 view_corners[8];
			frustum.getLocalCorners(view_corners);
			Vector3 world_corners[8];
			for (int i = 0; i < 8; ++i)
			{
				world_corners[i] = camera_to_world.MultiplyPoint(view_corners[i]);
			}

			Vector3 split_centroid(0, 0, 0);
			for (auto& c : world_corners)
			{
				split_centroid += c;
			}
			split_centroid /= 8.0f;

			float dist = Mathf::Max(split_far - split_near, Vector3::Distance(world_corners[4], world_corners[5]));
			auto eye_pos = split_centroid - light_dir * dist;
			Gizmos::DrawWireSphere(eye_pos, 0.5f);
			Matrix4x4 world_to_light = Matrix4x4::LookAt(eye_pos, split_centroid, Vector3::up);

			Bounds aabb;    // the bounding box of view frustum in light's local space
			for (int i = 0; i < 8; ++i)
			{
				view_corners[i] = world_to_light.MultiplyPoint(world_corners[i]);
				aabb.Encapsulate(view_corners[i]);
			}

#define CSM_SPLIT_SPHERE
#ifdef CSM_SPLIT_SPHERE
			float sphereRadius = 0.0f;
			for (auto& c : world_corners)
			{
				float dist = Vector3::Distance(c, split_centroid);
				sphereRadius = std::max(sphereRadius, dist);
			}

			sphereRadius = std::ceil(sphereRadius * 16.0f) / 16.0f;
			Vector3 maxExtents(sphereRadius, sphereRadius, sphereRadius);
			Vector3 minExtents = -maxExtents;
			Vector3 cascadeExtents = maxExtents - minExtents;
#endif
			constexpr float near_offset = 10.0f;
			constexpr float far_offset = 20.0f;
			auto min_p = aabb.min();
			auto max_p = aabb.max();

			Gizmos::setColor(Color::red * (i / 3.0f));
			Gizmos::setMatrix(world_to_light.inverse());
			Gizmos::DrawWireCube(aabb.center(), aabb.size());
			Gizmos::setMatrix(Matrix4x4::identity);

			float z_near = min_p.z;
			float z_far = max_p.z;
			light->m_cascadesNear[i] = z_near - near_offset;
			light->m_cascadesFar[i] = z_far + far_offset;

#if 0
			light->m_projectMatrixForShadowMap[i] = Matrix4x4::Ortho(min_p.x, max_p.x, min_p.y, max_p.y, z_near, z_far);
			light->m_viewMatrixForShadowMap[i] = world_to_light;
#elif 0
			float scaleX = 2.0f / (max_p.x - min_p.x);
			float scaleY = 2.0f / (max_p.y - min_p.y);
			constexpr float scaleQuantizer = 64.0f;
			scaleX = 1.0f / std::ceilf(1.0f / scaleX * scaleQuantizer) * scaleQuantizer;
			scaleY = 1.0f / std::ceilf(1.0f / scaleY * scaleQuantizer) * scaleQuantizer;
			float offsetX = -0.5f * (max_p.x + min_p.x) * scaleX;
			float offsetY = -0.5f * (max_p.y + min_p.y) * scaleY;
			const float halfTextureSize = 0.5f * light->m_shadowMap->width();
			offsetX = std::ceilf(offsetX * halfTextureSize) / halfTextureSize;
			offsetY = std::ceilf(offsetY * halfTextureSize) / halfTextureSize;
			auto& forward = light_dir; // +z
			auto right = Vector3::Cross(Vector3(0, 1, 0), forward); // +x
			auto up = Vector3::Cross(forward, right); // +y
			eye_pos += right * offsetX + up * offsetY;
			split_centroid += right * offsetX + up * offsetY;
			//Gizmos::DrawWireSphere(eye_pos, 0.5f);
			world_to_light = Matrix4x4::LookAt(eye_pos, split_centroid, Vector3::up);
			light->m_projectMatrixForShadowMap[i] = Matrix4x4::Ortho(min_p.x, max_p.x, min_p.y, max_p.y, z_near, z_far);
			light->m_viewMatrixForShadowMap[i] = world_to_light;
#else
			//Debug::Log("sphereRadius: %lf", sphereRadius);
			auto shadowCameraPos = -light_dir * sphereRadius + split_centroid;
			auto shadowView = Matrix4x4::LookAt(shadowCameraPos, split_centroid, Vector3::up);
			auto shadowProj = Matrix4x4::Ortho(minExtents.x, maxExtents.x, minExtents.y, maxExtents.y, 0, cascadeExtents.z);

			const float halfShadowMapSize = 0.5f * light->m_shadowMap->width();
			auto shadowMatrix = shadowProj * shadowView;
			Vector3 shadowOrigin = shadowMatrix.MultiplyPoint3x4(Vector3::zero);
			shadowOrigin *= halfShadowMapSize;
			Vector3 roundedOrigin{ std::round(shadowOrigin.x), std::round(shadowOrigin.y), std::round(shadowOrigin.z) };
			Vector3 roundOffset = roundedOrigin - shadowOrigin;
			roundOffset /= halfShadowMapSize;
			roundOffset.z = 0.0f;

			shadowProj[0][3] += roundOffset.x;
			shadowProj[1][3] += roundOffset.y;

			light->m_projectMatrixForShadowMap[i] = shadowProj;
			light->m_viewMatrixForShadowMap[i] = shadowView;
#endif

			light->m_cascadesSplitPlaneNear[i] = split_near;
			light->m_cascadesSplitPlaneFar[i] = split_far;
		}

		auto shadow_map_material = Material::builtinMaterial("CascadedShadowMap");

		Pipeline::BindLight(light);

		auto shadowMap = light->m_shadowMap;
		Pipeline::PushRenderTarget(light->m_renderTarget);

		glViewport(0, 0, shadowMap->width(), shadowMap->height());
		glClear(GL_DEPTH_BUFFER_BIT);

		glEnable(GL_DEPTH_CLAMP);

		auto shader = shadow_map_material->shader();
		shader->Use();
		//shader->BindUniformMat4("TestMat", Matrix4x4::identity);

#if 1
		auto gameObjects = m_gameObjects;
		
		while (!gameObjects.empty())
		{
			auto go = gameObjects.front();
			gameObjects.pop_front();
			
			if (!go->activeInHierarchy())
				continue;
			
			for (auto & child : go->transform()->children())
			{
				gameObjects.push_back(child->gameObject());
			}

			bool is_skinned = false;

			MeshPtr mesh;
			auto mesh_renderer = go->GetComponent<MeshRenderer>();
			if (mesh_renderer != nullptr)
			{
				if (mesh_renderer->shadowCastingMode() != ShadowCastingMode::Off)
				{
					auto meshFilter = go->GetComponent<MeshFilter>();
					if (meshFilter != nullptr)
					{
						mesh = meshFilter->mesh();
					}
				}
			}
			else
			{
				auto renderer = go->GetComponent<SkinnedMeshRenderer>();
				if (renderer != nullptr && renderer->shadowCastingMode() != ShadowCastingMode::Off)
				{
					mesh = renderer->sharedMesh();
					if (renderer->m_avatar != nullptr)
					{
						shadow_map_material->EnableKeyword(ShaderKeyword::SkinnedAnimation);
						is_skinned = true;
						Pipeline::UpdateBonesUniforms(renderer->matrixPalette());
					}
				}
			}
			
			if (!is_skinned)
			{
				shadow_map_material->DisableKeyword(ShaderKeyword::SkinnedAnimation);
			}

			
			if (mesh != nullptr)
			{
				Pipeline::UpdatePerDrawUniforms(go->transform()->localToWorldMatrix());
				//shader->BindUniformMat4("ObjectToWorld", go->transform()->localToWorldMatrix());
				shader->CheckStatus();
				mesh->Render();
			}

		}
		
#else
		for (auto& go : m_gameObjects)
		{
			bool is_skinned = false;

			if (!go->activeInHierarchy())
				continue;

			MeshPtr mesh;
			auto mesh_renderer = go->GetComponent<MeshRenderer>();
			if (mesh_renderer != nullptr)
			{
				if (mesh_renderer->shadowCastingMode() != ShadowCastingMode::Off)
				{
					auto meshFilter = go->GetComponent<MeshFilter>();
					if (meshFilter != nullptr)
					{
						mesh = meshFilter->mesh();
					}
				}
			}
			else
			{
				auto renderer = go->GetComponent<SkinnedMeshRenderer>();
				if (renderer != nullptr)
				{
					mesh = renderer->sharedMesh();
					if (renderer->m_avatar != nullptr)
					{
						shadow_map_material->EnableKeyword(ShaderKeyword::SkinnedAnimation);
						is_skinned = true;
						Pipeline::UpdateBonesUniforms(renderer->m_matrixPalette);
					}
				}
			}

			if (mesh != nullptr)
			{
				shader->BindUniformMat4("ObjectToWorld", go->transform()->localToWorldMatrix());
				shader->CheckStatus();
				mesh->Render();
				if (!is_skinned)
				{
					shadow_map_material->DisableKeyword(ShaderKeyword::SkinnedAnimation);
				}
			}
		}
#endif
		glDisable(GL_DEPTH_CLAMP);
		Pipeline::PopRenderTarget();
#undef DEBUG_SHADOW
	}

	void Scene::OnDrawGizmos()
	{
		for (auto& go : m_gameObjects) {
			go->OnDrawGizmos();
		}
	}

	void Scene::Destroy(GameObjectPtr obj, const float t /*= 0.0f*/)
	{
		m_gameObjectsToBeDestroyed.push_back(obj);
	}

	void Scene::Destroy(ComponentPtr c, const float t /*= 0.0f*/)
	{
		m_componentsToBeDestroyed.push_back(c);
	}

	void Scene::DestroyImmediate(GameObjectPtr g)
	{
		auto t = g->transform();
		// remove children
		while (!t->m_children.empty())
		{
			auto c = t->m_children.back();
			t->m_children.pop_back();
			DestroyImmediate(c->gameObject());
		}
		t->SetParent(nullptr);  // remove from parent
		t->m_gameObjectStrongRef = nullptr;
		g->m_transform = nullptr;
		m_gameObjects.remove(g);
	}

	void Scene::DestroyImmediate(ComponentPtr c)
	{
		c->gameObject()->RemoveComponent(c);
	}

	GameObjectPtr Scene::Find(const std::string& name)
	{
		for (auto& go : m_gameObjects)
		{
			if (go->name() == name)
			{
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
			auto renderers = go->GetComponentsInChildren<Renderer>();
			for (auto & render : renderers)
			{
				m_bounds.Encapsulate(render->bounds());
			}
		}
	}
	
	GameObjectPtr Scene::IntersectRay(const Ray& ray)
	{
		//if (!m_bounds.IntersectRay(ray))
		//	return nullptr;

		m_bounds = Bounds();

		GameObjectPtr selected = nullptr;
		float tmin = Mathf::Infinity;
		for (auto& go : m_gameObjects)
		{
			auto renderers = go->GetComponentsInChildren<Renderer>();
			for (auto & renderer : renderers)
			{
				Bounds bound = renderer->bounds();
				m_bounds.Encapsulate(bound);
				//float tmin = Mathf::Infinity;
				float t = Mathf::Infinity;
				if (bound.IntersectRay(ray, &t))
				{
					if (t > 0 && t < tmin)
					{
						tmin = t;
						selected = renderer->gameObject();
					}
				}
			}
		}

		return selected;
		
		//GameObjectPtr selected = nullptr;
		//for (auto& go : m_gameObjects)
		//{
		//	//if (go->transform()->parent() == nullptr)
		//	if (true)
		//	{
		//		Bounds bound;
		//		auto rend = go->GetComponent<MeshRenderer>();
		//		if (rend == nullptr)
		//		{
		//			auto srend = go->GetComponent<SkinnedMeshRenderer>();
		//			if (srend == nullptr)
		//				continue;
		//			bound = srend->bounds();
		//		}
		//		else {
		//			bound = rend->bounds();
		//		}
		//		//m_bounds.Encapsulate(bound);

		//		float tmin = Mathf::Infinity;
		//		float t = Mathf::Infinity;
		//		if (bound.IntersectRay(ray, &t))
		//		{
		//			if (t < tmin)
		//			{
		//				tmin = t;
		//				selected = go;
		//			}
		//		}
		//	}
		//}
		//
		return selected;
	}
	
	void Scene::Serialize(std::string const & path)
	{
		//std::ofstream fout(path);
		//YAMLOutputArchive archive(fout);
		//for (auto & go : m_gameObjects)
		//{
		//	archive << go;
		//}
	}
}

