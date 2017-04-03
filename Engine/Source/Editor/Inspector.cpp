#include "Inspector.hpp"

#include <QLayout>
#include <QMenu>
#include <QTreeWidget>
#include "UI/InspectorWidget.hpp"

#include <Debug.hpp>
#include <GameObject.hpp>
#include <Transform.hpp>
#include <Component.hpp>
#include <LayerMask.hpp>
#include <ReflectClass.hpp>
#include <ReflectEnum.hpp>
#include <Vector3.hpp>

#include <Camera.hpp>
#include <Light.hpp>
#include <MeshFilter.hpp>
#include <Mesh.hpp>
#include <MeshRenderer.hpp>
#include <SkinnedMeshRenderer.hpp>
#include <CameraController.hpp>
#include <Rigidbody.hpp>
#include <Collider.hpp>
#include <BoxCollider.hpp>
#include <SphereCollider.hpp>
#include <CapsuleCollider.hpp>
#include <Shader.hpp>
#include <Material.hpp>

#include "EditorGUI.hpp"
//#include "private/EditorGUI_p.hpp"

#include <ReflectEnum.hpp>
#include <generate/Enum_LightType.hpp>
#include <generate/Enum_ShadowCastingMode.hpp>

#include <TextureImporter.hpp>
#include <generate/Enum_ModelImporterMaterialSearch.hpp>
#include <generate/Enum_ModelImporterNormals.hpp>
#include <generate/Enum_ModelImporterTangents.hpp>

#include <ModelImporter.hpp>
#include <generate/Enum_TextureImporterType.hpp>
#include <generate/Enum_TextureImporterShape.hpp>
#include <generate/Enum_TextureImporterType.hpp>
#include <generate/Enum_TextureWrapMode.hpp>
#include <generate/Enum_FilterMode.hpp>

using namespace FishEngine;
using namespace FishEditor;

InspectorWidget* Inspector::s_inspectorWidget;

std::weak_ptr<FishEngine::Component> FishEditor::Inspector::s_targetComponent;


ComponentPtr componentToBeDestroyed;

template<>
void Inspector::OnInspectorGUI(std::shared_ptr<Transform> const & t)
{
	EditorGUI::FloatField("Instance ID", t->GetInstanceID());
	if (EditorGUI::Vector3Field("Position", &t->m_localPosition))
	{
		t->MakeDirty();
	}

	auto localEulerAngles = t->m_localRotation.eulerAngles();
	if (EditorGUI::Vector3Field("Rotation", &localEulerAngles))
	{
		t->setLocalEulerAngles(localEulerAngles);
	}

	if (EditorGUI::Vector3Field("Scale", &t->m_localScale))
	{
		t->MakeDirty();
	}

	//auto worldPostion = t->position();
	//auto worldEulers = t->eulerAngles();
	//auto worldScale = t->lossyScale();
	//EditorGUI::Vector3Field("World Position", &worldPostion);
	//EditorGUI::Vector3Field("World Eulers", &worldEulers);
	//EditorGUI::Vector3Field("World Scale", &worldScale);
}

template<>
void Inspector::OnInspectorGUI(std::shared_ptr<Camera> const & camera)
{
	EditorGUI::FloatField("Instance ID", camera->GetInstanceID());
	// note: must be *static* array
	static const char* listbox_items[] = {
		"Perspective", "Orthographic"
	};
	int list_item_current = camera->m_orthographic ? 1 : 0;
	EditorGUI::EnumPopup("Projection", &list_item_current, listbox_items, 2);
	if (camera->m_orthographic != (list_item_current == 1))
	{
		camera->setOrthographic(!camera->m_orthographic);
	}

	if (camera->m_orthographic)
	{
		if (EditorGUI::FloatField("Size", &camera->m_orthographicSize))
		{
			camera->m_isDirty = true;
		}
	}
	else
	{
		if (EditorGUI::Slider("Field of View", &camera->m_fieldOfView, 1, 179))
		{
			camera->m_isDirty = true;
		}
	}

	if (EditorGUI::FloatField("Clipping Planes(Near)", &camera->m_nearClipPlane))
	{
		camera->m_isDirty = true;
	}
	if (EditorGUI::FloatField("Clipping Planes(Far)", &camera->m_farClipPlane))
	{
		camera->m_isDirty = true;
	}
}

