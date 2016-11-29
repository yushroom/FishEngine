#include "EditorGUI.hpp"

#include <sstream>
#include <iostream>
#include <fstream>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw_gl3.h>
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui_internal.h>
#include <imgui/imgui_dock.h>
#include <imgui/imgui_user.h>
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
#include <Texture.hpp>

#include <Serialization.hpp>
#include <cereal/archives/json.hpp>

#include "FishEditorWindow.hpp"
#include "Selection.hpp"
#include "EditorRenderSystem.hpp"
#include "EditorInput.hpp"
#include "MaterialEditor.hpp"
#include "Shader.hpp"
#include "Resources.hpp"

using namespace FishEngine;

namespace FishEditor
{
    constexpr float inspector_indent_width              = 4;

    SceneViewEditorPtr EditorGUI::m_mainSceneViewEditor;

    bool    EditorGUI::s_locked = false;
    //int     EditorGUI::m_selectedAxis                   = -1;
    bool    EditorGUI::s_isAnyItemClicked               = false;
    bool    EditorGUI::s_openMenuPopup                  = false;
    bool    EditorGUI::m_showAssectSelectionDialogBox   = false;

    ImGuiWindowFlags globalWindowFlags = 0;
    
    FishEngine::Int2 EditorGUI::m_sceneViewSize{1, 1};

    const char* ToString(TransformSpace& space)
    {
        if (space == TransformSpace::Global)
            return "Global";
        else
            return "Local";
    }

    struct EditoGUISettings
    {
        float mainMenubarHeight;
        float mainToolbarHeight;
    };

    EditoGUISettings g_editorGUISettings;


    void EditorGUI::Init()
    {
#if FISHENGINE_PLATFORM_WINDOWS
        const std::string root_dir = "D:/program/FishEngine/Assets/";
#else
        const std::string root_dir = "/Users/yushroom/program/graphics/FishEngine/assets/";
#endif

        ImGuiIO& io = ImGui::GetIO();
        io.Fonts->AddFontFromFileTTF((root_dir + "fonts/DroidSans.ttf").c_str(), 15.0f);
        io.IniFilename = nullptr;

        ImGuiContext& g = *GImGui;
        ImGuiStyle& style = g.Style;
#if 0
        style.FrameRounding = 2.f;
        style.WindowRounding = 0.f;
        style.Colors[ImGuiCol_Text] = ImVec4(1, 1, 1, 1);
        style.Colors[ImGuiCol_Button] = ImVec4(0, 0, 0, 1.f);
        style.Colors[ImGuiCol_ButtonHovered] = ImVec4(40 / 255.f, 40 / 255.f, 40 / 255.f, 1.f);
        style.Colors[ImGuiCol_ButtonActive] = ImVec4(181 / 255.f, 214 / 255.f, 232 / 255.f, 1.f);
        style.Colors[ImGuiCol_WindowBg] = ImVec4(64 / 255.f, 64 / 255.f, 64 / 255.f, 1.0f);
        style.Colors[ImGuiCol_MenuBarBg] = ImVec4(45 / 255.f, 45 / 255.f, 45 / 255.f, 1.0f);
        style.Colors[ImGuiCol_TitleBg] = ImVec4(0.5f, 0.5f, 0.5f, 0.8f);
        style.Colors[ImGuiCol_PopupBg] = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
        style.Colors[ImGuiCol_ComboBg] = ImVec4(0.6f, 0.6f, 0.6f, 1.0f);
        style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(28 / 255.0f, 151 / 255.f, 234 / 255.0f, 1.f);
        style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0, 122 / 255.0f, 204 / 255.0f, 1.0f);
#else
        // dark theme, adapted from Cinder
        //style.WindowMinSize = ImVec2(160, 20);
        style.FramePadding = ImVec2(4, 2);
        style.ItemSpacing = ImVec2(6, 2);
        style.ItemInnerSpacing = ImVec2(6, 4);
        style.Alpha = 0.95f;
        //mStyle.WindowFillAlphaDefault = 1.0f;
        style.WindowRounding = 4.0f;
        style.FrameRounding = 2.0f;
        style.IndentSpacing = 6.0f;
        style.ItemInnerSpacing = ImVec2(2, 4);
        style.ColumnsMinSpacing = 50.0f;
        style.GrabMinSize = 14.0f;
        style.GrabRounding = 16.0f;
        style.ScrollbarSize = 12.0f;
        style.ScrollbarRounding = 16.0f;

