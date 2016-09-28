#include "EditorGUI.hpp"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw_gl3.h>
#include <imgui/imgui_internal.h>

#include "GameObject.hpp"
#include "Scene.hpp"
#include "Material.hpp"
#include "MeshFilter.hpp"
#include "MeshRenderer.hpp"
#include "Mesh.hpp"
#include "Common.hpp"
#include "Debug.hpp"
#include "Selection.hpp"
#include "EditorRenderSystem.hpp"
#include <Time.hpp>
#include <sstream>
#include <Matrix4x4.hpp>
#include <Bounds.hpp>
#include <Ray.hpp>
#include <ModelImporter.hpp>

#include <imgui/imgui_dock.h>

using namespace FishEngine;

NAMESPACE_FISHEDITOR_BEGIN

constexpr float translate_gizmo_length = 0.1f;

TransformToolType EditorGUI::m_transformToolType = TransformToolType::Translate;
int EditorGUI::m_idCount = 0;


int EditorGUI::m_selectedAxis = -1;

std::weak_ptr<FishEngine::GameObject> FishEditor::EditorGUI::m_lastSelectedGameObject;

bool EditorGUI::s_isAnyItemClicked = false;
bool EditorGUI::m_showAssectSelectionDialogBox = false;

Material::PMaterial sceneGizmoMaterial = nullptr;
Mesh::PMesh cubeMesh = nullptr;
Mesh::PMesh coneMesh = nullptr;

void EditorGUI::Init()
{
#if FISHENGINE_PLATFORM_WINDOWS
    const std::string root_dir = "../../assets/";
#else
    const std::string root_dir = "/Users/yushroom/program/graphics/FishEngine/assets/";
#endif
 
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF((root_dir+"fonts/DroidSans.ttf").c_str(), 14.0f);
    
    ImGuiContext& g = *GImGui;
    ImGuiStyle& style = g.Style;
    style.FrameRounding = 4.f;
    style.WindowRounding = 0.f;
    //style.Colors[ImGuiCol_Text] = ImVec4(0, 0, 0, 1);
    //style.Colors[ImGuiCol_Button] = ImVec4(171/255.f, 204/255.f, 242/255.f, 1.f);
    //style.Colors[ImGuiCol_WindowBg] = ImVec4(0.8f, 0.8f, 0.8f, 1.f);
    //style.GrabRounding = 0.f;
    //style.WindowTitleAlign = ImGuiAlign_Left | ImGuiAlign_VCenter;
    style.WindowMinSize = ImVec2(256, 256);
    
    sceneGizmoMaterial = Material::builtinMaterial("VertexLit");
    cubeMesh = Model::builtinModel(BuiltinModelTyep::Cube)->mainMesh();
    coneMesh = Model::builtinModel(BuiltinModelTyep::Cone)->mainMesh();
}

