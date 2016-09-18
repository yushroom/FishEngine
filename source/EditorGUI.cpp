#include "EditorGUI.hpp"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw_gl3.h>

#include "GameObject.hpp"
#include "Scene.hpp"
#include "Material.hpp"
#include "MeshFilter.hpp"
#include "MeshRenderer.hpp"
#include "Mesh.hpp"
#include "Common.hpp"
#include "Debug.hpp"
#include "RenderSettings.hpp"

NAMESPACE_FISHENGINE_BEGIN

int EditorGUI::m_idCount = 0;
bool EditorGUI::m_showAssectSelectionDialogBox = false;

Material::PMaterial axisIndicatorMaterial;
Mesh::PMesh cubeMesh;
Mesh::PMesh coneMesh;

void EditorGUI::Init()
{
#if FISHENGINE_PLATFORM_WINDOWS
    const std::string root_dir = "../../assets/";
#else
    const std::string root_dir = "/Users/yushroom/program/graphics/RFGL/";
#endif
    const std::string models_dir = root_dir + "models/";
 
    axisIndicatorMaterial = Material::builtinMaterial("VertexLit");
    cubeMesh = Mesh::CreateFromObjFile(models_dir+"cube.obj");
    coneMesh = Mesh::CreateFromObjFile(models_dir+"cone.obj");
}

void EditorGUI::Update()
{
    ImGui::Begin("Inspector");
    
    auto go = Scene::m_activeGameObject;
    if (go != nullptr) {
        ImGui::PushID("Inspector.selected.active");
        ImGui::Checkbox("", &go->m_activeSelf);
        ImGui::PopID();
        char name[32] = {0};
        memcpy(name, go->m_name.c_str(), go->m_name.size());
        name[go->m_name.size()] = 0;
        ImGui::SameLine();
        ImGui::PushID("Inspector.selected.name");
        if (ImGui::InputText("", name, 31)) {
            go->m_name = name;
        }
        ImGui::PopID();
        
        ImGui::LabelText("Tag", "%s", go->tag().c_str());
        //ImGui::SameLine();
        ImGui::LabelText("Layer", "Layer %d", go->m_layer);
        
        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
            go->m_transform.OnEditorGUI();
        }
        
        for (auto& c : go->m_components) {
            if (ImGui::CollapsingHeader(camelCaseToReadable(c->ClassName()).c_str(), ImGuiTreeNodeFlags_DefaultOpen))
                c->OnEditorGUI();
        }
        for (auto& s : go->m_scripts) {
            if (ImGui::CollapsingHeader(camelCaseToReadable(s->ClassName()).c_str(), ImGuiTreeNodeFlags_DefaultOpen))
                s->OnEditorGUI();
        }
    }
    
    ImGui::Button("Add Component");
    ImGui::End(); // Inspector
    
    glClear(GL_DEPTH_BUFFER_BIT);
    DrawSceneGizmo();
    
    // Main menu bar
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            ImGui::MenuItem("New Scene", "Ctrl+N");
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit")) {
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
    
    // Hierarchy view
    ImGui::Begin("Hierarchy");
    if (ImGui::Button("Create")) {
        auto go = Scene::CreateGameObject("GameObject");
    }
    
    if (go != nullptr) {
        ImGui::SameLine();
        if (ImGui::Button("Destroy")) {
            Object::Destroy(go);
            Scene::m_activeGameObject = nullptr;
        }
    }
    m_idCount = 0;
    ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, ImGui::GetFontSize()); // Increase spacing to differentiate leaves from expanded contents.
    for (auto go : Scene::m_gameObjects) {
        if (go->transform()->parent() == nullptr) {
            HierarchyItem(go.get());
        }
    }
    ImGui::PopStyleVar();
    ImGui::End();
    
    
    // Project view
    ImGui::Begin("Project");
    ImGui::End();
    
//    if (m_showAssectSelectionDialogBox) {
//        ImGui::OpenPopup("Select ...");
//        ImGuiWindowFlags window_flags = 0;
//        window_flags |= ImGuiWindowFlags_NoCollapse;
//        if (ImGui::BeginPopupModal("Select ...", &m_showAssectSelectionDialogBox, window_flags)) {
//            for (auto m : Mesh::m_meshes) {
//                if (ImGui::Button(m->name().c_str())) {
//                    Debug::Log("%s", m->name().c_str());
//                }
//            }
//            ImGui::Separator();
//            ImGui::EndPopup();
//        }
//    }
    
    ImGui::Render();
}

void EditorGUI::Clean()
{
    ImGui_ImplGlfwGL3_Shutdown();
}

