#include <GLEnvironment.hpp>
#include <glfw/glfw3.h>
#include <Debug.hpp>
#include <Resources.hpp>
#include <Shader.hpp>
#include <ModelImporter.hpp>
#include <Graphics.hpp>

using namespace FishEngine;

constexpr int WIDTH = 640;
constexpr int HEIGHT = 480;

GLuint tex = 0;
GLuint fbo = 0;
MeshPtr quad = nullptr;
ShaderPtr shader = nullptr;

void Init()
{
    glCheckError();
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D_ARRAY, tex);
    //glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA, WIDTH, HEIGHT, 4, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, WIDTH, HEIGHT, 4);
    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
    
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, tex, 0);
    glCheckError();
    static const GLenum draw_buffers[] = {
        GL_COLOR_ATTACHMENT0
    };
    glDrawBuffers(1, draw_buffers);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glCheckError();
    quad = Model::builtinMesh(PrimitiveType::Quad);
    
    glCheckError();
    shader = Shader::CreateFromFile("/Users/yushroom/program/graphics/FishEngine/Engine/Source/Test/TestLayerRendering.shader");
    
    glCheckError();
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
    
    Debug::setColorMode(false);
    Resources::Init();
    Shader::Init();
    Material::Init();
    Model::Init();
    Init();
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClearColor(0.1, 0.2, 0.3, 1);
        
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDisable(GL_CULL_FACE);
        glDisable(GL_DEPTH_TEST);
        glDepthFunc(GL_ALWAYS);
        shader->Use();
        quad->Render();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        
        glCheckError();
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glDepthFunc(GL_ALWAYS);
        auto display_csm_mtl = Material::builtinMaterial("DisplayCSM");
        auto s = display_csm_mtl->shader();
        s->Use();
        ShaderUniforms uniforms;
        constexpr float size = 0.25f;
        auto quad = Model::builtinMesh(PrimitiveType::Quad);
        for (int i = 0; i < 4; ++i)
        {
//            display_csm_mtl->SetFloat("Section", float(i));
//            Vector4 v(i*size*2-1, -1, size, size);
//            display_csm_mtl->SetVector4("DrawRectParameters", v);
//            display_csm_mtl->setMainTexture();
//            Graphics::DrawMesh(quad, display_csm_mtl);
            uniforms.floats["Section"] = float(i);
            Vector4 v(i*size*2-1, -1, size, size);
            uniforms.vec4s["DrawRectParameters"] = v;
            glCheckError();
            s->BindUniforms(uniforms);
            glCheckError();
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D_ARRAY, tex);
            glCheckError();
            
            quad->Render();
        }
        glDepthFunc(GL_LESS);
        
        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        
        /* Poll for and process events */
        glfwPollEvents();
    }
    
    glfwTerminate();
    return 0;
}
