#include "SceneViewEditor.hpp"

#include <RenderTexture.hpp>
#include <RenderSystem.hpp>
#include <Camera.hpp>
#include <Shader.hpp>
#include <Color.hpp>
#include <MeshFilter.hpp>
#include <Mesh.hpp>
#include <MeshRenderer.hpp>
#include <SkinnedMeshRenderer.hpp>
#include <Material.hpp>
#include <Pipeline.hpp>
#include <Screen.hpp>
#include <Scene.hpp>
#include <GameObject.hpp>
#include <Input.hpp>
#include <ModelImporter.hpp>
#include <Gizmos.hpp>
#include <CameraController.hpp>
#include <Graphics.hpp>
#include <RenderTarget.hpp>

#include "Selection.hpp"
#include "FishEditorWindow.hpp"
#include "EditorGUI.hpp"

using namespace FishEngine;

namespace FishEditor
{

    MaterialPtr sceneGizmoMaterial = nullptr;
    MeshPtr cubeMesh = nullptr;
    MeshPtr coneMesh = nullptr;
    SimpleMeshPtr gridMesh = nullptr;

    void SceneViewEditor::Init()
    {
        m_camera = Camera::Create(60.0, 0.3f, 1000.f, CameraType::SceneView);
        m_cameraGameObject = std::make_shared<GameObject>("EditorCamera");
        m_cameraGameObject->transform()->m_gameObject = m_cameraGameObject;
        //auto camera_go = Scene::CreateGameObject("MainCamera");
        m_cameraGameObject->AddComponent<CameraController>();
        m_cameraGameObject->AddComponent(m_camera);
        m_cameraGameObject->transform()->setLocalPosition(5, 5, 5);
        m_cameraGameObject->transform()->LookAt(0, 0, 0);
        //m_cameraGameObject->SetTag("MainCamera");

        sceneGizmoMaterial = Material::InstantiateBuiltinMaterial("VertexLit-Internal");
        cubeMesh = Model::builtinModel(PrimitiveType::Cube)->mainMesh();
        coneMesh = Model::builtinModel(PrimitiveType::Cone)->mainMesh();
        //ImGui::GetNamedDockPositionAndSize("Scene", &m_position.x, &m_position.y, &m_size.x, &m_size.y);
        m_size = EditorGUI::sceneViewSize();
        //m_sceneViewRenderTexture = RenderTexture::CreateColorMap(m_size.x, m_size.y);
        m_colorBuffer = ColorBuffer::Create(m_size.x, m_size.y);
        m_colorBuffer->setName("SceneViewColor");
        m_depthBuffer = DepthBuffer::Create(m_size.x, m_size.y);
        m_depthBuffer->setName("SceneViewDepth");
        m_sceneViewRenderTarget = std::make_shared<RenderTarget>();
        m_sceneViewRenderTarget->Set(m_colorBuffer, m_depthBuffer);

        m_selectionOutlineRT = std::make_shared<RenderTarget>();
        m_selectionOutlineDepthBuffer = DepthBuffer::Create(m_size.x, m_size.y);
        m_selectionOutlineDepthBuffer->setName("SelectionOutlineDepth");
        m_selectionOutlineDepthBuffer->setFilterMode(FilterMode::Point);
        m_selectionOutlineDepthBuffer->setWrapMode(TextureWrapMode::Repeat);
        m_selectionOutlineRT->SetDepthBufferOnly(m_selectionOutlineDepthBuffer);

        m_selectionOutlineRT2 = std::make_shared<RenderTarget>();
        m_selectionOutlineColorBuffer2 = ColorBuffer::Create(m_size.x, m_size.y);
        m_selectionOutlineColorBuffer2->setName("SelectionOutlineColor");
        m_selectionOutlineColorBuffer2->setFilterMode(FilterMode::Point);
        m_selectionOutlineColorBuffer2->setWrapMode(TextureWrapMode::Clamp);
        m_selectionOutlineRT2->SetColorBufferOnly(m_selectionOutlineColorBuffer2);

        constexpr int rows = 10;
        constexpr int vertex_count = (rows * 2 + 1) * 2 * 2;
        float grid_vertex[vertex_count * 3];
        constexpr int step = 1;
        float* p = grid_vertex;
        for (int i = -rows; i <= rows; i++)
        {
            float x = static_cast<float>(i*step);
            *p = x; p++;
            *p = 0; p++;
            *p = -rows; p++;
            *p = x; p++;
            *p = 0; p++;
            *p = rows; p++;

            *p = -rows; p++;
            *p = 0; p++;
            *p = x; p++;
            *p = rows; p++;
            *p = 0; p++;
            *p = x; p++;
        }
        gridMesh = std::make_shared<SimpleMesh>(grid_vertex, vertex_count, GL_LINES);
    }