template<>
void Inspector::OnInspectorGUI(const FishEngine::LightPtr& light)
{
	EditorGUI::FloatField("Instance ID", light->GetInstanceID());
	EditorGUI::EnumPopup<LightType>("Type", &light->m_type);
	if (light->m_type != LightType::Directional)
	{
		EditorGUI::FloatField("Range", &light->m_range);
	}
	if (light->m_type == LightType::Spot)
	{
		EditorGUI::Slider("Spot Angle", &light->m_spotAngle, 1, 179);
	}
	EditorGUI::ColorField("Color",         &light->m_color);
	EditorGUI::Slider("Intensity",         &light->m_intensity, 0, 8);
	
	EditorGUI::Slider("Bias",              &light->m_shadowBias, 0, 2);
	EditorGUI::Slider("Normal Bias",       &light->m_shadowNormalBias, 0, 3);
	EditorGUI::Slider("Shadow Near Plane", &light->m_shadowNearPlane, 0.1f, 10.f);
}


template<>
void Inspector::OnInspectorGUI(const FishEngine::MeshFilterPtr& meshFilter)
{
	EditorGUI::FloatField("Instance ID", meshFilter->GetInstanceID());
	EditorGUI::ObjectField("Mesh", meshFilter->m_mesh);
	EditorGUI::FloatField("mesh verts", meshFilter->m_mesh->vertexCount());
	//EditorGUI::FloatField("mesh tris", meshFilter->m_mesh->triangles());
}


template<>
void Inspector::OnInspectorGUI(const FishEngine::MaterialPtr& material)
{
	EditorGUI::FloatField("Instance ID", material->GetInstanceID());
	auto& uniforms = material->m_shader->uniforms();
	for (auto& u : uniforms)
	{
		if (u.type == GL_FLOAT)
		{
			//EditorGUI::Slider(u.name.c_str(), &material->m_uniforms.floats[u.name], 0, 1);
			EditorGUI::FloatField(u.name, &material->m_uniforms.floats[u.name]);
		}
		else if (u.type == GL_FLOAT_VEC3)
		{
			EditorGUI::Vector3Field(u.name, &material->m_uniforms.vec3s[u.name]);
		}
		else if (u.type == GL_FLOAT_VEC4)
		{
			EditorGUI::Vector4Field(u.name, &material->m_uniforms.vec4s[u.name]);
		}
		else if (u.type == GL_SAMPLER_2D)
		{
			//auto& tex = material->m_textures[u.name];
			//ImGui::LabelText(u.name.c_str(), "%s", tex->name().c_str());
			//ImGui::Image((void*)tex->GetNativeTexturePtr(), ImVec2(64, 64));
			//ImGui::SameLine();
			//ImGui::Button("Select");
			auto& tex = material->m_textures[u.name];
			EditorGUI::TextureField(u.name, &tex);
		}
	}
}


template<>
void Inspector::OnInspectorGUI(const FishEngine::RendererPtr& renderer)
{
	EditorGUI::FloatField("Instance ID", renderer->GetInstanceID());
	EditorGUI::EnumPopup("Cast Shadows", &renderer->m_shadowCastingMode);
	EditorGUI::Toggle("Receive Shadows", &renderer->m_receiveShadows);
	for (auto const & material : renderer->m_materials)
	{
		EditorGUI::ObjectField("Material", material);
	}
}

template<>
void Inspector::OnInspectorGUI(const FishEngine::MeshRendererPtr& renderer)
{
	OnInspectorGUI<Renderer>(renderer);
}

template<>
void Inspector::OnInspectorGUI(const std::shared_ptr<Script>& t)
{
	EditorGUI::FloatField("Instance ID", t->GetInstanceID());
}

template<>
void Inspector::OnInspectorGUI(const FishEngine::SkinnedMeshRendererPtr& renderer)
{
	OnInspectorGUI<Renderer>(renderer);
	EditorGUI::ObjectField( "Mesh", renderer->m_sharedMesh );
	EditorGUI::ObjectField( "Root Bone", renderer->m_rootBone.lock() );
}

