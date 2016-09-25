#include "EditorRenderSystem.hpp"

//#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

#include "Debug.hpp"
#include "Input.hpp"
#include "EditorGUI.hpp"
#include "Camera.hpp"
#include "Time.hpp"
#include "MeshRenderer.hpp"
#include "Scene.hpp"
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw_gl3.h>
#include "FishEditorWindow.hpp"
#include <RenderSystem.hpp>
#include <Light.hpp>
#include <Mesh.hpp>
#include <MeshFilter.hpp>
#include "Selection.hpp"

using namespace FishEngine;

NAMESPACE_FISHEDITOR_BEGIN

int EditorRenderSystem::m_width = 800;
int EditorRenderSystem::m_height = 600;

bool EditorRenderSystem::m_isWireFrameMode = false;
bool EditorRenderSystem::m_useGammaCorrection = true;
bool EditorRenderSystem::m_showShadowMap = true;
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
    glfwGetFramebufferSize(window, &m_width, &m_height);
    glViewport(0, 0, m_width, m_height);
    RenderSystem::m_width = m_width;
    RenderSystem::m_height = m_height;

    ImGui_ImplGlfwGL3_Init(window, false);
    
    Shader::Init();
    Material::Init();
    Mesh::Init();
    EditorGUI::Init();
    Scene::Init();
//#ifdef GLM_FORCE_LEFT_HANDED
    glFrontFace(GL_CW);
//#endif
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
}

void EditorRenderSystem::Render()
{
    ImGui_ImplGlfwGL3_NewFrame();

    // Shadow
    
    auto lights = Light::lights();
    for (auto& l : lights) {
        Scene::RenderShadow(l);
    }

    
    // Render
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    auto v = Scene::mainCamera()->viewport();
    glViewport(GLint(v.x*m_width), GLint(v.y*m_height), GLsizei(v.z*m_width), GLsizei(v.w*m_height));

    // Selection

    auto camera = Scene::mainCamera();
    auto view = camera->worldToCameraMatrix();
    auto proj = camera->projectionMatrix();
    auto vp = proj * view;
    glEnable(GL_POLYGON_OFFSET_LINE);
    glPolygonOffset(-1.0, -1.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    ShaderUniforms uniforms;
    auto material = Material::builtinMaterial("SolidColor");
    material->SetVector4("Color", Vector4(0, 1, 0, 1));
    material->shader()->Use();
    
    if (m_highlightSelections) {
        std::list<std::shared_ptr<GameObject>> selections;
        auto go = Selection::activeGameObject();
        selections.push_back(go);
        while (!selections.empty()) {
            go = selections.front();
            selections.pop_back();
            if (go == nullptr) {
                continue;
            }
            auto& children = go->transform()->children();
            for (auto& c : children) {
                selections.push_back(c.lock()->gameObject());
            }
            auto meshFilter = go->GetComponent<MeshFilter>();
            if (meshFilter != nullptr) {
                auto model = go->transform()->localToWorldMatrix() * Matrix4x4::Scale(1.001f, 1.001f, 1.001f);
                uniforms.mat4s["MATRIX_MVP"] = vp * model;
                
                material->shader()->BindUniforms(uniforms);
                material->Update();
                material->shader()->CheckStatus();
                meshFilter->mesh()->Render();
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

    //Scene::Render();
    std::vector<std::shared_ptr<GameObject>> transparentQueue;
    for (auto& go : Scene::m_gameObjects) {
        if (!go->activeInHierarchy()) continue;
        auto renderer = go->GetComponent<MeshRenderer>();
        if (renderer == nullptr) {
            continue;
        }
        bool isTransparent = renderer->material()->shader()->IsTransparent();
        if (isTransparent) {
            transparentQueue.push_back(go);
            continue;
        }
        renderer->Render();
    }

    for (auto& go : transparentQueue) {
        auto renderer = go->GetComponent<MeshRenderer>();
        renderer->Render();
    }

    transparentQueue.clear();

    if (m_isWireFrameMode)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    if (m_showShadowMap) {
        auto& l = Light::lights().front();
        std::map<std::string, Texture::PTexture> textures;
        textures["screenTexture"] = l->m_shadowMap;
        glViewport(200, 200, 400, 400);
        auto m = Material::builtinMaterial("ScreenTexture");
        m->shader()->Use();
        m->shader()->BindTextures(textures);
        Mesh::builtinMesh("quad")->Render();
    }
    EditorGUI::Update();

    // Swap the screen buffers
    glfwSwapBuffers(FishEditorWindow::window());
}

void EditorRenderSystem::Clean()
{
    EditorGUI::Clean();
}

void EditorRenderSystem::SaveScreenShot(const std::string& path)
{
    auto pixels = new uint8_t[3 * m_width*m_height * 2];
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadPixels(0, 0, m_width, m_height, GL_RGB, GL_UNSIGNED_BYTE, pixels + m_width*m_height * 3);
    // horizontally flip
    auto line_a = pixels;
    auto line_b = pixels + m_width*m_height * 3 * 2 - m_width * 3;
    for (int l = 0; l < m_height; ++l)
    {
        memcpy(line_a, line_b, m_width * 3 * sizeof(uint8_t));
        line_a += m_width * 3;
        line_b -= m_width * 3;
    }
    stbi_write_png(path.c_str(), m_width, m_height, 3, pixels, 0);
    delete[] pixels;
}

void EditorRenderSystem::OnWindowSizeChanged(const int width, const int height) {
    //glfwGetFramebufferSize(window, &m_width, &m_height);
    m_width = width;
    m_height = height;
    glViewport(0, 0, m_width, m_height);
    Scene::mainCamera()->setAspect(float(m_width)/m_height);
}



NAMESPACE_FISHEDITOR_END
