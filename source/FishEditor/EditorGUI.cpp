#include "EditorGUI.hpp"

#include <sstream>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw_gl3.h>
#include <imgui/imgui_internal.h>
//#include <imgui/imgui_dock.h>
#include <boost/lexical_cast.hpp>

#include <GameObject.hpp>
#include <Scene.hpp>
#include <Material.hpp>
#include <MeshFilter.hpp>
#include <MeshRenderer.hpp>
#include <Mesh.hpp>
#include <Common.hpp>
#include <Debug.hpp>
#include <Time.hpp>
#include <Matrix4x4.hpp>
#include <Bounds.hpp>
#include <Ray.hpp>
#include <ModelImporter.hpp>
#include <Gizmos.hpp>
#include <BoxCollider.hpp>
#include <SphereCollider.hpp>
#include <CapsuleCollider.hpp>
#include <Rigidbody.hpp>
#include <Pipeline.hpp>
#include <SkinnedMeshRenderer.hpp>
#include <Camera.hpp>
#include <Component_gen.hpp>
#include <Light.hpp>

#include "FishEditorWindow.hpp"
#include "Selection.hpp"
#include "EditorRenderSystem.hpp"


using namespace FishEngine;

namespace FishEditor
{
    constexpr float inspector_indent_width              = 4;
    constexpr float menuBarHeight = 20;

    TransformToolType EditorGUI::m_transformToolType    = TransformToolType::Translate;
    TransformSpace    EditorGUI::m_transformSpace       = TransformSpace::Global;
    int     EditorGUI::m_idCount                        = 0;
    bool    EditorGUI::s_locked                         = false;
    int     EditorGUI::m_selectedAxis                   = -1;
    bool    EditorGUI::s_isAnyItemClicked               = false;
    bool    EditorGUI::s_openMenuPopup                  = false;
    bool    EditorGUI::m_showAssectSelectionDialogBox   = false;

    bool    EditorGUI::s_windowResized                  = true;
    bool    EditorGUI::s_mouseEventHandled              = false;

    std::weak_ptr<FishEngine::GameObject> FishEditor::EditorGUI::m_lastSelectedGameObject;
    PMaterial sceneGizmoMaterial = nullptr;
    PMesh cubeMesh = nullptr;
    PMesh coneMesh = nullptr;

    ImVec2 inspectorWindowPos;
    ImVec2 inspectorWindowSize;
    ImVec2 hierarchyWindowPos;
    ImVec2 hierarchyWindowSize;
    ImVec2 projectWindowPos;
    ImVec2 projectWindowSize;

    ImGuiWindowFlags globalWindowFlags = 0;
    
    const char* ToString(TransformSpace& space)
    {
        if (space == TransformSpace::Global)
            return "Global";
        else
            return "Local";
    }

    void EditorGUI::Init()
    {
#if FISHENGINE_PLATFORM_WINDOWS
        const std::string root_dir = "../../assets/";
#else
        const std::string root_dir = "/Users/yushroom/program/graphics/FishEngine/assets/";
#endif

        ImGuiIO& io = ImGui::GetIO();
        io.Fonts->AddFontFromFileTTF((root_dir + "fonts/DroidSans.ttf").c_str(), 14.0f);
        io.IniFilename = nullptr;

        ImGuiContext& g = *GImGui;
        ImGuiStyle& style = g.Style;
        style.FrameRounding = 4.f;
        style.WindowRounding = 0.f;
        style.Colors[ImGuiCol_Text]         = ImVec4(0, 0, 0, 1);
        style.Colors[ImGuiCol_Button]       = ImVec4(171/255.f, 204/255.f, 242/255.f, 1.f);
        style.Colors[ImGuiCol_ButtonHovered] = ImVec4(190 / 255.f, 224 / 255.f, 262 / 255.f, 1.f);
        style.Colors[ImGuiCol_ButtonActive] = ImVec4(181 / 255.f, 214 / 255.f, 232 / 255.f, 1.f);
        style.Colors[ImGuiCol_WindowBg]     = ImVec4(0.8f, 0.8f, 0.8f, 0.6f);
        style.Colors[ImGuiCol_MenuBarBg]    = ImVec4(0.8f, 0.8f, 0.8f, 1.0f);
        style.Colors[ImGuiCol_TitleBg]      = ImVec4(0.5f, 0.5f, 0.5f, 0.8f);
        style.Colors[ImGuiCol_PopupBg]      = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
        style.Colors[ImGuiCol_ComboBg]      = ImVec4(0.6f, 0.6f, 0.6f, 1.0f);
        //style.GrabRounding = 0.f;
        //style.WindowTitleAlign = ImGuiAlign_Left | ImGuiAlign_VCenter;
        style.WindowMinSize = ImVec2(128, 128);

        sceneGizmoMaterial = Material::builtinMaterial("VertexLit");
        cubeMesh = Model::builtinModel(PrimitiveType::Cube)->mainMesh();
        coneMesh = Model::builtinModel(PrimitiveType::Cone)->mainMesh();

        globalWindowFlags |= ImGuiWindowFlags_NoCollapse;
        //globalWindowFlags |= ImGuiWindowFlags_NoResize;
        globalWindowFlags |= ImGuiWindowFlags_ShowBorders;
        CalculateWindowSizeAndPosition();
    }