void EditorGUI::Update()
{
    auto selectedGO = Selection::activeGameObject();

    if (Input::GetKeyDown(KeyCode::F)) {
        Camera::main()->FrameSelected(selectedGO);
    }

    glClear(GL_DEPTH_BUFFER_BIT);
    DrawSceneGizmo();
    if (selectedGO != nullptr)
    {
        if (m_transformToolType == TransformToolType::Translate)
            DrawTranslateGizmo();
        else if (m_transformToolType == TransformToolType::Rotate)
            DrawRotateGizmo();
        else if (m_transformToolType == TransformToolType::Scale)
            DrawScaleGizmo();
    }

    //ImGuiContext& g = *GImGui;
    //ImGuiStyle& style = g.Style;
    
    //ImGui::BeginDock("Dock");
    //ImGui::EndDock();
    
    DrawMainMenu();
    DrawHierarchyWindow();
    DrawInspectorWindow();
    
    // Project view
    //ImGui::BeginDock("Project");
    //ImGui::EndDock();
    
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

bool EditorGUI::OnMouseButton(MouseButtonCode button, MouseButtonState action)
{
    if (button == MouseButtonCode::Left &&
        action == MouseButtonState::Down)
    {
        auto selectedGO = Selection::activeGameObject();
        if (selectedGO == nullptr)
            return false;
        
        Vector3 center = selectedGO->transform()->position();
        Vector3 camera_pos = Camera::main()->transform()->position();
        center = Vector3::Normalize(center-camera_pos) + camera_pos;
        
        constexpr float d = 0.03f;
        Bounds aabb[3];
        for (int i = 0; i < 3; ++i) {
            Vector3 v(0, 0, 0);
            v[i] = translate_gizmo_length/2.f;
            aabb[i].setCenter(center + v);
            v.Set(d, d, d);
            v[i] = translate_gizmo_length;
            aabb[i].setSize(v);
        }

        m_selectedAxis = -1;
        auto ray = Camera::main()->ScreenPointToRay(Input::mousePosition());
        for (int i = 0; i < 3; ++i) {
            if (aabb[i].IntersectRay(ray)) {
                //Debug::Log("%c axis", "xyz"[i]);
                m_selectedAxis = i;
                return true;
            }
        }
    }
    return false;
}

//bool GUI::OnMouseScroll(double yoffset)
//{
//    return false;
//}

bool EditorGUI::Button(const char* text)
{
    ImGuiContext& g = *GImGui;
    ImGuiStyle& style = g.Style;
    float w = ImGui::GetWindowWidth() - style.WindowPadding.x * 2.f;
    return ImGui::Button(text, ImVec2(w, 0));
}

void EditorGUI::Matrix4x4(const std::string& label, FishEngine::Matrix4x4& mat)
{
    ImGui::InputFloat4((label+"##row1").c_str(), mat.rows[0].data());
    ImGui::InputFloat4((label+"##row2").c_str(), mat.rows[1].data());
    ImGui::InputFloat4((label+"##row3").c_str(), mat.rows[2].data());
    ImGui::InputFloat4((label+"##row4").c_str(), mat.rows[3].data());
}

void EditorGUI::SelectMeshDialogBox(std::function<void(std::shared_ptr<Mesh>)> callback)
{
//    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse;
//    bool is_open = true;
//    if (ImGui::BeginPopupModal("Select ...", &is_open, window_flags)) {
//        for (auto m : Mesh::m_meshes) {
//            if (ImGui::Button(m->name().c_str())) {
//                Debug::Log("%s", m->name().c_str());
//                //SetMesh(m);
//                callback(m);
//                ImGui::CloseCurrentPopup();
//            }
//        }
//        ImGui::Separator();
//        ImGui::EndPopup();
//    }
}

void EditorGUI::HierarchyItem(std::shared_ptr<GameObject> gameObject)
{
    ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow;
    if (gameObject == Selection::activeGameObject())
        node_flags |= ImGuiTreeNodeFlags_Selected;
    
    bool is_leaf = (gameObject->transform()->childCount() == 0);
    if (is_leaf) {// no children
        node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
    }
    if (!gameObject->activeSelf()) {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.20f, 0.20f, 0.20f, 1.00f));
    }
    bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)m_idCount, node_flags, "%s", gameObject->name().c_str());

    if (ImGui::IsItemClicked()) {
        Debug::Log("select: %s", gameObject->name().c_str());
        Selection::setActiveGameObject(gameObject);
        s_isAnyItemClicked = true;
    }
    // child node
    if (!is_leaf) {
        if (node_open) {
            for (auto t : gameObject->transform()->m_children)
                HierarchyItem(t.lock()->gameObject());
            ImGui::TreePop();
        }
    }
    m_idCount ++;
    if (!gameObject->activeSelf()) {
        ImGui::PopStyleColor();
    }
}

void FishEditor::EditorGUI::DrawHierarchyWindow()
{
    s_isAnyItemClicked = false;
    auto selectedGO = Selection::activeGameObject();
    // Hierarchy view
    //ImGui::BeginDock("Hierarchy");
    ImGui::Begin("Hierarchy");
    if (ImGui::Button("Create")) {
        auto go = Scene::CreateGameObject("GameObject");
        if (Selection::activeGameObject() != nullptr) {
            go->transform()->SetParent(Selection::activeGameObject()->transform());
        }
    }

    if (selectedGO != nullptr) {
        ImGui::SameLine();
        if (ImGui::Button("Destroy")) {
            Object::Destroy(selectedGO);
            //Selection::setActiveGameObject(nullptr);
        }
    }
    m_idCount = 0;
    ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, ImGui::GetFontSize()); // Increase spacing to differentiate leaves from expanded contents.
    for (auto& go : Scene::m_gameObjects) {
        if (go->transform()->parent() == nullptr) {
            HierarchyItem(go);
        }
    }
    ImGui::PopStyleVar();

    if (!s_isAnyItemClicked && ImGui::IsMouseClicked(0) && ImGui::IsMouseHoveringWindow()) {
        Debug::Log("Mouse Clicked here");
        Selection::setActiveGameObject(nullptr);
    }

    //ImGui::EndDock();
    ImGui::End();

}

