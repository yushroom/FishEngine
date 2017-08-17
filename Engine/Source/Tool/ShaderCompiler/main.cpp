#include <FishEngine/Shader.hpp>
#include <FishEngine/ShaderCompiler.hpp>
#include <FishEngine/GLEnvironment.hpp>
#include <FishEngine/Debug.hpp>
#include <glfw/glfw3.h>

using namespace FishEngine;

int main(int argc, char* argv[])
{
	//Debug::Init();
	//Debug::setColorMode(false);
	LogInfo("Compiling...");
	
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
	
#if FISHENGINE_PLATFORM_WINDOWS
	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	glewInit();
	std::string path = R"(D:\program\github\FishEngine\Example\UnityChan-crs\Assets\UnityChanStage\Effects\Shaders\Light Beam.shader)";
	ShaderCompiler::setShaderIncludeDir(R"(D:\program\github\FishEngine\Engine\Shaders\include)");
#else
	std::string path = "/Users/yushroom/program/FishEngine/Example/UnityChan-crs/Assets/UnityChan/CandyRockStar/Shader/Unitychan_chara_fuku_ds.shader";
	ShaderCompiler::setShaderIncludeDir("/Users/yushroom/program/FishEngine/Engine/Shaders/include");
#endif

	
	if (argc == 2)
	{
		path = argv[1];
	}
//#ifdef _DEBUG
//    else
//    {
//        Debug::LogError("invalid command.");
//        return 1;
//    }
//#endif
	
	auto shader = Shader::CreateFromFile(path);
	if (shader == nullptr)
		return 1;

	shader->Use();
	if (shader->IsValid())
	{
		LogInfo("OK");
		return 0;
	}
	else
	{
		return 1;
	}
}
