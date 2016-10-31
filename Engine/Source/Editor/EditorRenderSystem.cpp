#include "EditorRenderSystem.hpp"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw_gl3.h>

#include <Camera.hpp>
#include <MeshRenderer.hpp>
#include <SkinnedMeshRenderer.hpp>
#include <Scene.hpp>
#include <Mesh.hpp>
#include <MeshFilter.hpp>
#include <Pipeline.hpp>
#include <GameObject.hpp>
#include <RenderSystem.hpp>
#include <Material.hpp>
#include <Color.hpp>

#include "FishEditorWindow.hpp"
#include "EditorGUI.hpp"
#include "Selection.hpp"

using namespace FishEngine;

namespace FishEditor
{
    bool EditorRenderSystem::m_isWireFrameMode = false;
    bool EditorRenderSystem::m_useGammaCorrection = false;
    bool EditorRenderSystem::m_showShadowMap = false;
    bool EditorRenderSystem::m_highlightSelections = true;

    void EditorRenderSystem::Init()
    {
        // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
        glewExperimental = GL_TRUE;
        // Initialize GLEW to setup the OpenGL Function pointers
        glewInit();
        glCheckError();

        GLFWwindow* window = FishEditorWindow::window();
        // Define the view port dimensions


        ImGui_ImplGlfwGL3_Init(window, false);

        RenderSystem::Init();
        EditorGUI::Init();
    }