    void SceneViewEditor::Update()
    {
        m_cameraGameObject->GetComponent<CameraController>()->Update();
    }

    void SceneViewEditor::Render()
    {
        auto tsize = EditorGUI::sceneViewSize();
        if (tsize.x != m_size.x || tsize.y != m_size.y)
        {
            m_size = tsize;
            const int ix = static_cast<int>(m_size.x * Screen::pixelsPerPoint());
            const int iy = static_cast<int>(m_size.y * Screen::pixelsPerPoint());
            m_colorBuffer->Resize(ix, iy);
            m_depthBuffer->Resize(ix, iy);
            m_selectionOutlineDepthBuffer->Resize(ix, iy);
            m_selectionOutlineColorBuffer2->Resize(ix, iy);
            RenderSystem::ResizeBufferSize(ix, iy);
            Camera::OnWindowSizeChanged(ix, iy);
            Screen::m_width = ix;
            Screen::m_height = iy;
        }
        Pipeline::PushRenderTarget(m_sceneViewRenderTarget);
        glViewport(0, 0, Screen::m_width, Screen::m_height);

        RenderSystem::Render();
         
        /************************************************************************/
        /* Grid                                                                 */
        /************************************************************************/
        if (false)
        {
            auto view = Camera::main()->worldToCameraMatrix();
            auto proj = Camera::main()->projectionMatrix();
            auto shader = Shader::builtinShader("SolidColor-Internal");
            shader->Use();
            shader->BindUniformVec4("_Color", Color::gray);
            shader->BindUniformMat4("MATRIX_MVP", proj*view);
            gridMesh->Render();
        }


        /************************************************************************/
        /* Selection                                                            */
        /************************************************************************/
#if 0
        if (m_highlightSelections)
        {
            auto camera = Camera::main();
            //auto view = camera->worldToCameraMatrix();
            //auto proj = camera->projectionMatrix();
            //auto vp = proj * view;
            glEnable(GL_POLYGON_OFFSET_LINE);
            glPolygonOffset(-1.0, -1.0f);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            auto material = Material::builtinMaterial("SolidColor");
            material->DisableKeyword(ShaderKeyword::All);
            material->SetVector4("Color", Vector4(0.375f, 0.388f, 0.463f, 1));


            std::list<GameObjectPtr> selections;
            auto go = Selection::selectedGameObjectInHierarchy();
            selections.push_back(go);
            while (!selections.empty())
            {
                go = selections.front();
                selections.pop_front();
                if (go == nullptr)
                {
                    continue;
                }
                auto& children = go->transform()->children();
                for (auto& c : children)
                {
                    selections.push_back(c.lock()->gameObject());
                }
                MeshPtr mesh;
                auto meshFilter = go->GetComponent<MeshFilter>();
                if (meshFilter != nullptr)
                {
                    mesh = meshFilter->mesh();
                }
                else
                {
                    auto skinnedMeshRenderer = go->GetComponent<SkinnedMeshRenderer>();
                    if (skinnedMeshRenderer != nullptr)
                    {
                        mesh = skinnedMeshRenderer->sharedMesh();
                        bool useSkinnedVersion = FishEditorWindow::InPlayMode();
                        if (useSkinnedVersion)
                        {
                            material->EnableKeyword(ShaderKeyword::SkinnedAnimation);
                            Pipeline::UpdateBonesUniforms(skinnedMeshRenderer->m_matrixPalette);
                        }
                    }
                }
                if (mesh != nullptr)
                {
                    auto model = go->transform()->localToWorldMatrix() * Matrix4x4::Scale(1.001f, 1.001f, 1.001f);
                    Graphics::DrawMesh(mesh, model, material);
                }
            }
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glDisable(GL_POLYGON_OFFSET_LINE);
        }
#endif
        auto selectedGO = Selection::selectedGameObjectInHierarchy();
        if (m_highlightSelections && selectedGO != nullptr)
        {
            Pipeline::PushRenderTarget(m_selectionOutlineRT);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
            auto material = Material::builtinMaterial("SolidColor");
            material->SetVector4("Color", Vector4(1, 0, 1, 1));
            std::list<GameObjectPtr> selections;
            selections.push_back(selectedGO);
            while (!selections.empty())
            {
                auto go = selections.front();
                selections.pop_front();
                if (go == nullptr)
                {
                    continue;
                }
                auto& children = go->transform()->children();
                for (auto& c : children)
                {
                    selections.push_back(c.lock()->gameObject());
                }
                MeshPtr mesh;
                auto meshFilter = go->GetComponent<MeshFilter>();
                if (meshFilter != nullptr)
                {
                    mesh = meshFilter->mesh();
                }
                else
                {
                    auto skinnedMeshRenderer = go->GetComponent<SkinnedMeshRenderer>();
                    if (skinnedMeshRenderer != nullptr)
                    {
                        mesh = skinnedMeshRenderer->sharedMesh();
                        bool useSkinnedVersion = FishEditorWindow::InPlayMode();
                        if (useSkinnedVersion)
                        {
                            material->EnableKeyword(ShaderKeyword::SkinnedAnimation);
                            Pipeline::UpdateBonesUniforms(skinnedMeshRenderer->m_matrixPalette);
                        }
                    }
                }
                if (mesh != nullptr)
                {
                    auto model = go->transform()->localToWorldMatrix() * Matrix4x4::Scale(1.001f, 1.001f, 1.001f);
                    Graphics::DrawMesh(mesh, model, material);
                }
            }
            Pipeline::PopRenderTarget();

            Pipeline::PushRenderTarget(m_selectionOutlineRT2);
            glClear(GL_COLOR_BUFFER_BIT);
            auto selection_outline_mtl = Material::builtinMaterial("PostProcessSelectionOutline");
            auto quad = Model::builtinMesh(PrimitiveType::Quad);
            selection_outline_mtl->SetTexture("StencilTexture", m_selectionOutlineDepthBuffer);
            selection_outline_mtl->SetTexture("ColorTexture", m_colorBuffer);
            selection_outline_mtl->SetTexture("DepthTexture", RenderSystem::m_mainDepthBuffer);
            Graphics::DrawMesh(quad, selection_outline_mtl);
            Pipeline::PopRenderTarget();

            m_selectionOutlineRT2->AttachForRead();
            auto w = m_colorBuffer->width();
            auto h = m_colorBuffer->height();
            glBlitFramebuffer(0, 0, w, h, 0, 0, w, h, GL_COLOR_BUFFER_BIT, GL_NEAREST);
        }

        if (m_isWireFrameMode)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        if (m_useGammaCorrection)
            glEnable(GL_FRAMEBUFFER_SRGB);
        else
            glDisable(GL_FRAMEBUFFER_SRGB);

        if (m_isWireFrameMode)
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        /************************************************************************/
        /* Selection Gizmos                                                     */
        /************************************************************************/
        auto go = Selection::selectedGameObjectInHierarchy();
        if (go != nullptr)
            go->OnDrawGizmosSelected();

        /************************************************************************/
        /* Camera Preview                                                       */
        /************************************************************************/
        //glClear(GL_DEPTH_BUFFER_BIT);
        //auto selectedGO = Selection::selectedGameObjectInHierarchy();
        auto camera_preview = selectedGO == nullptr ? nullptr : selectedGO->GetComponent<Camera>();
        if (camera_preview != nullptr)
        {
            const int w = Screen::width();
            const int h = Screen::height();
            //auto v = camera_preview->viewport();
            const int preview_width = static_cast<int>(w * 0.25f);
            const int preview_height = static_cast<int>(h * 0.25f);
            constexpr int padding = 20;
            const int preview_pos_x = w - preview_width - padding;
            const int preview_pos_y = padding;
            glViewport(preview_pos_x, preview_pos_y, preview_width, preview_height);
            auto main_camera = Camera::main();
            Screen::m_width = static_cast<int>(w*0.25f);
            Screen::m_height = static_cast<int>(h*0.25f);
            Camera::m_mainCamera = camera_preview;

            Pipeline::BindCamera(camera_preview);
            //Pipeline::BindPerFrameUniforms();
            Scene::Render();
            //RenderSystem::Render();
            Camera::m_mainCamera = main_camera;
            Screen::m_width = w;
            Screen::m_height = h;
        }

        //glClear(GL_DEPTH_BUFFER_BIT);
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
        
        if (!m_mouseEventHandled && Input::GetMouseButtonDown(0))
        {
            Ray ray = Camera::main()->ScreenPointToRay(Input::mousePosition());
            auto go = Scene::IntersectRay(ray);
            Selection::setActiveGameObject(go);
        }

        Pipeline::PopRenderTarget();
    }