        //style.Colors[ImGuiCol_Text] = ImVec4(0.86f, 0.93f, 0.89f, 0.61f);
        style.Colors[ImGuiCol_Text] = ImVec4(1, 1, 1, 1);
        style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.86f, 0.93f, 0.89f, 0.28f);
        style.Colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.14f, 0.17f, 1.00f);
        style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.20f, 0.22f, 0.27f, 0.58f);
        style.Colors[ImGuiCol_Border] = ImVec4(0.31f, 0.31f, 1.00f, 0.00f);
        style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        style.Colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
        style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.78f);
        style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
        style.Colors[ImGuiCol_TitleBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
        style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.20f, 0.22f, 0.27f, 0.75f);
        style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
        style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.20f, 0.22f, 0.27f, 0.47f);
        style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.47f, 0.77f, 0.83f, 0.21f);
        style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.78f);
        style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
        style.Colors[ImGuiCol_ComboBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
        style.Colors[ImGuiCol_CheckMark] = ImVec4(0.71f, 0.22f, 0.27f, 1.00f);
        style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
        style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
        style.Colors[ImGuiCol_Button] = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
        style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.86f);
        style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
        style.Colors[ImGuiCol_Header] = ImVec4(0.92f, 0.18f, 0.29f, 0.76f);
        style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.86f);
        style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
        style.Colors[ImGuiCol_Column] = ImVec4(0.47f, 0.77f, 0.83f, 0.32f);
        style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.78f);
        style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
        style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.47f, 0.77f, 0.83f, 0.04f);
        style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.78f);
        style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
        style.Colors[ImGuiCol_CloseButton] = ImVec4(0.86f, 0.93f, 0.89f, 0.16f);
        style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.86f, 0.93f, 0.89f, 0.39f);
        style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.86f, 0.93f, 0.89f, 1.00f);
        style.Colors[ImGuiCol_PlotLines] = ImVec4(0.86f, 0.93f, 0.89f, 0.63f);
        style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
        style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.86f, 0.93f, 0.89f, 0.63f);
        style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
        style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.92f, 0.18f, 0.29f, 0.43f);
        //style.Colors[ImGuiCol_TooltipBg] = ImVec4(0.47f, 0.77f, 0.83f, 0.72f);
        style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.22f, 0.27f, 0.73f);
