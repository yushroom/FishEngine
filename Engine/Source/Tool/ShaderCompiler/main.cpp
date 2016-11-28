#include <Shader.hpp>
#include <GLEnvironment.hpp>
#include <glfw/glfw3.h>
#include <Debug.hpp>

using namespace FishEngine;

int main(int argc, char* argv[])
{
    //Debug::Init();
    Debug::setColorMode(false);
    Resources::Init();
    Debug::Log("Compiling...");
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    //glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    constexpr int WIDTH = 1;
    constexpr int HEIGHT = 1;

    // Create a GLFWwindow object that we can use for GLFW's functions
    auto window = glfwCreateWindow(WIDTH, HEIGHT, "FishEngine", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glCheckError();

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    glewInit();

    std::string path = R"(D:\program\FishEngine\Engine\Shaders\SkyBox-Cubed.shader)";
    if (argc == 2)
    {
        path = argv[1];
    }
    else
    {
        Debug::LogError("invalid command.");
#ifndef _DEBUG
        return 1;
#endif
    }

    auto shader = Shader::CreateFromFile(path);
    if (shader == nullptr)
        return 1;

    Debug::Log("OK");
    return 0;
}