void FishEditor::EditorGUI::DrawInspectorWindow()
{
    auto selectedGO = Selection::activeGameObject();

    // Inspector Editor
    //ImGui::BeginDock("Inspector", nullptr);
    ImGui::Begin("Inspector", nullptr);
    ImGui::PushItemWidth(ImGui::GetWindowWidth()*0.55f);
    if (selectedGO != nullptr) {
        ImGui::PushID("Inspector.selected.active");
        ImGui::Checkbox("", &selectedGO->m_activeSelf);
        ImGui::PopID();
        char name[128] = { 0 };
        memcpy(name, selectedGO->name().c_str(), selectedGO->name().size());
        name[selectedGO->m_name.size()] = 0;
        ImGui::SameLine();
        ImGui::PushID("Inspector.selected.name");
        if (ImGui::InputText("", name, 127)) {
            selectedGO->m_name = name;
        }
        ImGui::PopID();

        ImGui::PushItemWidth(ImGui::GetWindowWidth()*0.3f);
        //ImGui::LabelText("", "Tag");
        ImGui::Text("Tag");
        ImGui::SameLine();
        ImGui::LabelText("##Tag", "%s", selectedGO->tag().c_str());
        ImGui::SameLine();
        ImGui::Text("Layer");
        ImGui::SameLine();
        ImGui::LabelText("##Layer", "Layer %d", selectedGO->m_layer);
        ImGui::PopItemWidth();

        if (ImGui::CollapsingHeader("Transform##header", ImGuiTreeNodeFlags_DefaultOpen)) {
            selectedGO->m_transform->OnInspectorGUI();
        }

        for (auto& c : selectedGO->m_components) {
            bool is_open = true;
            if (ImGui::CollapsingHeader((camelCaseToReadable(c->ClassName()) + "##header").c_str(), &is_open, ImGuiTreeNodeFlags_DefaultOpen))
                c->OnInspectorGUI();
            if (!is_open) {
                Object::Destroy(c);
            }
        }
        for (auto& s : selectedGO->m_scripts) {
            bool is_open = true;
            if (ImGui::CollapsingHeader((camelCaseToReadable(s->ClassName()) + "##header").c_str(), &is_open, ImGuiTreeNodeFlags_DefaultOpen))
                s->OnInspectorGUI();
            if (!is_open) {
                Object::Destroy(s);
            }
        }
    }

    EditorGUI::Button("Add Component");

    //ImGui::EndDock(); // Inspector Editor
    ImGui::End();
}


void FishEditor::EditorGUI::DrawMainMenu()
{
    static float time_stamp = 0;

    // Main menu bar
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            ImGui::MenuItem("New Scene", "Ctrl+N");
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit")) {
            ImGui::EndMenu();
        }

        double new_time = glfwGetTime();
        int fps = (int)roundf(1.f / float(new_time - time_stamp));
        time_stamp = new_time;
        std::ostringstream sout;
        sout << "FPS: " << fps;
        const char* s = sout.str().c_str();
        auto fps_stats_size = ImGui::CalcTextSize(s);
        ImGui::SameLine(ImGui::GetContentRegionMax().x - fps_stats_size.x);
        ImGui::Text("%s", s);
        ImGui::EndMainMenuBar();
    }

}