#endif

        globalWindowFlags |= ImGuiWindowFlags_NoCollapse;
        globalWindowFlags |= ImGuiWindowFlags_NoResize;
        //globalWindowFlags |= ImGuiWindowFlags_ShowBorders;
        globalWindowFlags |= ImGuiWindowFlags_NoMove;
        //CalculateWindowSizeAndPosition();
        m_mainSceneViewEditor = std::make_shared<SceneViewEditor>();
        m_mainSceneViewEditor->Init();
    }


    void EditorGUI::Update()
    {
        auto selectedGO = Selection::selectedGameObjectInHierarchy();

        if (EditorInput::GetKeyDown(KeyCode::F))
        {
            Camera::main()->FrameSelected(selectedGO);
        }
        
        if (EditorInput::GetKeyDown(KeyCode::W))
        {
            m_mainSceneViewEditor->m_transformToolType = TransformToolType::Translate;
        }
        else if (EditorInput::GetKeyDown(KeyCode::E))
        {
            m_mainSceneViewEditor->m_transformToolType = TransformToolType::Rotate;
        }
        else if (EditorInput::GetKeyDown(KeyCode::R))
        {
            m_mainSceneViewEditor->m_transformToolType = TransformToolType::Scale;
        }

        if (EditorInput::GetKeyDown(KeyCode::LeftControl) && EditorInput::GetKeyDown(KeyCode::Z))
        {
            if (EditorInput::GetKeyDown(KeyCode::LeftShift))
            {
                Debug::LogWarning("Ctrl+Shift+Z");
                CommandManager::Redo();
            }
            else
            {
                Debug::LogWarning("Ctrl+Z");
                CommandManager::Undo();
            }
        }
        

        DrawMainMenu();
        DrawMainToolbar();
        DrawHierarchyWindow();
        DrawInspectorWindow();
        DrawProjectWindow();
        DrawSceneView();
        //MaterialEditor::Show();

        auto size = ImGui::GetIO().DisplaySize;
        float pos_y = g_editorGUISettings.mainMenubarHeight + g_editorGUISettings.mainToolbarHeight;
        size.y -= pos_y;
        ImGui::RootDock(ImVec2(0, pos_y), size);

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

        //if (EditorInput::GetKeyDown(KeyCode::G))
        //{
        //    GUIStyleTweakWindow();
        //}

        ImGui::Render();
        
        //if (EditorInput::GetKeyDown(KeyCode::P))
        //{
        //    Debug::Log("save layout");
        //    std::ofstream fout("./FishEditorLayout.xml");
        //    ImGui::SaveDock(fout);
        //}

        //if (EditorInput::GetKeyDown(KeyCode::O))
        //{
        //    Debug::Log("load layout");
        //    std::ifstream fin("./FishEditorLayout.xml");
        //    ImGui::LoadDock(fin);
        //}
    }

    void EditorGUI::Clean()
    {
        ImGui::ShutdownDock();
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

    void EditorGUI::SelectMeshDialogBox(std::function<void(MeshPtr)> callback)
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
        //CalculateWindowSizeAndPosition();
    }

    void EditorGUI::HierarchyItem(GameObjectPtr gameObject)
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

        bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)hash_value(gameObject->m_uuid), node_flags, "%s", gameObject->name().c_str());

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
        //m_idCount++;
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

        //ImGui::Begin("Hierarchy", nullptr, globalWindowFlags);
        ImGui::BeginDock("Hierarchy", nullptr);
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

        ImGui::EndDock();
        //ImGui::End();
    }


    void EditorGUI::DrawInspectorWindow()
    {
        ImGui::BeginDock("Inspector", nullptr);
        //ImGui::Begin("Inspector", nullptr, globalWindowFlags);
        ImGui::PushItemWidth(ImGui::GetWindowWidth()*0.55f);
        auto selectedGO = Selection::activeGameObject();
        if (selectedGO == nullptr)
        {
            ImGui::EndDock();
            //ImGui::End();
            return;
        }
        if (ImGui::Checkbox("Lock", &s_locked))
        {
            if (!s_locked)
                Selection::setActiveGameObject(Selection::selectedGameObjectInHierarchy());
        }
        ImGui::PushID("Inspector.selected.active");
        ImGui::Checkbox("", &selectedGO->m_activeSelf);
        ImGui::PopID();
        
        static char name[128] = { 0 };
        memcpy(name, selectedGO->name().c_str(), selectedGO->name().size());
        name[selectedGO->m_name.size()] = 0;
        ImGui::SameLine();
        ImGui::PushID("Inspector.selected.name");
        if (ImGui::InputText("", name, 127)) {
            selectedGO->m_name = name;
        }
        ImGui::PopID();

        ImGui::PushItemWidth(ImGui::GetWindowWidth()*0.3f);
        ImGui::Text("Tag");
        ImGui::SameLine();
        ImGui::LabelText("##Tag", "%s", selectedGO->tag().c_str());
        ImGui::SameLine();
        ImGui::Text("Layer");
        ImGui::SameLine();
        ImGui::LabelText("##Layer", "Layer %d", selectedGO->m_layer);
        ImGui::PopItemWidth();

        if (ImGui::CollapsingHeader("Transform##header", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::Indent(inspector_indent_width);
            OnInspectorGUI<FishEngine::Transform>(std::static_pointer_cast<FishEngine::Transform>(selectedGO->m_transform));
            ImGui::Unindent(inspector_indent_width);
        }

        int local_id = 0;

        ComponentPtr componentToBeDestroyed = nullptr;
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


        ScriptPtr scriptToBeDestroyed = nullptr;
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
                "BoxCollider", "CapsuleCollider", "SphereCollider",
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

        ImGui::EndDock(); // Inspector Editor
        //ImGui::End();
    }


    void EditorGUI::DrawProjectWindow()
    {
        ImGui::BeginDock("Project", nullptr);
        
        ImGui::PushStyleVar(ImGuiStyleVar_ChildWindowRounding, 5.0f);
        ImGui::BeginChild("Sub1", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.15f, 0));
        
        static int selected = 0;
        static int selected_item_id = -1;
        if (ImGui::Selectable("Textures", selected==0))
        {
            selected = 0;
            selected_item_id = -1;
        }
        if (ImGui::Selectable("Materials", selected==1))
        {
            selected = 1;
            selected_item_id = -1;
        }
        if (ImGui::Selectable("Shaders", selected==2))
        {
            selected = 2;
            selected_item_id = -1;
        }
        if (ImGui::Selectable("Scripts", selected==3))
        {
            selected = 3;
            selected_item_id = -1;
        }
        if (ImGui::Selectable("Models", selected==4))
        {
            selected = 4;
            selected_item_id = -1;
        }

        ImGui::Separator();

        if (ImGui::Selectable("Assets", selected == 5))
        {
            selected = 5;
            selected_item_id = -1;
        }

        ImGui::EndChild();
        ImGui::PopStyleVar();
        
        ImGui::SameLine();  // Sub1 and Sub2
        
        static FileNode* current_dir = nullptr;
        if (current_dir == nullptr)
            current_dir = &Resources::s_assetsDirectoryRootNode;
        ImGui::BeginChild("Sub2", ImVec2(0,0));

        ImGui::BeginChild("Sub2Top", ImVec2(0, 20));
        if (ImGui::Button("Assets"))
        {
            current_dir = &Resources::s_assetsDirectoryRootNode;
        }
        auto relative_path = boost::filesystem::relative(current_dir->path, Resources::s_assetsDirectory);
        Path rp(relative_path);
        int depth = 0;
        while (rp.has_parent_path())
        {
            depth++;
            rp = rp.parent_path();
        }
        for (auto& p : relative_path)
        {
            ImGui::SameLine();
            if (ImGui::Button(p.string().c_str()))
            {
                for (int i = 0; i < depth; ++i)
                {
                    current_dir = current_dir->parent;
                }
                break;
            }
            depth--;
        }
        ImGui::EndChild();

        ImGui::BeginChild("Sub2Bottom", ImVec2(0, 0));

        float image_size = 92;
        int item_count = 0;
        if (selected == 0) // textures
        {
            item_count = 0;
            static int item_selected = -1;
            for (const auto& t : Texture::AllTextures())
            {
                //constexpr ImVec2 size(128, 128);
                if (t->dimension() == TextureDimension::Tex2D)
                {
                    ImGui::SameLine();
                    if (ImGui::GetContentRegionAvailWidth() < image_size)
                    {
                        ImGui::NewLine();
                    }
                    //ImGui::Image((void*)t->GetNativeTexturePtr(), ImVec2(64, 64));
                    if (item_selected == item_count)
                        ImGui::ImageWithLabel(
                            t->name().c_str(),
                            (void*)t->GetNativeTexturePtr(),
                            ImVec2(image_size, image_size),
                            { 0, 0 }, { 1, 1 }, { 1, 1, 1, 1 }, { 1, 0, 0, 1 });
                    else
                        ImGui::ImageWithLabel(
                            t->name().c_str(),
                            (void*)t->GetNativeTexturePtr(),
                            ImVec2(image_size, image_size));
                    if (ImGui::IsItemClicked(0))
                    {
                        std::cout << t->name() << " clicked" << std::endl;
                        item_selected = item_count;
                    }
                }
                item_count++;
            }
        }
        else if (selected == 2) // shaders
        {
            item_count = 0;
            for (const auto& m : Shader::allShaders())
            {
                if (ImGui::Selectable(m.first.c_str(), selected_item_id == item_count))
                {
                    selected_item_id = item_count;
                }
                item_count++;
            }
        }
        else if (selected == 5) // Assets
        {
            item_count = 0;
            
            for (auto& node : current_dir->children)
            {
                ImGuiSelectableFlags flags = 0;
                if (node.IsDirectory())
                    flags |= ImGuiSelectableFlags_AllowDoubleClick;
                if (ImGui::Selectable(node.path.filename().string().c_str(),
                    selected_item_id == item_count, flags))
                {
                    selected_item_id = item_count;
                    if (ImGui::IsMouseDoubleClicked(0))
                    {
                        current_dir = &node;
                        break;
                    }
                }
                item_count++;
            }
        }

        ImGui::EndChild(); // Sub2Bottom
        ImGui::EndChild(); // Sub2

        
        //ImGui::End();
        ImGui::EndDock();
    }

    void EditorGUI::DrawSceneView()
    {
        ImGui::BeginDock("Scene", nullptr, ImGuiWindowFlags_NoScrollWithMouse);
        m_mainSceneViewEditor->m_focused = ImGui::IsWindowFocused();
        m_mainSceneViewEditor->m_isMouseHovered = ImGui::IsWindowHovered();

        //constexpr int menubar_height = 40;
        auto frame_padding = ImGui::GetStyle().FramePadding;
        float padding = frame_padding.y * 2;
        float height = 24 + padding;
        ImVec2 toolbar_size(ImGui::GetIO().DisplaySize.x, height);
        if (ImGui::BeginToolbar("SceneViewToolBar", ImVec2(0, g_editorGUISettings.mainMenubarHeight), toolbar_size))
        {
            if (ImGui::Button("Light on"))
            {
                
            }
            
            ImGui::SameLine();
            if (ImGui::Button("Gizmos on"))
            {
                
            }
            
            ImGui::SameLine();
            bool highlight = m_mainSceneViewEditor->highlightSelections();
            if (ImGui::Checkbox("Highlight", &highlight))
            {
                m_mainSceneViewEditor->setHighlightSelections(highlight);
            }
            
            ImGui::SameLine();
            if (ImGui::Button("Serialize"))
            {
                std::ofstream fout("Scene.json");
                {
                    cereal::JSONOutputArchive oa(fout);
                    oa << Scene::m_gameObjects;
                }
                //std::cout << ss.str() << std::endl;
            }
            
        }
        ImGui::EndToolbar();
        
        auto position = ImGui::GetCurrentWindow()->DC.CursorPos;
        m_mainSceneViewEditor->m_position.x = position.x;
        m_mainSceneViewEditor->m_position.y = position.y;
        auto size = ImGui::GetContentRegionAvail();
        m_sceneViewSize.x = static_cast<int>(size.x);
        m_sceneViewSize.y = static_cast<int>(size.y);
        m_mainSceneViewEditor->Render();
        auto& rt = m_mainSceneViewEditor->m_sceneViewRenderTexture;
        ImGui::Image((void*)rt->GetNativeTexturePtr(), size, ImVec2(0, 1), ImVec2(1, 0));
        ImGui::EndDock();
    }

    void EditorGUI::DrawMainMenu()
    {
        static float time_stamp = 0;

        // Main menu bar
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("New Scene", "Ctrl+N"))
                {
                    //Debug::LogWarning("New");
                    //std::files
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Edit"))
            {
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Assets"))
            {
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("GameObject"))
            {
                if (ImGui::MenuItem("Create Empty", "Ctrl+Shift+N"))
                {
                    Scene::CreateGameObject("GameObject");
                }
                if (ImGui::MenuItem("Create Empty Child", "Alt+Shift+N"))
                {

                }

                if (ImGui::BeginMenu("3D Object"))
                {
                    if (ImGui::MenuItem("Cube"))
                    {
                        GameObject::CreatePrimitive(PrimitiveType::Cube);
                    }
                    if (ImGui::MenuItem("Sphere"))
                    {
                        GameObject::CreatePrimitive(PrimitiveType::Sphere);
                    }
                    if (ImGui::MenuItem("Capsule"))
                    {
                        GameObject::CreatePrimitive(PrimitiveType::Capsule);
                    }
                    if (ImGui::MenuItem("Cylinder"))
                    {
                        GameObject::CreatePrimitive(PrimitiveType::Cylinder);
                    }
                    if (ImGui::MenuItem("Plane"))
                    {
                        GameObject::CreatePrimitive(PrimitiveType::Plane);
                    }
                    if (ImGui::MenuItem("Quad"))
                    {
                        GameObject::CreatePrimitive(PrimitiveType::Quad);
                    }
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("2D Object"))
                {
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Light"))
                {
                    //Light::Create();
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Audio"))
                {
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("UI"))
                {
                    ImGui::EndMenu();
                }
                if (ImGui::MenuItem("Particle System"))
                {
                }
                if (ImGui::MenuItem("Camera"))
                {
                    Camera::Create();
                }
                ImGui::EndMenu();
            }

            float new_time = (float)glfwGetTime();
            int fps = (int)roundf(1.f / float(new_time - time_stamp));
            time_stamp = new_time;
            std::string fps_str = "FPS: " + std::to_string(fps);
            auto fps_stats_size = ImGui::CalcTextSize(fps_str.c_str());
            ImGui::SameLine(ImGui::GetContentRegionMax().x - fps_stats_size.x);
            ImGui::Text("%s", fps_str.c_str());
            g_editorGUISettings.mainMenubarHeight = ImGui::GetWindowSize().y;
            ImGui::EndMainMenuBar();
        }
    }

    constexpr const char* ImGuiCol_Strings[] = {
        "ImGuiCol_Text",
        "ImGuiCol_TextDisabled",
        "ImGuiCol_WindowBg",
        "ImGuiCol_ChildWindowBg",
        "ImGuiCol_PopupBg",
        "ImGuiCol_Border",
        "ImGuiCol_BorderShadow",
        "ImGuiCol_FrameBg",
        "ImGuiCol_FrameBgHovered",
        "ImGuiCol_FrameBgActive",
        "ImGuiCol_TitleBg",
        "ImGuiCol_TitleBgCollapsed",
        "ImGuiCol_TitleBgActive",
        "ImGuiCol_MenuBarBg",
        "ImGuiCol_ScrollbarBg",
        "ImGuiCol_ScrollbarGrab",
        "ImGuiCol_ScrollbarGrabHovered",
        "ImGuiCol_ScrollbarGrabActive",
        "ImGuiCol_ComboBg",
        "ImGuiCol_CheckMark",
        "ImGuiCol_SliderGrab",
        "ImGuiCol_SliderGrabActive",
        "ImGuiCol_Button",
        "ImGuiCol_ButtonHovered",
        "ImGuiCol_ButtonActive",
        "ImGuiCol_Header",
        "ImGuiCol_HeaderHovered",
        "ImGuiCol_HeaderActive",
        "ImGuiCol_Column",
        "ImGuiCol_ColumnHovered",
        "ImGuiCol_ColumnActive",
        "ImGuiCol_ResizeGrip",
        "ImGuiCol_ResizeGripHovered",
        "ImGuiCol_ResizeGripActive",
        "ImGuiCol_CloseButton",
        "ImGuiCol_CloseButtonHovered",
        "ImGuiCol_CloseButtonActive",
        "ImGuiCol_PlotLines",
        "ImGuiCol_PlotLinesHovered",
        "ImGuiCol_PlotHistogram",
        "ImGuiCol_PlotHistogramHovered",
        "ImGuiCol_TextSelectedBg",
        "ImGuiCol_ModalWindowDarkening",
        "ImGuiCol_COUNT",
    };

    void EditorGUI::GUIStyleTweakWindow()
    {
        ImGuiContext& g = *GImGui;
        ImGuiStyle& style = g.Style;
        ImGui::Begin("style tweak");
        for (int i = 0; i < ImGuiCol_COUNT; ++i)
        {
            //const std::string label = "color-" + std::to_string(i);
            ImGui::ColorEdit4(ImGuiCol_Strings[i], &style.Colors[i].x);
        }
        ImGui::End();
    }

    void EditorGUI::DrawMainToolbar()
    {
        auto frame_padding = ImGui::GetStyle().FramePadding;
        float padding = frame_padding.y * 2;
        float height = 24 + padding;
        ImVec2 toolbar_size(ImGui::GetIO().DisplaySize.x, height);
        //auto size = ImGui::GetIO().DisplaySize;
        if (ImGui::BeginToolbar("MainToolBar", ImVec2(0, g_editorGUISettings.mainMenubarHeight), toolbar_size))
        {
            ImGui::SameLine();
            if (FishEditorWindow::InPlayMode())
            {
                if (ImGui::Button("Stop"))
                {
                    FishEditorWindow::Stop();
                }
            }
            else
            {
                if (ImGui::Button("Play"))
                {
                    FishEditorWindow::Play();
                }
            }

            ImGui::SameLine();
            if (ImGui::Button("Translate"))
            {
                m_mainSceneViewEditor->m_transformToolType = TransformToolType::Translate;
            }

            ImGui::SameLine();
            if (ImGui::Button("Rotate"))
            {
                m_mainSceneViewEditor->m_transformToolType = TransformToolType::Rotate;
            }

            ImGui::SameLine();
            if (ImGui::Button("Scale"))
            {
                m_mainSceneViewEditor->m_transformToolType = TransformToolType::Scale;
            }

            ImGui::SameLine();
            if (ImGui::Button(ToString(m_mainSceneViewEditor->m_transformSpace)))
            {
                m_mainSceneViewEditor->m_transformSpace = m_mainSceneViewEditor->m_transformSpace == TransformSpace::Global ?
                    TransformSpace::Local : TransformSpace::Global;
            }
        }
        ImGui::EndToolbar();
        g_editorGUISettings.mainToolbarHeight = height;
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
    void EditorGUI::OnInspectorGUI(const FishEngine::TransformPtr& transform)
    {
        if (Float3("Position", &transform->m_localPosition)) {
            transform->MakeDirty();
        }
        Vector3 localEulerAngles = transform->m_localRotation.eulerAngles();
        if (Float3("Rotation", &localEulerAngles)) {
            transform->setLocalEulerAngles(localEulerAngles);
            //transform->m_localRotation.setEulerAngles(localEulerAngles);
            //transform->MakeDirty();
        }
        if (Float3("Scale", &transform->m_localScale)) {
            transform->MakeDirty();
        }
    }

    template<>
    void EditorGUI::OnInspectorGUI(const FishEngine::CameraPtr& camera)
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
    void EditorGUI::OnInspectorGUI(const FishEngine::AnimatorPtr& animator)
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
    void EditorGUI::OnInspectorGUI(const FishEngine::MeshFilterPtr& meshFilter)
    {
        if (ImGui::Button("Change")) {
            ImGui::OpenPopup("Select ...");
        }
        EditorGUI::SelectMeshDialogBox([&meshFilter](MeshPtr mesh)->void {
            meshFilter->SetMesh(mesh);
        });
        //ImGui::SameLine();
        if (meshFilter->m_mesh == nullptr)
        {
            ImGui::LabelText("Mesh", "None");
        }
        else
        {
            if (meshFilter->m_mesh->m_skinned) {
                int boneCount = (int)meshFilter->m_mesh->m_boneNameToIndex.size();
                Int("Bone Count", boneCount);
            }
            ImGui::LabelText("Mesh", "%s", meshFilter->m_mesh->name().c_str());
        }
    }

    template<>
    void EditorGUI::OnInspectorGUI(const MaterialPtr& material)
    {
        if (ImGui::CollapsingHeader(material->m_name.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::Indent();
            auto& uniforms = material->m_shader->uniforms();
            for (auto& u : uniforms)
            {
                if (u.type == GL_FLOAT)
                {
                    ImGui::SliderFloat(u.name.c_str(), &material->m_uniforms.floats[u.name], 0, 1);
                }
                else if (u.type == GL_FLOAT_VEC3)
                {
                    ImGui::InputFloat3(u.name.c_str(), material->m_uniforms.vec3s[u.name].data());
                }
                else if (u.type == GL_FLOAT_VEC4)
                {
                    ImGui::InputFloat4(u.name.c_str(), material->m_uniforms.vec4s[u.name].data());
                }
                else if (u.type == GL_SAMPLER_2D)
                {
                    auto& tex = material->m_textures[u.name];
                    ImGui::LabelText(u.name.c_str(), "%s", tex->name().c_str());
                    ImGui::Image((void*)tex->GetNativeTexturePtr(), ImVec2(64, 64));
                    ImGui::SameLine();
                    ImGui::Button("Select");
                }
            }
            ImGui::Unindent();
        }
    }

    template<>
    void EditorGUI::OnInspectorGUI(const FishEngine::RendererPtr& renderer)
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
    void EditorGUI::OnInspectorGUI(const FishEngine::MeshRendererPtr& renderer)
    {
        OnInspectorGUI<Renderer>(renderer);
    }


    template<>
    void EditorGUI::OnInspectorGUI(const FishEngine::SkinnedMeshRendererPtr& renderer)
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
    void EditorGUI::OnInspectorGUI(const FishEngine::RigidbodyPtr& rigidBody)
    {
        Float("Mass", &rigidBody->m_mass);
        Float("Drag", &rigidBody->m_drag);
        Float("Angular", &rigidBody->m_angularDrag);
        Bool("Use Gravity", &rigidBody->m_useGravity);
        Bool("Is Kinematic", &rigidBody->m_isKinematic);
    }


    template<>
    void EditorGUI::OnInspectorGUI(const FishEngine::BoxColliderPtr& boxCollider)
    {
        Bool("Is Trigger", &boxCollider->m_isTrigger);
        Float3("Center", &boxCollider->m_center);
        Float3("Size", &boxCollider->m_size);
    }

    template<>
    void EditorGUI::OnInspectorGUI(const FishEngine::SphereColliderPtr& sphereCollider)
    {
        Bool("Is Trigger", &sphereCollider->m_isTrigger);
        Float3("Center", &sphereCollider->m_center);
        Float("Radius", &sphereCollider->m_radius);
    }


    template<>
    void EditorGUI::OnInspectorGUI(const FishEngine::CapsuleColliderPtr& capsuleCollider)
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
    void EditorGUI::OnInspectorGUI(const FishEngine::LightPtr& light)
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
    void EditorGUI::OnInspectorGUI(const FishEngine::TexturePtr& texture)
    {
        ImGui::Image((void *)texture->GetNativeTexturePtr(), ImVec2(128,128));
    }

    template<>
    void EditorGUI::OnInspectorGUI(const FishEngine::ComponentPtr& component)
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
    
    FishEngine::Int2 EditorGUI::sceneViewSize()
    {
        return m_sceneViewSize;
    }
}
