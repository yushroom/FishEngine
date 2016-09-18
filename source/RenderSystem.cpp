#include "RenderSystem.hpp"

#define STB_IMAGE_WRITE_IMPLEMENTATION
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
#include "GameLoop.hpp"

NAMESPACE_FISHENGINE_BEGIN

int RenderSystem::m_width = 800;
int RenderSystem::m_height = 600;

bool RenderSystem::m_isWireFrameMode = false;
bool RenderSystem::m_useGammaCorrection = true;


void RenderSystem::Init()
{
    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    glewInit();
    glCheckError();

    GLFWwindow* window = GameLoop::window();
    // Define the view port dimensions
    glfwGetFramebufferSize(window, &m_width, &m_height);
    glViewport(0, 0, m_width, m_height);

    ImGui_ImplGlfwGL3_Init(window, false);
    
    Shader::Init();
    Material::Init();
    EditorGUI::Init();
    Scene::Init();
}

void RenderSystem::Render()
{
    ImGui_ImplGlfwGL3_NewFrame();
    
    // Render
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    
    auto v = Scene::mainCamera()->m_viewport;
    glViewport(v.x*m_width, v.y*m_height, v.z*m_width, v.z*m_height);
        
//#ifdef GLM_FORCE_LEFT_HANDED
//    glFrontFace(GL_CW);
//#endif

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    
    if (m_isWireFrameMode)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    if (m_useGammaCorrection)
        glEnable(GL_FRAMEBUFFER_SRGB);
    else
        glDisable(GL_FRAMEBUFFER_SRGB);

    Scene::Render();
    
    if (m_isWireFrameMode)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    EditorGUI::Update();

    // Swap the screen buffers
    glfwSwapBuffers(GameLoop::window());
}

void RenderSystem::Clean()
{
    EditorGUI::Clean();
}

void RenderSystem::SaveScreenShot(const std::string& path)
{
    auto pixels = new uint8_t[3*m_width*m_height*2];
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadPixels(0, 0, m_width, m_height, GL_RGB, GL_UNSIGNED_BYTE, pixels+ m_width*m_height * 3);
    // horizontally flip
    auto line_a = pixels;
    auto line_b = pixels+m_width*m_height*3*2-m_width*3;
    for (int l = 0; l < m_height; ++l) 
    {
        memcpy(line_a, line_b, m_width*3*sizeof(uint8_t));
        line_a += m_width * 3;
        line_b -= m_width * 3;
    }
    stbi_write_png(path.c_str(), m_width, m_height, 3, pixels, 0);
    delete[] pixels;
}


NAMESPACE_FISHENGINE_END
