#include <GLEnvironment.hpp>
#include <Shader.hpp>
#include <Debug.hpp>
#include <string>

using namespace FishEngine;
using namespace std;

int main(int argc, char* argv[])
{
    Debug::Init();
    Debug::setColorMode(false);
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
    if (argc != 3)
        return 1;
    
    GLenum shaderType = GL_VERTEX_SHADER;
    std::string path = argv[2];;
    const int type = std::stoi(std::string(argv[1]));
    
    if (type == 3)
    {
        try
        {
            Shader::LoadShaderCombined(path);
        }
        catch(exception)
        {
            return 1;
        }
        Debug::Log("OK");
        return 0;
    }
    
    switch (type)
    {
    case 0: shaderType = GL_VERTEX_SHADER; break;
    case 1: shaderType = GL_FRAGMENT_SHADER; break;
    case 2: shaderType = GL_GEOMETRY_SHADER; break;
    default: Debug::LogError("Unknown shader type %d", type);
    }
    
    //Debug::Log(path.c_str());

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
