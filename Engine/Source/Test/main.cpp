#include <GLEnvironment.hpp>
#include <glfw/glfw3.h>
#include <Debug.hpp>
#include <Resources.hpp>
#include <Shader.hpp>
#include <ModelImporter.hpp>
#include <Graphics.hpp>
#include <Camera.hpp>
#include <CameraController.hpp>
#include <Pipeline.hpp>
#include <RenderSystem.hpp>
#include <Light.hpp>
#include <RenderTarget.hpp>
//#include <Input.hpp>

using namespace FishEngine;
using namespace std;

constexpr int WIDTH = 640;
constexpr int HEIGHT = 480;

//class GameApp
//{
//public:
//    virtual void Init() = 0;
//    virtual void Update() = 0;
//    virtual void Render() = 0;
//};


ColorBufferPtr color_buffer;
DepthBufferPtr depth_buffer;
RenderTargetPtr render_target;
RenderTargetPtr render_target2;
RenderTargetPtr render_target3;
MaterialPtr selection_outline;
MeshPtr cone = nullptr;
ShaderPtr shader = nullptr;

void Init()
{
    depth_buffer = DepthBuffer::Create(WIDTH, HEIGHT);
    color_buffer = ColorBuffer::Create(WIDTH, HEIGHT);
    render_target = make_shared<RenderTarget>();
    render_target->SetDepthBufferOnly(depth_buffer);

    render_target2 = make_shared<RenderTarget>();
    render_target2->SetColorBufferOnly(color_buffer);

    cone = Model::builtinMesh(PrimitiveType::Cone);
    auto camera = Camera::Create();
    auto camera_go = Scene::CreateGameObject("Main Camera");
    camera_go->AddComponent(camera);
    camera_go->AddComponent<CameraController>();
    camera_go->transform()->setLocalPosition(0, 0, 5);
    camera_go->transform()->setLocalPosition(0, 1, -10);
    camera_go->transform()->LookAt(0, 0, 0);
    camera_go->setTag("MainCamera");
    //camera_go->AddComponent<TakeScreenShot>();

    auto light_go = Scene::CreateGameObject("Directional Light");
    light_go->transform()->setPosition(0, 3, 0);
    light_go->transform()->setLocalEulerAngles(50, -30, 0);
    light_go->AddComponent(Light::Create());

    auto shader = Shader::CreateFromFile(Resources::shaderRootDirectory() / "SelectionOutline.shader");
    selection_outline = Material::CreateMaterial();
    selection_outline->SetShader(shader);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);

    //GLint range[2];
    //glGetIntegerv(GL_LINE_WIDTH_RANGE, range);
    //Debug::LogError("%d %d", range[0], range[1]);
}

void Render()
{
    Pipeline::BindCamera(Camera::main());
    Pipeline::BindLight(Light::lights().front());
    glClearColor(0.1, 0.2, 0.3, 1);
    glClearStencil(0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    auto mtl = Material::defaultMaterial();
    Graphics::DrawMesh(cone, Matrix4x4::identity, mtl);

    Pipeline::PushRenderTarget(render_target);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //glStencilFunc(GL_ALWAYS, 1, -1);
    //glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    Graphics::DrawMesh(cone, Matrix4x4::identity, mtl);
    glCheckError();
    Pipeline::PopRenderTarget();

#if 0
    glBindFramebuffer(GL_READ_FRAMEBUFFER, render_target->GetGLNativeFBO());
    glBlitFramebuffer(0, 0, WIDTH, HEIGHT, 0, 0, WIDTH, HEIGHT, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT, GL_NEAREST);
    //glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

    //glClear(GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    auto mtl2 = Material::builtinMaterial("SolidColor");
    mtl2->SetVector4("Color", Vector4(1, 0, 1, 1));
    glStencilFunc(GL_NOTEQUAL, 1, -1);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    Graphics::DrawMesh(cone, Matrix4x4::Scale(1.2f), mtl2);
#endif

    //Pipeline::PushRenderTarget(render_target2);
    //glClear(GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    auto quad = Model::builtinMesh(PrimitiveType::Quad);
    //auto mtl2 = Material::builtinMaterial("ScreenTexture");
    selection_outline->setMainTexture(depth_buffer);
    Graphics::DrawMesh(quad, selection_outline);
    //Pipeline::PopRenderTarget();
}

int main()
{
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    auto window = glfwCreateWindow(WIDTH, HEIGHT, "FishEngine", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glCheckError();
    
#if FISHENGINE_PLATFORM_WINDOWS
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    auto err = glewInit();
    if (err != GLEW_OK)
    {
        Debug::LogError("%s", glewGetErrorString(err));
    }
#endif

    Debug::Init();
    Debug::setColorMode(true);
    Resources::Init();
    //Input::Init();
    RenderSystem::Init();
    //Shader::Init();
    //Material::Init();
    //Model::Init();
    Init();
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        Render();
        
        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        
        /* Poll for and process events */
        glfwPollEvents();
    }
    
    glfwTerminate();
    return 0;
}