template<>
void Inspector::OnInspectorGUI(const std::shared_ptr<CameraController>& t)
{
	EditorGUI::FloatField("Instance ID", t->GetInstanceID());
}

template<>
void Inspector::OnInspectorGUI(const FishEngine::RigidbodyPtr& rigidBody)
{
	EditorGUI::FloatField("Instance ID", rigidBody->GetInstanceID());
	EditorGUI::FloatField("Mass", &rigidBody->m_mass);
	EditorGUI::FloatField("Drag", &rigidBody->m_drag);
	EditorGUI::FloatField("Angular", &rigidBody->m_angularDrag);
	EditorGUI::Toggle("Use Gravity", &rigidBody->m_useGravity);
	EditorGUI::Toggle("Is Kinematic", &rigidBody->m_isKinematic);
}

template<>
void Inspector::OnInspectorGUI(const FishEngine::ColliderPtr& collider)
{
	EditorGUI::FloatField("Instance ID", collider->GetInstanceID());
	EditorGUI::Toggle("Is Trigger", &collider->m_isTrigger);
}

template<>
void Inspector::OnInspectorGUI(const FishEngine::BoxColliderPtr& boxCollider)
{
	OnInspectorGUI<Collider>(boxCollider);
	EditorGUI::Vector3Field("Center", &boxCollider->m_center);
	EditorGUI::Vector3Field("Size", &boxCollider->m_size);
}

template<>
void Inspector::OnInspectorGUI(const FishEngine::SphereColliderPtr& collider)
{
	OnInspectorGUI<Collider>(collider);
	EditorGUI::Vector3Field("Center", &collider->m_center);
	EditorGUI::FloatField("Radius", &collider->m_radius);
}

template<>
void Inspector::OnInspectorGUI(const FishEngine::CapsuleColliderPtr& collider)
{
	OnInspectorGUI<Collider>(collider);
	EditorGUI::Vector3Field("Center", &collider->m_center);
	EditorGUI::FloatField("Radius", &collider->m_radius);
	EditorGUI::FloatField("Height", &collider->m_height);
	static const char * direction_enum[] = { "X-Axis", "Y-Axis", "Z-Axis" };
	EditorGUI::EnumPopup("Direction", &collider->m_direction, direction_enum, 3);
}

#if 0

template<>
void Inspector::OnInspectorGUI(TextureImporterPtr const & importer)
{
	EditorGUI::EnumPopup("Texture Type", &importer->m_textureType);
	EditorGUI::EnumPopup("Texture Shape", &importer->m_textureShape);
	EditorGUI::Toggle("Read/Write Enabled", &importer->m_isReadable);
	EditorGUI::Toggle("Generate Mip Maps", &importer->m_mipmapEnabled);
	EditorGUI::EnumPopup("Filter Mode", &importer->m_textureSettings.m_filterMode);
	EditorGUI::EnumPopup("Wrap Mode", &importer->m_textureSettings.m_wrapMode);
}

template<>
void Inspector::OnInspectorGUI(ModelImporterPtr const & importer)
{
	EditorGUI::FloatField("File Scale", &importer->m_fileScale);
	EditorGUI::EnumPopup("Normals", &importer->m_importNormals);
	EditorGUI::EnumPopup("Tangents", &importer->m_importTangents);
	EditorGUI::EnumPopup("Material Search", &importer->m_materialSearch);
	//EditorGUI::Toggle("Read/Write Enabled", &importer->m_);
	//EditorGUI::Toggle("Generate Mip Maps", &importer->m_mipmapEnabled);
	//EditorGUI::EnumPopup("Filter Mode", &importer->m_textureSettings.m_filterMode);
	//EditorGUI::EnumPopup("Wrap Mode", &importer->m_textureSettings.m_wrapMode);
}

#endif