    void EditorGUI::Update()
    {
        s_mouseEventHandled = false;
        auto selectedGO = Selection::selectedGameObjectInHierarchy();

        if (Input::GetKeyDown(KeyCode::F)) {
            Camera::main()->FrameSelected(selectedGO);
        }

        glClear(GL_DEPTH_BUFFER_BIT);
        DrawSceneGizmo();
        
        if (Input::GetKeyDown(KeyCode::W))
        {
            m_transformToolType = TransformToolType::Translate;
        }
        else if (Input::GetKeyDown(KeyCode::E))
        {
            m_transformToolType = TransformToolType::Rotate;
        }
        else if (Input::GetKeyDown(KeyCode::R))
        {
            m_transformToolType = TransformToolType::Scale;
        }
        
        if (selectedGO != nullptr)
        {
            if (m_transformToolType == TransformToolType::Translate)
                DrawTranslateGizmo();
            else if (m_transformToolType == TransformToolType::Rotate)
                DrawRotateGizmo();
            else if (m_transformToolType == TransformToolType::Scale)
                DrawScaleGizmo();
        }

        DrawMainMenu();
        //DrawToolBar();
        DrawHierarchyWindow();
        DrawInspectorWindow();
        DrawProjectWindow();

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
        s_windowResized = false;
        
        if (!s_mouseEventHandled && Input::GetMouseButtonDown(0))
        {
            Ray ray = Camera::main()->ScreenPointToRay(Input::mousePosition());
            auto go = Scene::IntersectRay(ray);
            Selection::setSelectedGameObjectInHierarchy(go);
        }
    }

    void EditorGUI::Clean()
    {
        ImGui_ImplGlfwGL3_Shutdown();
    }

    bool EditorGUI::Button(const char* text)
    {
        ImGuiContext& g = *GImGui;
        ImGuiStyle& style = g.Style;
        float w = ImGui::GetWindowWidth() - style.WindowPadding.x * 2.f;
        return ImGui::Button(text, ImVec2(w, 0));
    }

    void EditorGUI::Matrix4x4(const std::string& label, FishEngine::Matrix4x4& mat)
    {
        ImGui::InputFloat4((label + "##row1").c_str(), mat.rows[0].data());
        ImGui::InputFloat4((label + "##row2").c_str(), mat.rows[1].data());
        ImGui::InputFloat4((label + "##row3").c_str(), mat.rows[2].data());
        ImGui::InputFloat4((label + "##row4").c_str(), mat.rows[3].data());
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

    void EditorGUI::OnWindowSizeChanged(const int width, const int height)
    {
        CalculateWindowSizeAndPosition();
    }

    void EditorGUI::HierarchyItem(std::shared_ptr<GameObject> gameObject)
    {
        ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow;
        if (gameObject == Selection::selectedGameObjectInHierarchy())
        {
            node_flags |= ImGuiTreeNodeFlags_Selected;
        }

        bool is_leaf = (gameObject->transform()->childCount() == 0);
        if (is_leaf)
        {
            // no children
            node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
        }

        if (!gameObject->activeSelf())
        {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.20f, 0.20f, 0.20f, 1.00f));
        }

        bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)m_idCount, node_flags, "%s", gameObject->name().c_str());

        if (ImGui::IsItemClicked())
        {
            if (!s_locked)
            {
                Selection::setActiveGameObject(gameObject);
            }
            Selection::setSelectedGameObjectInHierarchy(gameObject);
            s_isAnyItemClicked = true;
        }

        if (!s_openMenuPopup && ImGui::IsItemClicked(1))
        {
            if (!s_locked)
            {
                Selection::setActiveGameObject(gameObject);
            }
            Selection::setSelectedGameObjectInHierarchy(gameObject);
            s_isAnyItemClicked = true;
            Debug::LogWarning("Right button");
            ImGui::OpenPopup("HierarchyItem.menu");
            s_openMenuPopup = true;
        }


        // child node
        if (!is_leaf)
        {
            if (node_open)
            {
                for (auto t : gameObject->transform()->m_children)
                    HierarchyItem(t.lock()->gameObject());
                ImGui::TreePop();
            }
        }
        m_idCount++;
        if (!gameObject->activeSelf())
        {
            ImGui::PopStyleColor();
        }
    }

    void EditorGUI::DrawHierarchyWindow()
    {
        s_isAnyItemClicked = false;
        //auto selectedGO = Selection::activeGameObject();
        auto selectedGO = Selection::selectedGameObjectInHierarchy();
        // Hierarchy view
        //ImGui::BeginDock("Hierarchy");

        if (s_windowResized)
        {
            ImGui::SetNextWindowPos(hierarchyWindowPos);
            ImGui::SetNextWindowSize(hierarchyWindowSize);
        }
        ImGui::Begin("Hierarchy", nullptr, globalWindowFlags);
        //static ImGuiTextFilter filter;
        //filter.Draw();
        static char filterStr[128];
        if (ImGui::InputText("Filter", filterStr, 127))
        {
            Debug::LogWarning("Filter changed");
        }


        if (ImGui::Button("Create"))
        {
            s_isAnyItemClicked = true;
            auto go = Scene::CreateGameObject("GameObject");
            if (Selection::selectedGameObjectInHierarchy() != nullptr)
            {
                go->transform()->SetParent(Selection::selectedGameObjectInHierarchy()->transform());
            }
        }

        m_idCount = 0;
        ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, ImGui::GetFontSize()); // Increase spacing to differentiate leaves from expanded contents.
        for (auto& go : Scene::m_gameObjects)
        {
            if (go->transform()->parent() == nullptr)
            {
                HierarchyItem(go);
            }
        }
        ImGui::PopStyleVar();

        if (s_openMenuPopup)
        {
            ImGui::OpenPopup("HierarchyItem.menu");
            s_openMenuPopup = false;
        }
        if (ImGui::BeginPopup("HierarchyItem.menu"))
        {
            ImGui::Selectable("Copy");
            ImGui::Selectable("Paste");
            ImGui::Separator();
            ImGui::Selectable("Rename");
            ImGui::Selectable("Duplicate");
            if (ImGui::Selectable("Delete") && selectedGO != nullptr)
            {
                Object::DestroyImmediate(selectedGO);
                s_openMenuPopup = false;
            }
            ImGui::Separator();
            ImGui::Selectable("Select Prefab");
            ImGui::Separator();
            ImGui::Selectable("Create Empty");
            ImGui::EndPopup();
        }

        if (ImGui::IsKeyDown(ImGuiKey_Delete))
        {
            Debug::LogWarning("delete");
        }

        // TODO: remove this
        if (!s_isAnyItemClicked && ImGui::IsMouseClicked(0) && ImGui::IsMouseHoveringWindow())
        {
            Selection::setSelectedGameObjectInHierarchy(nullptr);
            if (!s_locked)
                Selection::setActiveGameObject(nullptr);
            Selection::setSelectedGameObjectInHierarchy(nullptr);
        }

        //ImGui::EndDock();
        ImGui::End();

    }


    void EditorGUI::DrawInspectorWindow()
    {
        auto selectedGO = Selection::activeGameObject();

        // Inspector Editor
        //ImGui::BeginDock("Inspector", nullptr);

        if (s_windowResized)
        {
            ImGui::SetNextWindowPos(inspectorWindowPos);
            ImGui::SetNextWindowSize(inspectorWindowSize);
        }
        ImGui::Begin("Inspector", nullptr, globalWindowFlags);
        ImGui::PushItemWidth(ImGui::GetWindowWidth()*0.55f);
        if (selectedGO == nullptr) {
            //ImGui::EndDock(); // Inspector Editor
            ImGui::End();
            return;
        }
        if (ImGui::Checkbox("Lock", &s_locked)) {
            if (!s_locked)
                Selection::setActiveGameObject(Selection::selectedGameObjectInHierarchy());
        }
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
            //selectedGO->m_transform->OnInspectorGUI();
            ImGui::Indent(inspector_indent_width);
            OnInspectorGUI<FishEngine::Transform>(std::static_pointer_cast<FishEngine::Transform>(selectedGO->m_transform));
            ImGui::Unindent(inspector_indent_width);
        }

        int local_id = 0;

        //auto headerName = [&local_id](const std::string& className) {
        //    return camelCaseToReadable(className) + "##header" + boost::lexical_cast<std::string>(local_id++);
        //};

        std::shared_ptr<Component> componentToBeDestroyed = nullptr;
        for (auto c : selectedGO->m_components)
        {
            ImGui::PushID(local_id++);
            bool is_open = true;
            if (ImGui::CollapsingHeader(c->ClassName().c_str(), &is_open, ImGuiTreeNodeFlags_DefaultOpen))
            {
                ImGui::Indent(inspector_indent_width);
                OnInspectorGUI(c);
                ImGui::Unindent(inspector_indent_width);
            }
            if (!is_open)
            {
                //Object::DestroyImmediate(c);
                componentToBeDestroyed = c;
            }
            ImGui::PopID();
        }

        if (componentToBeDestroyed != nullptr)
        {
            Object::DestroyImmediate(componentToBeDestroyed);
        }


        std::shared_ptr<Script> scriptToBeDestroyed = nullptr;
        for (auto s : selectedGO->m_scripts)
        {
            ImGui::PushID(local_id++);
            bool is_open = true;
            if (ImGui::CollapsingHeader(s->ClassName().c_str(), &is_open, ImGuiTreeNodeFlags_DefaultOpen))
            {
                //OnInspectorGUI(s);
                ImGui::Indent(inspector_indent_width);
                s->OnInspectorGUI();
                ImGui::Unindent(inspector_indent_width);
            }
            if (!is_open)
            {
                //Object::DestroyImmediate(s);
                scriptToBeDestroyed = s;
            }
            ImGui::PopID();
        }

        if (scriptToBeDestroyed != nullptr)
        {
            Object::DestroyImmediate(scriptToBeDestroyed);
        }

        /************************************************************************/
        /*                          Add Component                               */
        /************************************************************************/
        if (selectedGO != nullptr)
        {
            if (EditorGUI::Button("Add Component"))
            {
                ImGui::OpenPopup("AddComponent");
                //auto comp = Component::CreateComponent("Camera");
                //selectedGO->AddComponent(comp);
            }

            static const char* names[] = {
                "Camera", "Animator", "MeshFilter", "MeshRenderer", "Rigidbody", "SkinnedMeshRenderer",
                "BoxCollider", "CapsuleCollider", "SphereCollider", "SphereCollider",
            };

            bool addComponentFailed = false;
            static const char* failedComponentName = nullptr;
            if (ImGui::BeginPopup("AddComponent"))
            {
                ImGui::Text("Components");
                ImGui::Separator();
                for (int i = 0; i < IM_ARRAYSIZE(names); i++)
                {
                    if (ImGui::Selectable(names[i]))
                    {
                        //auto comp = Component::CreateComponent(names[i]);
                        //selectedGO->AddComponent(comp);
                        if (nullptr == AddComponentToGameObject(names[i], selectedGO))
                        {
                            //ImGui::OpenPopup("SameComponentWarning");
                            failedComponentName = names[i];
                            addComponentFailed = true;
                            break;
                        }
                    }
                }
                ImGui::EndPopup();
            }

            if (addComponentFailed)
                ImGui::OpenPopup("SameComponentWarning");
            if (ImGui::BeginPopupModal("SameComponentWarning", nullptr,
                ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize))
            {
                ImGui::Text("The Component %s can't be added \n because %s already contains the same component.",
                    failedComponentName,
                    selectedGO->name().c_str());
                if (ImGui::Button("Cancel"))
                    ImGui::CloseCurrentPopup();
                ImGui::EndPopup();
            }
        }

        //ImGui::EndDock(); // Inspector Editor
        ImGui::End();
    }


    void EditorGUI::DrawProjectWindow()
    {
        if (s_windowResized)
        {
            ImGui::SetNextWindowPos(projectWindowPos);
            ImGui::SetNextWindowSize(projectWindowSize);
        }
        ImGui::Begin("Project", nullptr, globalWindowFlags);
        ImGui::End();
    }

    void EditorGUI::DrawMainMenu()
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

            ImGui::SameLine();
            if (FishEditorWindow::InPlayMode()) {
                if (ImGui::Button("Stop")) {
                    FishEditorWindow::Stop();
                }
            }
            else {
                if (ImGui::Button("Play")) {
                    FishEditorWindow::Play();
                }
            }

            ImGui::SameLine();
            if (ImGui::Button("Translate")) {
                m_transformToolType = TransformToolType::Translate;
            }

            ImGui::SameLine();
            if (ImGui::Button("Rotate")) {
                m_transformToolType = TransformToolType::Rotate;
            }

            ImGui::SameLine();
            if (ImGui::Button("Scale")) {
                m_transformToolType = TransformToolType::Scale;
            }
            
            ImGui::SameLine();
            if (ImGui::Button(ToString(m_transformSpace)))
            {
                m_transformSpace = m_transformSpace==TransformSpace::Global ?
                    TransformSpace::Local : TransformSpace::Global;
            }

            float new_time = (float)glfwGetTime();
            int fps = (int)roundf(1.f / float(new_time - time_stamp));
            time_stamp = new_time;
            std::string fps_str = "FPS: " + std::to_string(fps);
            auto fps_stats_size = ImGui::CalcTextSize(fps_str.c_str());
            ImGui::SameLine(ImGui::GetContentRegionMax().x - fps_stats_size.x);
            ImGui::Text("%s", fps_str.c_str());
            ImGui::EndMainMenuBar();
        }
    }

    void EditorGUI::DrawToolBar()
    {
        ImGui::Begin("ToolBar");
        ImGui::Button("Play");
        ImGui::End();
    }


    void EditorGUI::DrawTranslateGizmo()
    {
        constexpr float translate_gizmo_length = 0.1f;
        static Vector3 lastMousePosition;
        static Vector3 lastCenter;
        
        auto selectedGO = Selection::selectedGameObjectInHierarchy();
        if (m_lastSelectedGameObject.lock() != selectedGO)
        {
            m_selectedAxis = -1;
            m_lastSelectedGameObject = selectedGO;
        }
        if (selectedGO == nullptr)
            return;

        auto camera = Camera::main();
        Vector3 center = selectedGO->transform()->position();
        Vector3 camera_pos = camera->transform()->position();
        Vector3 dir = center - camera_pos;
        center = dir.normalized() + camera_pos;

        Vector3 axis[3];
        if (m_transformSpace == TransformSpace::Local)
        {
            axis[0] = selectedGO->transform()->right();     // +x
            axis[1] = selectedGO->transform()->up();        // +y
            axis[2] = selectedGO->transform()->forward();   // +z
        }
        else
        {
            axis[0] = Vector3::right;   // +x
            axis[1] = Vector3::up;      // +y
            axis[2] = Vector3::forward; // +z
        }
        
        // check if any axis is selected by mouse
        if (Input::GetMouseButtonDown(0))
        {
            m_selectedAxis = -1;
            Ray view = camera->ScreenPointToRay(Input::mousePosition());
            float t[3];
            view.IntersectPlane(axis[0], center, t);
            view.IntersectPlane(axis[1], center, t+1);
            view.IntersectPlane(axis[2], center, t+2);
            float tmid;
            if (t[0] >= t[1])
            {
                if (t[1] >= t[2])
                    tmid = t[1];
                else if (t[0] >= t[2])
                    tmid = t[2];
                else
                    tmid = t[0];
            } else
            {
                if (t[0] >= t[2])
                    tmid = t[0];
                else if (t[1] >= t[2])
                    tmid = t[2];
                else
                    tmid = t[1];
            }
            Vector3 p = view.GetPoint(tmid);
            Vector3 d = p-center;
            if (d.magnitude() <= translate_gizmo_length * 1.3f)
            {
                d.Normalize();
                for (int i = 0; i < 3; ++i)
                {
                    if (Vector3::Dot(d, axis[i]) > 0.96f)
                    {
                        m_selectedAxis = i;
                        s_mouseEventHandled = true;
                        break;
                    }
                }
            }
        }

        auto shader = sceneGizmoMaterial->shader();
        shader->Use();
        sceneGizmoMaterial->SetVector3("unity_LightPosition", Vector3(0, 0, -1));
        auto view = camera->worldToCameraMatrix();
        auto proj = camera->projectionMatrix();
        auto vp = proj * view;

        ShaderUniforms uniforms;


        Color colors[3] = {Color::red, Color::green, Color::blue};
        
        float f[] = {
            1,  0,  0,   0, 0, -90,    // red axis, +x
            0,  1,  0,   0, 0,   0,    // green axis, +y
            0,  0,  1,  90, 0,   0,    // blue axis, +z
        };
        
        FishEngine::Matrix4x4 model;

        for (int i = 0; i < 3; ++i)
        {
            int j = 6 * i;
            Vector3 pos = center + axis[i]*translate_gizmo_length;
            Quaternion rot = Quaternion::FromToRotation(Vector3::up, axis[i]);
            model.SetTRS(pos, rot, Vector3(1, 1.5, 1) * 0.015f);
            Pipeline::perDrawUniformData.MATRIX_MVP = vp*model;
            Pipeline::perDrawUniformData.MATRIX_IT_MV = view*model;
            Pipeline::BindPerDrawUniforms();
            Vector3 color = m_selectedAxis == i ? Vector3(1, 1, 0) : Vector3(f + j);
            sceneGizmoMaterial->SetVector3("_Color", color);
            shader->Use();
            shader->PreRender();
            sceneGizmoMaterial->Update();
            shader->CheckStatus();
            coneMesh->Render();

            Gizmos::setColor(Color(color.x, color.y, color.z, 1.0f));
            Gizmos::DrawLine(center, pos);
        }

        if (m_selectedAxis < 0)
            return;
        
        // get intersected point of two Rays
        // solve t2 in: o1 + t1 * dir1 = o2 + t2 * dir2
        auto solve = [](const Vector3& o1, const Vector3 dir1, const Vector3& o2, const Vector3& dir2) -> float
        {
            float x = dir1.x;
            float y = dir1.y;
            float z = dir1.z;
            float test = dir2.z*y - dir2.y*z;
            if (!Mathf::Approximately(test, 0.0f))
                return ((o1.z*y - o1.y*z) - (o2.z*y - o2.y*z)) / test;
            else
                return ((o1.z*x - o1.x*z) - (o2.z*x - o2.x*z)) / (dir2.z*x - dir2.x*z);
        };
        
        center = selectedGO->transform()->position();
        
        auto& axis_selected = axis[m_selectedAxis];
        
        if (Input::GetMouseButtonDown(0))
        {
            lastCenter = center;
            Ray ray = Camera::main()->ScreenPointToRay(Input::mousePosition());
            float t = solve(center, axis_selected, camera_pos, ray.direction);
            lastMousePosition = ray.GetPoint(t);
            return;
        }

        // handle mouse movement event
        if (Input::GetMouseButton(0))
        {
            Vector3 mouse_movement(Input::GetAxis(Axis::MouseX)*Screen::width(), Input::GetAxis(Axis::MouseY)*Screen::height(), 0); // in piexls
            Vector3 axis_on_plane = vp.MultiplyVector(axis_selected);
            axis_on_plane.z = 0;
            axis_on_plane.Normalize();                              // axis on the near clip plane
            Vector3 p = Input::mousePosition() - mouse_movement + Vector3::Project(mouse_movement, axis_on_plane);   // new mouse position(along the axis)
            Vector3 new_view_dir = Camera::main()->ScreenPointToRay(p).direction;
            
            // solve: camera_pos + t1 * new_view_dir = lastMousePosition + t2 * axis
            float t  = solve(camera_pos, new_view_dir, lastMousePosition, axis_selected);
            selectedGO->transform()->setPosition(lastCenter + t*axis_selected);
        }
    }

    void EditorGUI::DrawRotateGizmo()
    {
        static Vector3 lastFromDir;
        static Quaternion lastRotation;
        auto selectedGO = Selection::selectedGameObjectInHierarchy();
        if (m_lastSelectedGameObject.lock() != selectedGO)
        {
            m_selectedAxis = -1;
            m_lastSelectedGameObject = selectedGO;
        }
        if (selectedGO == nullptr)
            return;

        auto camera = Camera::main();
        //auto view = camera->worldToCameraMatrix();
        //auto proj = camera->projectionMatrix();
        //auto vp = proj * view;
        Vector3 center = selectedGO->transform()->position();
        Vector3 camera_pos = camera->transform()->position();
        Vector3 dir = center - camera_pos;
        Vector3 gizmo_center = dir.normalized() + camera_pos;

        constexpr float radius = 0.1f;
        
        Vector3 axis[3];
        if (m_transformSpace == TransformSpace::Local)
        {
            axis[0] = selectedGO->transform()->right();     // +x
            axis[1] = selectedGO->transform()->up();        // +y
            axis[2] = selectedGO->transform()->forward();   // +z
        }
        else
        {
            axis[0] = Vector3::right;   // +x
            axis[1] = Vector3::up;      // +y
            axis[2] = Vector3::forward; // +z
        }

        // check if any axis is selected by mouse
        if (Input::GetMouseButtonDown(0))
        {
            m_selectedAxis = -1;
            auto ray = Camera::main()->ScreenPointToRay(Input::mousePosition());
            float t;
            if (ray.IntersectSphere(gizmo_center, radius, &t))
            {
                auto p = ray.GetPoint(t);
                for (int i = 0; i < 3; ++i)
                {
                    auto dir = Vector3::Normalize(p - gizmo_center);
                    if ( std::fabsf(Vector3::Dot(axis[i], dir)) < 0.1f )
                    {
                        s_mouseEventHandled = true;
                        m_selectedAxis = i;
                        lastFromDir = Vector3::Cross(axis[i], Vector3::Cross(dir, axis[i]));
                        lastFromDir.Normalize();
                        lastRotation = selectedGO->transform()->localRotation();
                    }
                }
            }
        }

        //Gizmos::DrawWireSphere(center, 0.1f);
        Color colors[3] = {Color::red, Color::green, Color::blue};
        for (int i = 0; i < 3; ++i)
        {
            Gizmos::setColor(colors[i]);
            Gizmos::DrawLine(gizmo_center, gizmo_center+axis[i] * radius);
            if (m_selectedAxis == i)
                Gizmos::setColor(Color::yellow);
            Gizmos::DrawCircle(gizmo_center, radius, axis[i]);
        }

        Gizmos::setColor(Color::white);
        Gizmos::DrawCircle(gizmo_center, radius, camera->transform()->forward());
        Gizmos::DrawCircle(gizmo_center, radius*1.1f, camera->transform()->forward());

        if (m_selectedAxis < 0)
            return;
        
        if (Input::GetMouseButton(0))
        {
            Ray view = camera->ScreenPointToRay(Input::mousePosition());
            auto& face_normal = axis[m_selectedAxis];
            float t;
            if (!view.IntersectPlane(face_normal, center, &t))
            {
                return;
            }
//            if (t >= Vector3::Distance(center, camera_pos) + 0.001f)
//            {
//                return;
//            }
            Vector3 intersected_p = view.GetPoint(t);
            Vector3 dir_to = Vector3::Normalize(intersected_p - center);
            auto rot = Quaternion::FromToRotation(lastFromDir, dir_to) * lastRotation;
            selectedGO->transform()->setLocalRotation(rot);
            
            Gizmos::setColor(Color::yellow);
            Gizmos::DrawLine(gizmo_center, gizmo_center + lastFromDir * radius);
            Gizmos::DrawLine(gizmo_center, gizmo_center + dir_to * radius);
        }
    }

    void EditorGUI::DrawScaleGizmo()
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
            ray.origin = inv_model * Vector4(2.0f*x / w - 1.f, 2.0f*y / w - 1.0f, -5, 1);
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
            int j = i * 6;
            Vector3 pos(f + j);
            Vector3 color(1, 1, 1);
            if (i >= 3)
                color = Vector3(f + j);
            if (inRegion && !interested) {
                Bounds aabb(pos*0.7f, Vector3::one*0.5f);
                if (aabb.IntersectRay(ray)) {
                    interested = true;
                    hoverIndex = i;
                    color.Set(1, 1, 0); // yellow
                }
            }
            t.setLocalPosition(pos);
            t.setLocalEulerAngles(f[j + 3], f[j + 4], f[j + 5]);
            auto modelMat = model * t.localToWorldMatrix() * s;
            //sceneGizmoMaterial->SetMatrix("MATRIX_MVP", vp*modelMat);
            //sceneGizmoMaterial->SetMatrix("MATRIX_IT_MV", (view * modelMat).transpose().inverse());
            Pipeline::perDrawUniformData.MATRIX_MVP = vp*modelMat;
            Pipeline::perDrawUniformData.MATRIX_IT_MV = (view * modelMat).transpose().inverse();
            Pipeline::BindPerDrawUniforms();
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

        Pipeline::perDrawUniformData.MATRIX_MVP = vp*model;
        Pipeline::perDrawUniformData.MATRIX_IT_MV = view*model;
        Pipeline::BindPerDrawUniforms();
        //sceneGizmoMaterial->SetMatrix("MATRIX_MVP", vp*model);
        //sceneGizmoMaterial->SetMatrix("MATRIX_IT_MV", view*model);
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
            }
            else {
                Vector3 offset(f + hoverIndex * 6);
                Vector3 up(0, 1, 0);
                if (offset.y == 1) {
                    up.Set(0, 0, -1);
                }
                else if (offset.y == -1) {
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

    // read-write
    bool Float(const char* label, float* value)
    {
        return ImGui::InputFloat(label, value);
    }

    bool Int(const char* label, int* value)
    {
        return ImGui::InputInt(label, value);
    }

    bool Bool(const char* label, bool* value)
    {
        return ImGui::Checkbox(label, value);
    }

    bool Float3(const char* label, Vector3* value)
    {
        return ImGui::InputFloat3(label, value->data());
    }

    // read-only version
    void Float(const char* label, float value)
    {
        ImGui::InputFloat(label, &value, 0.f, 0.f, -1, ImGuiInputTextFlags_ReadOnly);
    }

    void Int(const char* label, int value)
    {
        ImGui::InputInt(label, &value, 0, 0, ImGuiInputTextFlags_ReadOnly);
    }

    void Bool(const char* label, bool value)
    {
        ImGui::Checkbox(label, &value);
    }

    void Float3(const char* label, Vector3& value)
    {
        ImGui::InputFloat3(label, value.data(), -1, ImGuiInputTextFlags_ReadOnly);
    }
    
    template<typename T>
    void Enum(const char* label, const T& e)
    {
        int index = ToIndex(e);
        ImGui::Combo(label, &index, LightTypeStrings, EnumCount<T>());
    }
    
    template<typename T>
    void Enum(const char* label, T* e)
    {
        int index = ToIndex(*e);
        if (ImGui::Combo(label, &index, LightTypeStrings, EnumCount<T>()))
        {
            *e = ToEnum<T>(index);
        }
    }

    template<>
    void EditorGUI::OnInspectorGUI(const std::shared_ptr<FishEngine::Transform>& transform)
    {
        if (Float3("Position", &transform->m_localPosition)) {
            transform->MakeDirty();
        }
        if (Float3("Rotation", &transform->m_localEulerAngles)) {
            transform->m_localRotation.setEulerAngles(transform->m_localEulerAngles);
            transform->MakeDirty();
        }
        if (Float3("Scale", &transform->m_localScale)) {
            transform->MakeDirty();
        }
    }

    template<>
    void EditorGUI::OnInspectorGUI(const std::shared_ptr<FishEngine::Camera>& camera)
    {
        const char* listbox_items[] = {
            "Perspective", "Orthographic"
        };
        int list_item_current = camera->m_orthographic ? 1 : 0;
        ImGui::Combo("Projection", &list_item_current, listbox_items, 2);
        if (camera->m_orthographic != (list_item_current == 1)) {
            camera->setOrthographic(!camera->m_orthographic);
        }

        if (camera->m_orthographic) {
            if (Float("Size", &camera->m_orthographicSize)) {
                camera->m_isDirty = true;
            }
        }
        else {
            if (ImGui::SliderFloat("Field of View", &camera->m_fieldOfView, 1, 179)) {
                camera->m_isDirty = true;
            }
        }

        if (Float("Clipping Planes(Near)", &camera->m_nearClipPlane)) {
            camera->m_isDirty = true;
        }
        if (Float("Clipping Planes(Far)", &camera->m_farClipPlane)) {
            camera->m_isDirty = true;
        }
        ImGui::InputFloat4("Viewport Rect", camera->m_viewport.data());
    }

    template<>
    void EditorGUI::OnInspectorGUI(const std::shared_ptr<FishEngine::Animator>& animator)
    {
        if (animator->m_animation != nullptr) {
            int channels = (int)animator->m_animation->channels.size();
            Int("Channel count", channels);
        }
        if (animator->m_playing) {
            if (ImGui::Button("Stop")) {
                animator->Stop();
            }
        }
        else {
            if (ImGui::Button("Play")) {
                animator->Play();
            }
            ImGui::SameLine();
            if (ImGui::Button("Play Once")) {
                animator->PlayOnce();
            }
        }

        if (ImGui::Button("Next Frame")) {
            animator->NextFrame();
        }
        Float("Time", &animator->m_time);
    }

    template<>
    void EditorGUI::OnInspectorGUI(const std::shared_ptr<FishEngine::MeshFilter>& meshFilter)
    {
        if (ImGui::Button("Change")) {
            ImGui::OpenPopup("Select ...");
        }
        EditorGUI::SelectMeshDialogBox([&meshFilter](PMesh mesh)->void {
            meshFilter->SetMesh(mesh);
        });
        //ImGui::SameLine();
        ImGui::LabelText("Mesh", "%s", meshFilter->m_mesh->name().c_str());

        //bool skinned = meshFilter->m_mesh->m_skinned;
        //ImGui::Checkbox("Skinned", &skinned);
        if (meshFilter->m_mesh->m_skinned) {
            int boneCount = (int)meshFilter->m_mesh->m_boneNameToIndex.size();
            Int("Bone Count", boneCount);
        }
    }

    template<>
    void EditorGUI::OnInspectorGUI(const std::shared_ptr<Material>& material)
    {
        if (ImGui::CollapsingHeader(material->m_name.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Indent();
            auto& uniforms = material->m_shader->uniforms();
            for (auto& u : uniforms) {
                if (u.type == GL_FLOAT) {
                    ImGui::SliderFloat(u.name.c_str(), &material->m_uniforms.floats[u.name], 0, 1);
                }
                else if (u.type == GL_FLOAT_VEC3) {
                    ImGui::InputFloat3(u.name.c_str(), material->m_uniforms.vec3s[u.name].data());
                }
                else if (u.type == GL_FLOAT_VEC4) {
                    ImGui::InputFloat4(u.name.c_str(), material->m_uniforms.vec4s[u.name].data());
                }
                else if (u.type == GL_SAMPLER_2D || u.type == GL_SAMPLER_CUBE) {
                    auto& tex = material->m_textures[u.name];
                    ImGui::LabelText(u.name.c_str(), "%s", tex->name().c_str());
                    ImGui::Image((void*)tex->GLTexuture(), ImVec2(64, 64));
                    ImGui::SameLine();
                    ImGui::Button("Select");
                }
            }
            ImGui::Unindent();
        }
    }

    template<>
    void EditorGUI::OnInspectorGUI(const std::shared_ptr<Renderer>& renderer)
    {
        if (ImGui::CollapsingHeader("Materials", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::Indent(inspector_indent_width);
            Int("Size", (int)renderer->m_materials.size());
            for (auto& m : renderer->m_materials)
            {
                OnInspectorGUI(m);
            }
            ImGui::Unindent(inspector_indent_width);
        }
    }

    template<>
    void EditorGUI::OnInspectorGUI(const std::shared_ptr<FishEngine::MeshRenderer>& renderer)
    {
        OnInspectorGUI<Renderer>(renderer);
    }


    template<>
    void EditorGUI::OnInspectorGUI(const std::shared_ptr<FishEngine::SkinnedMeshRenderer>& renderer)
    {
        OnInspectorGUI<Renderer>(renderer);

        ImGui::LabelText("Mesh", "%s", renderer->m_sharedMesh->name().c_str());
        ImGui::LabelText("Root Bone", "%s", renderer->m_rootBone.lock()->name().c_str());

        ImGui::Text("Bounds");
        Bounds bounds = renderer->localBounds();
        auto center = bounds.center();
        auto extents = bounds.extents();
        Float3("Center", center);
        Float3("Extents", extents);
    }


    template<>
    void EditorGUI::OnInspectorGUI(const std::shared_ptr<FishEngine::Rigidbody>& rigidBody)
    {
        Float("Mass", &rigidBody->m_mass);
        Float("Drag", &rigidBody->m_drag);
        Float("Angular", &rigidBody->m_angularDrag);
        Bool("Use Gravity", &rigidBody->m_useGravity);
        Bool("Is Kinematic", &rigidBody->m_isKinematic);
    }


    template<>
    void EditorGUI::OnInspectorGUI(const std::shared_ptr<FishEngine::BoxCollider>& boxCollider)
    {
        Bool("Is Trigger", &boxCollider->m_isTrigger);
        Float3("Center", &boxCollider->m_center);
        Float3("Size", &boxCollider->m_size);
    }

    template<>
    void EditorGUI::OnInspectorGUI(const std::shared_ptr<FishEngine::SphereCollider>& sphereCollider)
    {
        Bool("Is Trigger", &sphereCollider->m_isTrigger);
        Float3("Center", &sphereCollider->m_center);
        Float("Radius", &sphereCollider->m_radius);
    }


    template<>
    void EditorGUI::OnInspectorGUI(const std::shared_ptr<FishEngine::CapsuleCollider>& capsuleCollider)
    {
        Bool("Is Trigger", &capsuleCollider->m_isTrigger);
        Float3("Center", &capsuleCollider->m_center);
        Float("Radius", &capsuleCollider->m_radius);
        Float("Height", &capsuleCollider->m_height);
        //ImGui::InputFloat3("Direction", capsuleCollider->m_direction);
        const char* listbox_items[] = {
            "X-Axis", "Y-Axis", "Z-Axis"
        };
        ImGui::Combo("Direction", &capsuleCollider->m_direction, listbox_items, 3);
    }

    template<>
    void EditorGUI::OnInspectorGUI(const std::shared_ptr<FishEngine::Light>& light)
    {
        Enum<LightType>("Type", &light->m_type);
        ImGui::ColorEdit4("Color", light->m_color.m);
        Float("Intensity", &light->m_intensity);
        ImGui::DragFloat("Range", &light->m_range);
        Float("Bias", &light->m_shadowBias);
        Float("Normal Bias", &light->m_shadowNormalBias);
        Float("Shadow Near Plane", &light->m_shadowNearPlane);
    }

    template<>
    void EditorGUI::OnInspectorGUI(const std::shared_ptr<FishEngine::Component>& component)
    {
#define Case(T) else if (component->ClassName() == FishEngine::T::StaticClassName()) { OnInspectorGUI(std::static_pointer_cast<FishEngine::T>(component)); }

        if (component->ClassName() == FishEngine::Transform::StaticClassName()) {
            OnInspectorGUI(std::static_pointer_cast<FishEngine::Transform>(component));
        }
        Case(Camera)
        Case(Animator)
        Case(MeshFilter)
        Case(MeshRenderer)
        Case(BoxCollider)
        Case(SphereCollider)
        Case(CapsuleCollider)
        Case(Rigidbody)
        Case(SkinnedMeshRenderer)
        Case(Light)
#undef Case
    }


    void EditorGUI::CalculateWindowSizeAndPosition()
    {
        int w = Screen::width();
        int h = Screen::height();
        float scale = 1.0f / Screen::pixelsPerPoint();
        w *= scale;
        h *= scale;

        hierarchyWindowPos.x = 0;
        hierarchyWindowPos.y = menuBarHeight;
        hierarchyWindowSize.x = 200;
        hierarchyWindowSize.y = h - menuBarHeight - 200;

        projectWindowPos.x = 0;
        projectWindowPos.y = static_cast<float>(h - 200);
        projectWindowSize.x = static_cast<float>(w - 256);
        projectWindowSize.y = 200;

        inspectorWindowPos.x = static_cast<float>(w - 256);
        inspectorWindowPos.y = menuBarHeight;
        inspectorWindowSize.x = 256;
        inspectorWindowSize.y = h - menuBarHeight;
        s_windowResized = true;
    }
}