void FishEditor::EditorGUI::DrawTranslateGizmo()
{
    auto selectedGO = Selection::activeGameObject();
    if (m_lastSelectedGameObject.lock() != selectedGO) {
        m_selectedAxis = -1;
        m_lastSelectedGameObject = selectedGO;
    }
    if (selectedGO == nullptr)
        return;

    auto shader = sceneGizmoMaterial->shader();
    shader->Use();
    //auto camera_pos = Vector3(0, 0, -5);
    sceneGizmoMaterial->SetVector3("unity_LightPosition", Vector3(0, 0, -1));
    auto camera = Camera::main();
    auto view = camera->worldToCameraMatrix();
    auto proj = camera->projectionMatrix();
    auto vp = proj * view;

    ShaderUniforms uniforms;

    static Transform t;
    //t.setLocalScale(Vector3(1, 1.5, 1) * 0.15f);

    float f[] = {
        1,  0,  0,   0, 0, -90,    // red axis, +x
        0,  1,  0,   0, 0,   0,    // green axis, +y
        0,  0,  1,  90, 0,   0,    // blue axis, +z
    };

    Vector3 center = selectedGO->transform()->position();
    Vector3 camera_pos = camera->transform()->position();
    Vector3 dir = center - camera_pos;
    float distance = dir.magnitude();
    center = dir.normalized() + camera_pos;
    
    for (int i = 0; i < 3; ++i) {
        int j = 6 * i;
        //t.setLocalScale(0.1f, 0.1f, 0.1f);
        t.setLocalScale(Vector3(1, 1.5, 1) * 0.015f);
        t.setLocalEulerAngles(f+j+3);
        Vector3 pos = center + Vector3(f+j)*translate_gizmo_length;
        t.setLocalPosition(pos);
        auto model = t.localToWorldMatrix();
        sceneGizmoMaterial->SetMatrix("MATRIX_MVP", vp*model);
        sceneGizmoMaterial->SetMatrix("MATRIX_IT_MV", view*model);
        Vector3 color = m_selectedAxis == i ? Vector3(1, 1, 0) : Vector3(f+j);
        sceneGizmoMaterial->SetVector3("_Color", color);
        shader->PreRender();
        sceneGizmoMaterial->Update();
        shader->CheckStatus();
        coneMesh->Render();
        
        Vector3 trans = center;
        trans[i] += translate_gizmo_length * 0.5f;
        t.setLocalPosition(trans);
        Vector3 scale = Vector3::one*0.002f;
        scale[i] = translate_gizmo_length;
        t.setLocalScale(scale);
        t.setLocalEulerAngles(0, 0, 0);
        model = t.localToWorldMatrix();
        sceneGizmoMaterial->SetMatrix("MATRIX_MVP", vp*model);
        sceneGizmoMaterial->SetMatrix("MATRIX_IT_MV", view*model);
        sceneGizmoMaterial->Update();
        cubeMesh->Render();
    }

    if (m_selectedAxis < 0)
        return;

    if (Input::GetMouseButton(0))
    {
        float m_dragSpeed = distance / Camera::main()->nearClipPlane() * 0.5f;
        float x = m_dragSpeed * Input::GetAxis(Axis::MouseX);
        float y = m_dragSpeed * Input::GetAxis(Axis::MouseY);
        Vector3 mouse_movement(x, y, 0);
        Vector3 axis_base(0, 0, 0);
        axis_base[m_selectedAxis] = 1.f;
        Vector3 axis_base_view = vp * Vector4(axis_base, 0);
        float d = Vector3::Dot(mouse_movement, axis_base_view.normalized());
        selectedGO->transform()->Translate(axis_base * d, Space::World);
    }
}


void FishEditor::EditorGUI::DrawRotateGizmo()
{

}


void FishEditor::EditorGUI::DrawScaleGizmo()
{

}