void Inspector::Bind(const GameObjectPtr & go)
{
	if (go == nullptr)
	{
		HideAll();
		return;
	}
	else
	{
		if ( s_inspectorWidget->isHidden() )
			s_inspectorWidget->setHidden(false);
	}
	EditorGUI::s_treeWidget = s_inspectorWidget->m_treeWidget;
	s_inspectorWidget->Bind(go);

	EditorGUI::Begin();
	auto t = go->transform();

	BeginComponentImpl<Transform>(t);

	for (auto const & comp : go->m_components)
	{
		Inspector::BeginComponent(comp);
	}

	//UIHeaderState state;    // ignore state
	// material
	auto renderer = go->GetComponent<Renderer>();
	if ( renderer != nullptr )
	{
		for (auto const & material : renderer->m_materials)
		{
			assert(material != nullptr);
			if ( EditorGUI::BeginMaterial( material->name() ))
			{
				Inspector::OnInspectorGUI<Material>(material);
			}
			EditorGUI::EndMaterial();
		}
	}

	if (componentToBeDestroyed != nullptr)
	{
		Object::DestroyImmediate(componentToBeDestroyed);
		componentToBeDestroyed = nullptr;
	}

	if (EditorGUI::Button("Add Component"))
	{
		//Debug::Log("clicked");
		//auto name = s_inspectorWidget->ShowAddComponentMenu();
		auto const & name = ShowAddComponentMenu();
		AddComponentToGameObject(name, go);
	}

	EditorGUI::End();
}

void Inspector::Bind(FishEngine::ObjectPtr const & object)
{
	if (object == nullptr)
	{
		HideAll();
		return;
	}
	if (object->ClassID() == ClassID<GameObject>())
	{
		Bind(std::dynamic_pointer_cast<GameObject>(object));
	}
	else if (object->ClassID() == ClassID<TextureImporter>())
	{
		Bind(std::dynamic_pointer_cast<TextureImporter>(object));
	}
	else if (object->ClassID() == ClassID<ModelImporter>())
	{
		Bind(std::dynamic_pointer_cast<ModelImporter>(object));
	}
	else // if (object->ClassID() == ClassID<Material>())
	{
		HideAll();
	}
}

void Inspector::Bind(TextureImporterPtr const & importer)
{
	if (importer == nullptr)
	{
		HideAll();
		return;
	}
	else
	{
		if (s_inspectorWidget->isHidden())
			s_inspectorWidget->setHidden(false);
	}

	EditorGUI::s_treeWidget = s_inspectorWidget->m_treeWidget;
	s_inspectorWidget->Bind(importer);

//	EditorGUI::Begin();
//	OnInspectorGUI<TextureImporter>(importer);
//	EditorGUI::End();
}


void FishEditor::Inspector::Bind(ModelImporterPtr const & importer)
{
	if (importer == nullptr)
	{
		HideAll();
		return;
	}
	else
	{
		if (s_inspectorWidget->isHidden())
			s_inspectorWidget->setHidden(false);
	}

	EditorGUI::s_treeWidget = s_inspectorWidget->m_treeWidget;
	s_inspectorWidget->Bind(importer);

//	EditorGUI::Begin();
//	OnInspectorGUI<ModelImporter>(importer);
//	EditorGUI::End();
}

void Inspector::HideAll()
{
//	if (!s_inspectorWidget->isHidden())
//		s_inspectorWidget->setHidden(true);
	s_inspectorWidget->HideAll();
}

std::string Inspector::ShowAddComponentMenu()
{
	static QMenu* menu = nullptr;
	if (menu == nullptr)
	{
		menu = new QMenu(s_inspectorWidget->m_treeWidget);
		menu->addAction(FishEngine::Rigidbody::StaticClassName());
		menu->addAction(FishEngine::MeshFilter::StaticClassName());
		menu->addAction(FishEngine::BoxCollider::StaticClassName());
		menu->addAction(FishEngine::SphereCollider::StaticClassName());
		menu->addAction(FishEngine::MeshRenderer::StaticClassName());
		menu->addAction(FishEngine::Camera::StaticClassName());
		menu->addAction(FishEngine::Light::StaticClassName());
		menu->addAction(FishEngine::CameraController::StaticClassName());
	}

	auto action = menu->exec(QCursor::pos());
	if (action == nullptr)
		return "";
	return action->text().toStdString();
}