//void GUI::OnKey(int key, int action)
//{
//}
//
//void GUI::OnWindowSizeChanged(int width, int height)
//{
//}
//
//void GUI::OnMouse(double xpos, double ypos)
//{
//}
//
//bool GUI::OnMouseButton(int button, int action)
//{
//    return false;
//}
//
//bool GUI::OnMouseScroll(double yoffset)
//{
//    return false;
//}


void EditorGUI::SelectMeshDialogBox(std::function<void(std::shared_ptr<Mesh>)> callback)
{
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse;
    bool is_open = true;
    if (ImGui::BeginPopupModal("Select ...", &is_open, window_flags)) {
        for (auto m : Mesh::m_meshes) {
            if (ImGui::Button(m->name().c_str())) {
                Debug::Log("%s", m->name().c_str());
                //SetMesh(m);
                callback(m);
                ImGui::CloseCurrentPopup();
            }
        }
        ImGui::Separator();
        ImGui::EndPopup();
    }
}


void EditorGUI::HierarchyItem(GameObject* gameObject) {
    ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ((gameObject == Scene::m_activeGameObject)? ImGuiTreeNodeFlags_Selected : 0);
    
    bool is_leaf = (gameObject->transform()->childCount() == 0);
    if (is_leaf) {// no child
        node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
    }
    if (!gameObject->activeSelf()) {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.20f, 0.20f, 0.20f, 1.00f));
    }
    bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)m_idCount, node_flags, "%s", gameObject->name().c_str());

    if (ImGui::IsItemClicked()) {
        Scene::SelectGameObject(gameObject);
    }
    // child node
    if (!is_leaf) {
        if (node_open) {
            for (auto t : gameObject->transform()->m_children)
                HierarchyItem(t->gameObject());
            ImGui::TreePop();
        }
    }
    m_idCount ++;
    if (!gameObject->activeSelf()) {
        ImGui::PopStyleColor();
    }
}


void EditorGUI::DrawSceneGizmo()
{
    int w = RenderSystem::width();
    int h = RenderSystem::height();
    glViewport(0, 0, w*0.1f, h*0.1f);
    
    auto shader = axisIndicatorMaterial->shader();
    shader->Use();
    axisIndicatorMaterial->SetVector3("unity_LightPosition", Vector3(0, 0, 1));
    auto camera = Scene::mainCamera();
    //auto view = camera->worldToCameraMatrix();
    auto view = glm::lookAt(Vector3(0, 0, 5), Vector3(0, 0, 0), Vector3(0, 1, 0));
    auto proj = camera->projectionMatrix();
    auto vp = proj * view;
    auto model = glm::mat4(glm::inverse(camera->transform()->rotation()));
    
    ShaderUniforms uniforms;
    axisIndicatorMaterial->SetMatrix("MATRIX_MVP", vp*model);
    axisIndicatorMaterial->SetMatrix("MATRIX_IT_MV", view*model);
    axisIndicatorMaterial->SetVector3("_Color", Vector3(1, 1, 1));
    shader->PreRender();
    axisIndicatorMaterial->Update();
    shader->CheckStatus();
    cubeMesh->Render();

    auto s = glm::scale(glm::mat4(), Vector3(0.5f, 0.75f, 0.5f));
    
    float f[] = {
        -1,  0,  0,    0, 0, -90,
         0, -1,  0,    0, 0,   0,
         0,  0, -1,   90, 0,   0,
         1,  0,  0,    0, 0,  90,
         0,  1,  0,  180, 0,   0,
         0,  0,  1,  -90, 0,   0,
    };
    static Transform t;
    for (int i = 0; i < 6; ++i) {
        int j = i*6;
        t.setLocalPosition(f[j], f[j+1], f[j+2]);
        t.setLocalEulerAngles(f[j+3], f[j+4], f[j+5]);
        //t.Update();
        auto modelMat = model * t.localToWorldMatrix() * s;
        axisIndicatorMaterial->SetMatrix("MATRIX_MVP", vp*modelMat);
        axisIndicatorMaterial->SetMatrix("MATRIX_IT_MV", glm::inverse(glm::transpose(view*modelMat)));
        if (i >= 3)
            axisIndicatorMaterial->SetVector3("_Color", Vector3(f[j], f[j+1], f[j+2]));
        axisIndicatorMaterial->Update();
        coneMesh->Render();
    }
    
    shader->PostRender();
    
    auto v = camera->m_viewport;
    glViewport(w*v.x, h*v.y, w*v.z, h*v.w);
}


NAMESPACE_FISHENGINE_END
