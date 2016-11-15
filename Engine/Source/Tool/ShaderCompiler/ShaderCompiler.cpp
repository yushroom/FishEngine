#include <GLEnvironment.hpp>
#include <Shader.hpp>
#include <Debug.hpp>
#include <string>
#include <glfw/glfw3.h>

using namespace FishEngine;
using namespace std;

int main(int argc, char* argv[])
{
    //Debug::Init();
    Debug::setColorMode(false);
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

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    glewInit();
    glCheckError();

    //assert(argc == 3);
    if (argc != 2)
        return 1;
    
    GLenum shaderType = GL_VERTEX_SHADER;
    std::string path = argv[1];
    auto ext = FishEngine::getExtensionWithoutDot(path);

    if (ext == "surf")
    {
        try
        {
            Shader::CreateFromFile(path);
        }
        catch (exception)
        {
            return 1;
        }
        Debug::Log("OK");
        return 0;
    }
    else if (ext == "vsfs")
    {
        try
        {
            Shader::CreateFromFile(path);
        }
        catch (exception)
        {
            return 1;
        }
        Debug::Log("OK");
        return 0;
    }
    else if (ext == "vert")
    {
        shaderType = GL_VERTEX_SHADER;
    }
    else if (ext == "frag")
    {
        shaderType = GL_FRAGMENT_SHADER;
    }
    else if (ext == "geom")
    {
        shaderType = GL_GEOMETRY_SHADER;
    }
    else
    {
        Debug::LogError("Unknown shader type %s", ext.c_str());
        return 1;
    }

    try
    {
        Shader::LoadShader(shaderType, path);
    }
    catch(exception)
    {
        return 1;
    }
    Debug::Log("OK");

    return 0;
}