    void EditorRenderSystem::Render()
    {
        ImGui_ImplGlfwGL3_NewFrame();

        RenderSystem::Render();

        /************************************************************************/
        /* Grid                                                                 */
        /************************************************************************/
        static std::shared_ptr<SimpleMesh> grid;
        static bool grid_initialized = false;
        if (!grid_initialized)
        {
            constexpr int rows = 10;
            constexpr int vertex_count = (rows*2 + 1) * 2 * 2;
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
            grid = std::make_shared<SimpleMesh>(grid_vertex, vertex_count, GL_LINES);
            grid_initialized = true;
        }
        auto view = Camera::main()->worldToCameraMatrix();
        auto proj = Camera::main()->projectionMatrix();
        auto shader = Shader::builtinShader("SolidColor-Internal");
        shader->Use();
        shader->BindUniformVec4("_Color", Color::gray);
        shader->BindUniformMat4("MATRIX_MVP", proj*view);
        grid->Render();


        /************************************************************************/
        /* Selection                                                            */
        /************************************************************************/
        if (m_highlightSelections)
        {
            auto camera = Camera::main();
            auto view = camera->worldToCameraMatrix();
            auto proj = camera->projectionMatrix();
            auto vp = proj * view;
            glEnable(GL_POLYGON_OFFSET_LINE);
            glPolygonOffset(-1.0, -1.0f);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            auto material = Material::builtinMaterial("SolidColor");
            material->SetVector4("Color", Vector4(0.375f, 0.388f, 0.463f, 1));
            

            std::list<std::shared_ptr<GameObject>> selections;
            auto go = Selection::selectedGameObjectInHierarchy();
            selections.push_back(go);
            while (!selections.empty()) {
                go = selections.front();
                selections.pop_front();
                if (go == nullptr) {
                    continue;
                }
                auto& children = go->transform()->children();
                for (auto& c : children) {
                    selections.push_back(c.lock()->gameObject());
                }
                auto meshFilter = go->GetComponent<MeshFilter>();
                if (meshFilter != nullptr)
                {
                    material->shader()->Use();
                    auto model = go->transform()->localToWorldMatrix() * Matrix4x4::Scale(1.001f, 1.001f, 1.001f);
                    Pipeline::perDrawUniformData.MATRIX_MVP = vp * model;
                    Pipeline::BindPerDrawUniforms();
                    material->Update();
                    material->shader()->CheckStatus();
                    meshFilter->mesh()->Render();
                }
                else
                {
                    auto skinnedMeshRenderer = go->GetComponent<SkinnedMeshRenderer>();
                    if (skinnedMeshRenderer != nullptr)
                    {
                        auto mesh = skinnedMeshRenderer->sharedMesh();
                        auto shader = material->shader();
                        bool useSkinnedVersion = FishEditorWindow::InPlayMode();
                        if (useSkinnedVersion)
                        {
                            shader = material->shader()->m_skinnedShader;
                            shader->Use();
                            shader->BindMatrixArray("BoneTransformations", skinnedMeshRenderer->m_matrixPalette);
                        }
                        else {
                            shader->Use();
                        }
                        auto model = go->transform()->localToWorldMatrix() * Matrix4x4::Scale(1.001f, 1.001f, 1.001f);
                        Pipeline::perDrawUniformData.MATRIX_MVP = vp * model;
                        Pipeline::BindPerDrawUniforms();
                        material->Update(useSkinnedVersion);
                        shader->CheckStatus();
                        mesh->Render();
                    }
                }
            }
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glDisable(GL_POLYGON_OFFSET_LINE);
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
        glClear(GL_DEPTH_BUFFER_BIT);
        auto selectedGO = Selection::selectedGameObjectInHierarchy();
        auto camera_preview = selectedGO == nullptr ? nullptr : selectedGO->GetComponent<Camera>();
        if (camera_preview != nullptr)
        {
            const int w = Screen::width();
            const int h = Screen::height();
            //auto v = camera_preview->viewport();
            const int preview_width  = static_cast<int>(w * 0.25f);
            const int preview_height = static_cast<int>(h * 0.25f);
            constexpr int padding = 20;
            const int preview_pos_x  = w - preview_width - padding;
            const int preview_pos_y  = padding;
            glViewport(preview_pos_x, preview_pos_y, preview_width, preview_height);
            auto main_camera = Camera::main();
            Screen::m_width = static_cast<int>(w*0.25f);
            Screen::m_height = static_cast<int>(h*0.25f);
            Camera::m_mainCamera = camera_preview;
            
            auto proj = camera_preview->projectionMatrix();
            auto view = camera_preview->worldToCameraMatrix();
            Pipeline::perFrameUniformData.MATRIX_P = proj;
            Pipeline::perFrameUniformData.MATRIX_V = view;
            Pipeline::perFrameUniformData.MATRIX_I_V = view.inverse();
            Pipeline::perFrameUniformData.MATRIX_VP = proj * view;
            Pipeline::perFrameUniformData.WorldSpaceCameraPos = camera_preview->transform()->position();
            Pipeline::BindPerFrameUniforms();
            Scene::Render();
            Camera::m_mainCamera = main_camera;
            Screen::m_width = w;
            Screen::m_height = h;
        }
        
        EditorGUI::Update();

        // Swap the screen buffers
        glfwSwapBuffers(FishEditorWindow::window());
    }

    void EditorRenderSystem::Clean()
    {
        RenderSystem::Clean();
        EditorGUI::Clean();
    }

    void EditorRenderSystem::SaveScreenShot(const std::string& path)
    {
        const int w = Screen::width();
        const int h = Screen::height();
        auto pixels = new uint8_t[3 * w *h * 2];
        glPixelStorei(GL_PACK_ALIGNMENT, 1);
        glReadPixels(0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, pixels + w*h * 3);
        // horizontally flip
        auto line_a = pixels;
        auto line_b = pixels + w*h * 3 * 2 - w * 3;
        for (int l = 0; l < h; ++l)
        {
            memcpy(line_a, line_b, w * 3 * sizeof(uint8_t));
            line_a += w * 3;
            line_b -= w * 3;
        }
        stbi_write_png(path.c_str(), w, h, 3, pixels, 0);
        delete[] pixels;
    }

    void EditorRenderSystem::OnWindowSizeChanged(const int width, const int height)
    {
        //glViewport(0, 0, m_width, m_height);
        Camera::OnWindowSizeChanged(width, height);
    }
}