void EditorGUI::DrawSceneGizmo()
{
    int w = int(Screen::width()*0.06f);
    int margin = 20;
    glViewport(margin, margin, GLsizei(w), GLsizei(w));
    
    auto shader = sceneGizmoMaterial->shader();
    shader->Use();
    
    Vector3 camera_pos(0, 0, -5);
    sceneGizmoMaterial->SetVector3("unity_LightPosition", camera_pos.normalized());
    auto camera = Camera::main();
    auto view = Matrix4x4::LookAt(camera_pos, Vector3(0, 0, 0), Vector3(0, 1, 0));
    auto proj = camera->m_orthographic ? Matrix4x4::Ortho(-2, 2, -2, 2, 1, 10) : Matrix4x4::Perspective(45, 1, 0.3f, 10.f);
    auto vp = proj * view;
    auto model = FishEngine::Matrix4x4::FromRotation(Quaternion::Inverse(camera->transform()->rotation()));

    auto inv_model = model.inverse();
    
    auto mousePos = Input::mousePosition();
    bool inRegion = false;
    float x = mousePos.x - margin;
    float y = mousePos.y - margin;
    Ray ray(Vector3::zero, Vector3::zero);
    if (x > 0 && x < w && y > 0 && y < w) {
        inRegion = true;
        //Debug::Log("In SceneGizmo region");
        //ray = Ray(camera_pos, ray_world.normalized()); // world space
        ray.origin = inv_model * Vector4(2.0f*x/w-1.f, 2.0f*y/w-1.0f, -5, 1);
        ray.direction = inv_model * Vector4(0, 0, 1, 0);
    }
    
    auto s = Matrix4x4::Scale(0.5f, 0.75f, 0.5f);
    
    float f[] = {
        -1,  0,  0,    0, 0, -90,
         0, -1,  0,    0, 0,   0,
         0,  0, -1,   90, 0,   0,
         1,  0,  0,    0, 0,  90,
         0,  1,  0,  180, 0,   0,
         0,  0,  1,  -90, 0,   0,
    };
    
    ShaderUniforms uniforms;
    bool interested = false;
    int hoverIndex = -1;
    
    static Transform t;
    for (int i = 0; i < 6; ++i) {
        int j = i*6;
        Vector3 pos(f+j);
        Vector3 color(1, 1, 1);
        if (i >= 3)
            color = Vector3(f+j);
        if (inRegion && !interested) {
            Bounds aabb(pos*0.7f, Vector3::one*0.5f);
            if (aabb.IntersectRay(ray)) {
                interested = true;
                hoverIndex = i;
                color.Set(1, 1, 0); // yellow
            }
        }
        t.setLocalPosition(pos);
        t.setLocalEulerAngles(f[j+3], f[j+4], f[j+5]);
        auto modelMat = model * t.localToWorldMatrix() * s;
        sceneGizmoMaterial->SetMatrix("MATRIX_MVP", vp*modelMat);
        sceneGizmoMaterial->SetMatrix("MATRIX_IT_MV", (view * modelMat).transpose().inverse());
        //if (i >= 3)
        sceneGizmoMaterial->SetVector3("_Color", color);
        sceneGizmoMaterial->Update();
        coneMesh->Render();
    }
    
    Bounds aabb(Vector3::zero, Vector3::one*0.5f);
    bool interested6 = false;
    if (inRegion && !interested) {
        interested6 = aabb.IntersectRay(ray);
        if (interested6)
            hoverIndex = 6;
    }
    interested = interested || interested6;
    
    sceneGizmoMaterial->SetMatrix("MATRIX_MVP", vp*model);
    sceneGizmoMaterial->SetMatrix("MATRIX_IT_MV", view*model);
    sceneGizmoMaterial->SetVector3("_Color", interested6 ? Vector3(1, 1, 0) : Vector3(1, 1, 1));
    shader->PreRender();
    sceneGizmoMaterial->Update();
    shader->CheckStatus();
    cubeMesh->Render();
    
    shader->PostRender();

    if (interested && Input::GetMouseButtonDown(0))
    {
        Debug::Log("%d", hoverIndex);
        if (hoverIndex == 6) {
            camera->setOrthographic(!camera->orghographic());
            //camera->m_orthographic = !camera->m_orthographic;
        } else {
            Vector3 offset(f + hoverIndex * 6);
            Vector3 up(0, 1, 0);
            if (offset.y == 1) {
                up.Set(0, 0, -1);
            } else if (offset.y == -1) {
                up.Set(0, 0, 1);
            }
            camera->transform()->setLocalPosition(camera->m_focusPoint + offset * 4);
            camera->transform()->LookAt(camera->m_focusPoint, up);
            //camera->transform()->setLocalEulerAngles(Vector3(f + hoverIndex * 6 + 3));
            //camera->FrameSelected(Selection::activeGameObject());
        }
    }
    
    auto v = camera->viewport();
    w = Screen::width();
    int h = Screen::height();
    glViewport(GLint(w*v.x), GLint(h*v.y), GLsizei(w*v.z), GLsizei(h*v.w));
}


NAMESPACE_FISHEDITOR_END
