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

#include "FishEditorWindow.hpp"
#include "Selection.hpp"
#include "EditorRenderSystem.hpp"


using namespace FishEngine;

namespace FishEditor
{
    constexpr float translate_gizmo_length = 0.1f;

    constexpr float inspector_indent_width = 4;

    TransformToolType EditorGUI::m_transformToolType = TransformToolType::Translate;
    int     EditorGUI::m_idCount = 0;
    bool    EditorGUI::s_locked = false;
    int     EditorGUI::m_selectedAxis = -1;
    bool    EditorGUI::s_isAnyItemClicked = false;
    bool    EditorGUI::m_showAssectSelectionDialogBox = false;

    std::weak_ptr<FishEngine::GameObject> FishEditor::EditorGUI::m_lastSelectedGameObject;
    PMaterial sceneGizmoMaterial = nullptr;
    PMesh cubeMesh = nullptr;
    PMesh coneMesh = nullptr;

    void EditorGUI::Init()
    {
#if FISHENGINE_PLATFORM_WINDOWS
        const std::string root_dir = "../../assets/";
#else
        const std::string root_dir = "/Users/yushroom/program/graphics/FishEngine/assets/";
#endif

        ImGuiIO& io = ImGui::GetIO();
        io.Fonts->AddFontFromFileTTF((root_dir + "fonts/DroidSans.ttf").c_str(), 14.0f);

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
        cubeMesh = Model::builtinModel(BuiltinModelType::Cube)->mainMesh();
        coneMesh = Model::builtinModel(BuiltinModelType::Cone)->mainMesh();
    }

    void EditorGUI::Update()
    {
        auto selectedGO = Selection::selectedGameObjectInHierarchy();

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
        //DrawToolBar();
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
            auto selectedGO = Selection::selectedGameObjectInHierarchy();
            if (selectedGO == nullptr)
                return false;

            Vector3 center = selectedGO->transform()->position();
            Vector3 camera_pos = Camera::main()->transform()->position();
            center = Vector3::Normalize(center - camera_pos) + camera_pos;

            constexpr float d = 0.03f;
            Bounds aabb[3];
            for (int i = 0; i < 3; ++i) {
                Vector3 v(0, 0, 0);
                v[i] = translate_gizmo_length / 2.f;
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

    void EditorGUI::HierarchyItem(std::shared_ptr<GameObject> gameObject)
    {
        ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow;
        if (gameObject == Selection::selectedGameObjectInHierarchy())
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
            if (!s_locked) {
                Selection::setActiveGameObject(gameObject);
            }
            Selection::setSelectedGameObjectInHierarchy(gameObject);
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
        m_idCount++;
        if (!gameObject->activeSelf()) {
            ImGui::PopStyleColor();
        }
    }

    void FishEditor::EditorGUI::DrawHierarchyWindow()
    {
        s_isAnyItemClicked = false;
        //auto selectedGO = Selection::activeGameObject();
        auto selectedGO = Selection::selectedGameObjectInHierarchy();
        // Hierarchy view
        //ImGui::BeginDock("Hierarchy");
        ImGui::Begin("Hierarchy");
        if (ImGui::Button("Create"))
        {
            s_isAnyItemClicked = true;
            auto go = Scene::CreateGameObject("GameObject");
            if (Selection::selectedGameObjectInHierarchy() != nullptr)
            {
                go->transform()->SetParent(Selection::selectedGameObjectInHierarchy()->transform());
            }
        }

        //if (selectedGO != nullptr)
        //{
        //    ImGui::SameLine();
        //    if (ImGui::Button("Destroy"))
        //    {
        //        s_isAnyItemClicked = true;
        //        Object::DestroyImmediate(selectedGO);
        //        Selection::setActiveGameObject(nullptr);
        //    }
        //}

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

        // TODO: remove this
        if (!s_isAnyItemClicked && ImGui::IsMouseClicked(0) && ImGui::IsMouseHoveringWindow()) {
            Selection::setSelectedGameObjectInHierarchy(nullptr);
            if (!s_locked)
                Selection::setActiveGameObject(nullptr);
            Selection::setSelectedGameObjectInHierarchy(nullptr);
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
        /* Add Component                                                        */
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
                ImGui::Text("The Component %s can't be added \n because %s already contains the same component.", failedComponentName, selectedGO->name().c_str());
                if (ImGui::Button("Cancel"))
                    ImGui::CloseCurrentPopup();
                ImGui::EndPopup();
            }
        }

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

            float new_time = (float)glfwGetTime();
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

    void FishEditor::EditorGUI::DrawToolBar()
    {
        ImGui::Begin("ToolBar");
        ImGui::Button("Play");
        ImGui::End();
    }

    void FishEditor::EditorGUI::DrawTranslateGizmo()
    {
        //Gizmos::DrawLine(Vector3(0, 0, 0), Vector3(1, 0, 0));

        auto selectedGO = Selection::selectedGameObjectInHierarchy();
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
            t.setLocalEulerAngles(f + j + 3);
            Vector3 pos = center + Vector3(f + j)*translate_gizmo_length;
            t.setLocalPosition(pos);
            auto model = t.localToWorldMatrix();
            Pipeline::perDrawUniformData.MATRIX_MVP = vp*model;
            Pipeline::perDrawUniformData.MATRIX_IT_MV = view*model;
            Pipeline::BindPerDrawUniforms();
            //sceneGizmoMaterial->SetMatrix("MATRIX_MVP", vp*model);
            //sceneGizmoMaterial->SetMatrix("MATRIX_IT_MV", view*model);
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
            camera->m_orthographic = !camera->m_orthographic;
            camera->m_isDirty = true;
        }
        //m_orthographic = list_item_current == 1;

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
                    ImGui::LabelText(u.name.c_str(), "%s", material->m_textures[u.name]->name().c_str());
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
#undef Case
    }
}