    void SceneViewEditor::DrawTranslateGizmo()
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
            view.IntersectPlane(axis[1], center, t + 1);
            view.IntersectPlane(axis[2], center, t + 2);
            float tmid;
            if (t[0] >= t[1])
            {
                if (t[1] >= t[2])
                    tmid = t[1];
                else if (t[0] >= t[2])
                    tmid = t[2];
                else
                    tmid = t[0];
            }
            else
            {
                if (t[0] >= t[2])
                    tmid = t[0];
                else if (t[1] >= t[2])
                    tmid = t[2];
                else
                    tmid = t[1];
            }
            Vector3 p = view.GetPoint(tmid);
            Vector3 d = p - center;
            if (d.magnitude() <= translate_gizmo_length * 1.3f)
            {
                d.Normalize();
                for (int i = 0; i < 3; ++i)
                {
                    if (Vector3::Dot(d, axis[i]) > 0.96f)
                    {
                        m_selectedAxis = i;
                        //m_mouseEventHandled = true;
                        break;
                    }
                }
            }
        }

        auto shader = sceneGizmoMaterial->shader();
        shader->Use();
        //sceneGizmoMaterial->SetVector3("unity_LightPosition", Vector3(0, 0, -1));
        auto view = camera->worldToCameraMatrix();
        auto proj = camera->projectionMatrix();
        auto vp = proj * view;

        ShaderUniforms uniforms;


        //Color colors[3] = { Color::red, Color::green, Color::blue };

        float f[] = {
            1,  0,  0,   0, 0, -90,    // red axis, +x
            0,  1,  0,   0, 0,   0,    // green axis, +y
            0,  0,  1,  90, 0,   0,    // blue axis, +z
        };

        FishEngine::Matrix4x4 model;

        for (int i = 0; i < 3; ++i)
        {
            int j = 6 * i;
            Vector3 pos = center + axis[i] * translate_gizmo_length;
            Quaternion rot = Quaternion::FromToRotation(Vector3::up, axis[i]);
            model.SetTRS(pos, rot, Vector3(1, 1.5, 1) * 0.015f);
            Vector3 color = m_selectedAxis == i ? Vector3(1, 1, 0) : Vector3(f + j);
            sceneGizmoMaterial->SetVector3("_Color", color);
            sceneGizmoMaterial->SetMatrix("MATRIX_MVP", vp * model);
            sceneGizmoMaterial->SetMatrix("MATRIX_IT_MV", (view*model).inverse().transpose());
            shader->Use();
            shader->PreRender();
            sceneGizmoMaterial->BindProperties();
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

        static std::function<void(void)> undo_function;
        static std::function<void(void)> redo_function;

        // handle mouse movement event
        if (Input::GetMouseButtonDown(0))   // start
        {
            m_mouseEventHandled = true;
            lastCenter = center;
            Ray ray = Camera::main()->ScreenPointToRay(Input::mousePosition());
            float t = solve(center, axis_selected, camera_pos, ray.direction);
            lastMousePosition = ray.GetPoint(t);
            const auto position = selectedGO->transform()->position();
            undo_function = [selectedGO, position]() {
                Debug::LogWarning("Undo translation");
                selectedGO->transform()->setPosition(position);
            };
        }
        else if (Input::GetMouseButton(0))       // moving
        {
            Vector3 mouse_movement(Input::GetAxis(Axis::MouseX)*Screen::width(), Input::GetAxis(Axis::MouseY)*Screen::height(), 0); // in piexls
            Vector3 axis_on_plane = vp.MultiplyVector(axis_selected);
            axis_on_plane.z = 0;
            axis_on_plane.Normalize();                              // axis on the near clip plane
            Vector3 p = Input::mousePosition() - mouse_movement + Vector3::Project(mouse_movement, axis_on_plane);   // new mouse position(along the axis)
            Vector3 new_view_dir = Camera::main()->ScreenPointToRay(p).direction;

            // solve: camera_pos + t1 * new_view_dir = lastMousePosition + t2 * axis
            float t = solve(camera_pos, new_view_dir, lastMousePosition, axis_selected);
            selectedGO->transform()->setPosition(lastCenter + t*axis_selected);
        }
        else if (Input::GetMouseButtonUp(0))    // end
        {
            m_mouseEventHandled = false;
            const auto position = selectedGO->transform()->position();
            redo_function = [selectedGO, position]() {
                Debug::LogWarning("Redo translation");
                selectedGO->transform()->setPosition(position);
            };
            CommandManager::AddCommand(std::make_pair(undo_function, redo_function));
        }
    }

    void SceneViewEditor::DrawRotateGizmo()
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
        
        static std::function<void(void)> undo_function;
        static std::function<void(void)> redo_function;

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
                    if (std::fabsf(Vector3::Dot(axis[i], dir)) < 0.1f)
                    {
                        m_mouseEventHandled = true;
                        m_selectedAxis = i;
                        lastFromDir = Vector3::Cross(axis[i], Vector3::Cross(dir, axis[i]));
                        lastFromDir.Normalize();
                        lastRotation = selectedGO->transform()->localRotation();
                        
                        const auto rotation = selectedGO->transform()->rotation();
                        undo_function = [selectedGO, rotation]() {
                            Debug::LogWarning("Undo rotation");
                            selectedGO->transform()->setRotation(rotation);
                        };

                    }
                }
            }
        }

        //Gizmos::DrawWireSphere(center, 0.1f);
        Vector3 view_inverse = -camera->transform()->forward();
        Color colors[3] = { Color::red, Color::green, Color::blue };
        for (int i = 0; i < 3; ++i)
        {
            Gizmos::setColor(colors[i]);
            //Gizmos::DrawLine(gizmo_center, gizmo_center+axis[i] * radius);
            if (m_selectedAxis == i)
                Gizmos::setColor(Color::yellow);
            Vector3 dir2 = Vector3::ProjectOnPlane(view_inverse, axis[i]);
            dir2.Normalize();
            Gizmos::DrawHalfCircle(gizmo_center, radius, axis[i], dir2);
            //Gizmos::DrawCircle(gizmo_center, radius, axis[i]);
        }

        Gizmos::setColor(Color::white);
        Gizmos::DrawCircle(gizmo_center, radius, camera->transform()->forward());
        Gizmos::DrawCircle(gizmo_center, radius*1.1f, camera->transform()->forward());

        if (m_selectedAxis < 0)
            return;


        if (Input::GetMouseButton(0)) // rotating
        {
            Ray view = camera->ScreenPointToRay(Input::mousePosition());
            auto& face_normal = axis[m_selectedAxis];
            float t;
            if (!view.IntersectPlane(face_normal, center, &t))
            {
                return;
            }
            Vector3 intersected_p = view.GetPoint(t);
            Vector3 dir_to = Vector3::Normalize(intersected_p - center);
            auto rot = Quaternion::FromToRotation(lastFromDir, dir_to) * lastRotation;
            selectedGO->transform()->setLocalRotation(rot);

            Gizmos::setColor(Color::yellow);
            Gizmos::DrawLine(gizmo_center, gizmo_center + lastFromDir * radius);
            Gizmos::DrawLine(gizmo_center, gizmo_center + dir_to * radius);
        }
        else if (Input::GetMouseButtonUp(0)) // end
        {
            m_mouseEventHandled = false;
            const auto rotation = selectedGO->transform()->rotation();
            redo_function = [selectedGO, rotation]() {
                Debug::LogWarning("Redo rotation");
                selectedGO->transform()->setRotation(rotation);
            };
            CommandManager::AddCommand(std::make_pair(undo_function, redo_function));
        }
    }

    void SceneViewEditor::DrawScaleGizmo()
    {

    }

    void SceneViewEditor::DrawSceneGizmo()
    {
        const int vp_width = static_cast<int>(64*Screen::pixelsPerPoint());
        constexpr int margin = 20;
        const int vp_x = Screen::width() - vp_width - margin;
        const int vp_y = Screen::height() - vp_width - margin;
        glViewport(vp_x, vp_y, vp_width, vp_width);

        auto shader = sceneGizmoMaterial->shader();
        shader->Use();

        Vector3 camera_pos(0, 0, -5);
        //sceneGizmoMaterial->SetVector3("unity_LightPosition", camera_pos.normalized());
        auto camera = Camera::main();
        auto view = Matrix4x4::LookAt(camera_pos, Vector3(0, 0, 0), Vector3(0, 1, 0));
        auto proj = camera->m_orthographic ? Matrix4x4::Ortho(-2, 2, -2, 2, 1, 10) : Matrix4x4::Perspective(45, 1, 0.3f, 10.f);
        auto vp = proj * view;
        
        // world to camera;
        auto model = Matrix4x4::FromRotation(Quaternion::Inverse(camera->transform()->rotation()));

        // camera to world
        auto inv_model = model.inverse();

        auto mousePos = Input::mousePosition();
        bool inRegion = false;
        float x = mousePos.x - vp_x;
        float y = mousePos.y - vp_y;
        Ray ray(Vector3::zero, Vector3::zero);
        if (x > 0 && x < vp_width && y > 0 && y < vp_width)
        {
            inRegion = true;
            ray.origin = inv_model.MultiplyPoint(2.0f*x / vp_width - 1.f, 2.0f*y / vp_width - 1.0f, -5);
            ray.direction = inv_model.MultiplyVector(Vector3::forward);
        }

        auto s = Matrix4x4::Scale(0.5f, 0.75f, 0.5f);

        float f[] = {
            -1,  0,  0,   0, 0, -90,
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
        for (int i = 0; i < 6; ++i)
        {
            int j = i * 6;
            Vector3 pos(f + j);
            //pos*=1.5f;
            Vector3 color(1, 1, 1);
            if (i >= 3)
                color = Vector3(f + j);
            if (inRegion && !interested)
            {
                Bounds aabb(pos*0.7f, Vector3::one*0.5f);
                if (aabb.IntersectRay(ray))
                {
                    interested = true;
                    hoverIndex = i;
                    color.Set(1, 1, 0); // yellow
                }
            }
            t.setLocalPosition(pos);
            t.setLocalEulerAngles(f[j + 3], f[j + 4], f[j + 5]);
            auto modelMat = model * t.localToWorldMatrix() * s;
            sceneGizmoMaterial->SetVector3("_Color", color);
            sceneGizmoMaterial->SetMatrix("MATRIX_MVP", vp * modelMat);
            sceneGizmoMaterial->SetMatrix("MATRIX_IT_MV", (view*modelMat).inverse().transpose());
            sceneGizmoMaterial->SetVector3("_Color", color);
            sceneGizmoMaterial->BindProperties();
            coneMesh->Render();
        }

        Bounds aabb(Vector3::zero, Vector3::one*0.5f);
        bool interested6 = false;
        if (inRegion && !interested)
        {
            interested6 = aabb.IntersectRay(ray);
            if (interested6)
                hoverIndex = 6;
        }
        interested = interested || interested6;

        sceneGizmoMaterial->SetMatrix("MATRIX_MVP", vp * model);
        sceneGizmoMaterial->SetMatrix("MATRIX_IT_MV", (view*model).inverse().transpose());
        sceneGizmoMaterial->SetVector3("_Color", interested6 ? Vector3(1, 1, 0) : Vector3(1, 1, 1));
        shader->PreRender();
        sceneGizmoMaterial->BindProperties();
        shader->CheckStatus();
        cubeMesh->Render();

        shader->PostRender();

        if (interested && Input::GetMouseButtonDown(0))
        {
            Debug::Log("%d", hoverIndex);
            if (hoverIndex == 6)
            {
                camera->setOrthographic(!camera->orghographic());
            }
            else
            {
                Vector3 offset(f + hoverIndex * 6);
                Vector3 up(0, 1, 0);
                if (offset.y == 1) {
                    up.Set(0, 0, -1);
                } else if (offset.y == -1) {
                    up.Set(0, 0, 1);
                }
                camera->transform()->setLocalPosition(camera->m_focusPoint + offset * 4);
                camera->transform()->LookAt(camera->m_focusPoint, up);
            }
        }

        auto v = camera->viewport();
        int w = Screen::width();
        int h = Screen::height();
        glViewport(GLint(w*v.x), GLint(h*v.y), GLsizei(w*v.z), GLsizei(h*v.w));
    }
}
