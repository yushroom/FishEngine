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
#include <RenderTexture.hpp>
#include <Screen.hpp>
#include <Timer.hpp>

#include "FishEditorWindow.hpp"
#include "EditorGUI.hpp"
#include "Selection.hpp"

using namespace FishEngine;

namespace FishEditor
{
    void EditorRenderSystem::Init()
    {
        // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
#if FISHENGINE_PLATFORM_WINDOWS
        glewExperimental = GL_TRUE;
        // Initialize GLEW to setup the OpenGL Function pointers
        auto err = glewInit();
        if (err != GLEW_OK)
        {
            Debug::LogError("%s", glewGetErrorString(err));
        }
#endif
        glCheckError();
        Debug::Log("GLEW initialized.");

        GLFWwindow* window = FishEditorWindow::window();

        ImGui_ImplGlfwGL3_Init(window, false);

        RenderSystem::Init();
        EditorGUI::Init();
    }

    void EditorRenderSystem::Render()
    {
        ImGui_ImplGlfwGL3_NewFrame();
        //Timer timer("EditorRenderSystem::Render");
        EditorGUI::Update();
        // Swap the screen buffers
        glfwSwapBuffers(FishEditorWindow::window());
        //timer.StopAndPrint();
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
        //Camera::OnWindowSizeChanged(width, height);
    }
}