QAction* FishEditor::Inspector::ShowComponentMenu()
{
	static QMenu* menu = nullptr;
	if (menu == nullptr)
	{
		menu = new QMenu(s_inspectorWidget->m_treeWidget);
		auto action = menu->addAction("Reset");
		action->setEnabled(false);
		menu->addAction("Revert to Prefab")->setEnabled(false);
		menu->addSeparator();
		menu->addAction("Move to Front")->setEnabled(false);
		menu->addAction("Move to back")->setEnabled(false);
		menu->addAction("Remove Component");
		menu->addAction("Move Up")->setEnabled(false);
		menu->addAction("Move Down")->setEnabled(false);
		menu->addAction("Copy Component")->setEnabled(false);
		menu->addAction("Paste Component As New")->setEnabled(false);
		menu->addAction("Paste Component Value")->setEnabled(false);
		//menu->addAction("Select Material");
	}
	auto action = menu->exec(QCursor::pos());
	return action;
}


#if 1

void Inspector::BeginComponentImpl(const ComponentPtr &component)
{
	LogInfo("Not Implemented for " + component->ClassName());
	UIHeaderState state;
	if ( EditorGUI::BeginComponent( component->ClassName(), &state ) )
	{
		//OnInspectorGUI<T>(std::static_pointer_cast<T>(component));
		//HideRedundantChildItems();
	}
	EditorGUI::EndComponent();

//        if (state == UIHeaderState::remove)
//        {
//            //Object::Destroy(component);
//            componentToBeDestroyed = component;
//        }
}

template<class T, std::enable_if_t<can_be_enabled<T>::value, int>>
void Inspector::BeginComponentImpl(FishEngine::ComponentPtr const & component)
{
	auto p = std::static_pointer_cast<T>(component);
	bool enabled = p->enabled();
	//bool changed = false;
	UIHeaderState state;
	//bool expanded = EditorGUI::BeginComponent( T::StaticClassName(), enabled, state );
	bool expanded = EditorGUI::BeginComponent( component->ClassName(), enabled, &state );
	if ( expanded )
	{
		OnInspectorGUI<T>(p);
	}
	EditorGUI::EndComponent();
	if (state == UIHeaderState::enabledChanged)
	{
		p->setEnabled(!enabled);
	}
	else if (state == UIHeaderState::menuButtonClicked)
	{
		//Object::Destroy(component);
		//componentToBeDestroyed = component;
		auto action = ShowComponentMenu();
		if (action != nullptr && action->text() == "Remove Component")
		{
			//Object::Destroy(component);
			componentToBeDestroyed = component;
		}
	}
}

template<class T, std::enable_if_t<!can_be_enabled<T>::value, int>>
void Inspector::BeginComponentImpl(FishEngine::ComponentPtr const & component)
{
	static_assert(std::is_base_of<Component, T>(), "T must be derived from Component");
	UIHeaderState state;
	if ( EditorGUI::BeginComponent( T::StaticClassName(), &state ) )
	{
		OnInspectorGUI<T>(std::static_pointer_cast<T>(component));
	}
	EditorGUI::EndComponent();

	if (state == UIHeaderState::menuButtonClicked)
	{
		//Object::Destroy(component);
		//componentToBeDestroyed = component;
		auto action = ShowComponentMenu();
		if (action != nullptr && action->text() == "Remove Component")
		{
			 //Object::Destroy(component);
			 componentToBeDestroyed = component;
		}
	}
}

void Inspector::BeginComponent(const ComponentPtr &component)
{
#define CASE(T) case FishEngine::ClassID<T>(): BeginComponentImpl<T>(component); break;
	switch (component->ClassID())
	{
		CASE(Transform)
		CASE(Camera)
		CASE(Light)
		CASE(MeshFilter)
		CASE(MeshRenderer)
		CASE(SkinnedMeshRenderer)
		CASE(CameraController)
		CASE(Rigidbody)
		CASE(BoxCollider)
		CASE(SphereCollider)
		CASE(CapsuleCollider)
		CASE(Script)
		default:
			//Foldout( component->ClassName() );
			BeginComponentImpl(component);
	}
#undef CASE

}


#endif
