#include "Gizmos.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "Mesh.hpp"
#include "Pipeline.hpp"
#include "Texture.hpp"
#include "ModelImporter.hpp"

using namespace FishEngine;


PShader     Gizmos::s_solidColorShader;
static PShader     alphaShader;

Color       Gizmos::s_color         = Color::green;
PSimpleMesh Gizmos::s_circleMesh    = nullptr;
PSimpleMesh Gizmos::s_boxMesh       = nullptr;
PSimpleMesh Gizmos::s_light         = nullptr;


static PTexture cameraGizmoTexture;
static PTexture lightGizmoTexture;

//std::shared_ptr<SimpleMesh> Gizmos::s_wiredSphereMesh = nullptr;

constexpr int circleVertexCount = 64;

void Gizmos::Init()
{
#if FISHENGINE_PLATFORM_WINDOWS
    const std::string root_dir = "../../assets/";
#else
    const std::string root_dir = "/Users/yushroom/program/graphics/FishEngine/assets/";
#endif
    s_solidColorShader = Shader::CreateFromFile(root_dir + "shaders/Editor/SolidColor.vsfs");
    alphaShader = Shader::CreateFromFile(root_dir + "shaders/Editor/Alpha.vsfs");
    
    cameraGizmoTexture = Texture::CreateFromFile(root_dir + "textures/Gizmos/camera.png");
    lightGizmoTexture  = Texture::CreateFromFile(root_dir + "textures/Gizmos/light.png");

    // circle
    constexpr float radStep = 2.0f * Mathf::PI / circleVertexCount;
    float circle_vertex[circleVertexCount*3];
    for (int i = 0; i < circleVertexCount; ++i) {
        const int j = i * 3;
        const float rad = radStep * i - Mathf::PI/2;
        circle_vertex[j] = std::cosf(rad);
        circle_vertex[j+1] = 0.f;
        circle_vertex[j+2] = std::sinf(rad);
    }
    s_circleMesh = std::make_shared<SimpleMesh>(circle_vertex, circleVertexCount, GL_LINE_LOOP);
    
    float box_vertex[] = {
         0.5f,  0.5f,  0.5f,  0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,  0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f,
        
        -0.5f,  0.5f, -0.5f, -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f, -0.5f, -0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,  0.5f,  0.5f, -0.5f,
        
        -0.5f, -0.5f,  0.5f, -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f, -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f,
        
         0.5f, -0.5f, -0.5f,  0.5f, -0.5f,  0.5f,
         0.5f, -0.5f, -0.5f,  0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f,
    };
    s_boxMesh = std::make_shared<SimpleMesh>(box_vertex, 24, GL_LINES);

    float lightGizmoVertex[circleVertexCount * 3 + 8 * 2 * 3];
    memcpy(lightGizmoVertex, circle_vertex, circleVertexCount * 3 * sizeof(float));
    constexpr int   numLines    = 8;
    constexpr float step        = Mathf::PI * 2.0f / numLines;
    for (int i = 0; i < numLines; ++i)
    {
        int j = circleVertexCount * 3 + i * 2 * 3;
        const float rad = step * i;
        float s = std::sinf(rad);
        float c = std::cosf(rad);
        lightGizmoVertex[j]     = c;
        lightGizmoVertex[j + 1] = 0;
        lightGizmoVertex[j + 2] = s;
        lightGizmoVertex[j + 3] = c;
        lightGizmoVertex[j + 4] = 4;
        lightGizmoVertex[j + 5] = s;
    }
    s_light = std::make_shared<SimpleMesh>(lightGizmoVertex, circleVertexCount + numLines * 2, GL_LINES);
    
    // wired sphere
    //constexpr float radStep = 2.0f * Mathf::PI / circleVertexCount;
//    float wiredSphereVertex[circleVertexCount*3*3];
//    for (int i = 0; i < circleVertexCount; ++i) {
//        const int j1 = i * 3;
//        const int j2 = j1 + circleVertexCount*3;
//        const int j3 = j2 + circleVertexCount*3;
//        const float rad = radStep * i;
//        wiredSphereVertex[j3+1] = wiredSphereVertex[j2] =  wiredSphereVertex[j1] = std::cosf(rad);
//        wiredSphereVertex[j3] = wiredSphereVertex[j2+2] = wiredSphereVertex[j1+1] = 0.f;
//        wiredSphereVertex[j3+2] = wiredSphereVertex[j2+1] = wiredSphereVertex[j1+2] = std::sinf(rad);
//    }
//    
//    s_wiredSphereMesh = std::make_shared<SimpleMesh>(wiredSphereVertex, circleVertexCount*3, GL_LINE_STRIP);
}

void Gizmos::DrawLine(const Vector3& from, const Vector3& to)
{
    static DynamicMesh lineMesh;
    float vertices[6];
    vertices[0] = from.x;
    vertices[1] = from.y;
    vertices[2] = from.z;
    vertices[3] = to.x;
    vertices[4] = to.y;
    vertices[5] = to.z;
    //glLineWidth(50);  // TODO, no effect
    auto view = Camera::main()->worldToCameraMatrix();
    auto proj = Camera::main()->projectionMatrix();
    s_solidColorShader->Use();
    s_solidColorShader->BindUniformMat4("MATRIX_MVP", proj*view);
    s_solidColorShader->BindUniformVec4("_Color", s_color);
    lineMesh.Render(vertices, 2, GL_LINES);
}

void Gizmos::
DrawIcon(
    Vector3               center,
    const std::string&    name,
    bool                  allowScaling)
{
    static PModel quad = Model::builtinModel(PrimitiveType::Quad);
    alphaShader->Use();
    auto cameraPos = Camera::main()->transform()->position();
    auto view = Camera::main()->worldToCameraMatrix();
    auto proj = Camera::main()->projectionMatrix();
    Matrix4x4 m;
    //if (allowScaling)
    //{
        m.SetTRS(center, view.ToRotation().inverse(), Vector3::one);
    //}
    //else
    //{
    //    //Vector3 dir = Vector3::Normalize(center - cameraPos);
    //    m.SetTRS(cameraPos + dir*20, view.ToRotation().inverse(), Vector3::one);
    //}

    alphaShader->BindUniformMat4("MATRIX_MVP", proj*view*m);
    std::map<std::string, PTexture> textures;
    if (name == "Camera")
        textures["_MainTex"] = cameraGizmoTexture;
    else
        textures["_MainTex"] = lightGizmoTexture;
    alphaShader->BindTextures(textures);
    alphaShader->PreRender();
    quad->mainMesh()->Render();
    alphaShader->PostRender();
}

void Gizmos::DrawWireSphere(const Vector3& center,
                            float radius,
                            const Matrix4x4& modelMatrix)
{
    Matrix4x4 m;
    auto view = Camera::main()->worldToCameraMatrix();
    auto proj = Camera::main()->projectionMatrix();
    s_solidColorShader->Use();
    s_solidColorShader->BindUniformVec4("_Color", s_color);
    
    float euler_angles[] = {
        0, 0, 90,
        0, 0, 0,
        90, 0, 0,
    };
    
    for (int i = 0; i < 3; ++i) {
        float* e = euler_angles + i*3;
        m.SetTRS(center, Quaternion::Euler(Vector3(e)), Vector3::one * radius);
        s_solidColorShader->BindUniformMat4("MATRIX_MVP", proj*view*m);
        s_circleMesh->Render();
    }
}

void Gizmos::
DrawHalfWireSphere(const Vector3& center,
                   const float radius,
                   const Matrix4x4& modelMatrix)
{
    const auto& shader = Shader::builtinShader("SolidColor");
    shader->Use();
    Matrix4x4 m;
    //m.SetTRS(center, Quaternion::identity, Vector3::one * radius);
    ShaderUniforms uniforms;
    auto v = Camera::main()->worldToCameraMatrix();
    auto p = Camera::main()->projectionMatrix();
    uniforms.vec4s["Color"] = s_color;
    shader->BindUniforms(uniforms);
    
    float euler_angles[] = {
        0, 0, 90,
        0, 0, 0,
        0, 90, 90,
    };
    
    for (int i = 0; i < 3; ++i) {
        float* e = euler_angles + i*3;
        m.SetTRS(center, Quaternion::Euler(Vector3(e)), Vector3::one * radius);
        Pipeline::perDrawUniformData.MATRIX_MVP = p * v * m * modelMatrix;
        Pipeline::BindPerDrawUniforms();
        glBindVertexArray(s_circleMesh->m_VAO);
        int count = s_circleMesh->m_positionBuffer.size()/3;
        if (i == 1) {
            glDrawArrays(GL_LINE_LOOP, 0, count);
        } else {
            count = count/2+1;
            glDrawArrays(GL_LINE_STRIP, 0, count);
        }
        
        glBindVertexArray(0);
    }

}

void Gizmos::
DrawWireCube(const Vector3& center,
             const Vector3& size,
             const Matrix4x4& modelMatrix)
{
    const auto& shader = Shader::builtinShader("SolidColor");
    shader->Use();
    Matrix4x4 m;
    m.SetTRS(center, Quaternion::identity, size);
    ShaderUniforms uniforms;
    auto v = Camera::main()->worldToCameraMatrix();
    auto p = Camera::main()->projectionMatrix();
    uniforms.vec4s["Color"] = s_color;
    //uniforms.mat4s["MATRIX_MVP"] = p * v * modelMatrix * m;
    Pipeline::perDrawUniformData.MATRIX_MVP = p * v * modelMatrix * m;
    Pipeline::BindPerDrawUniforms();
    shader->BindUniforms(uniforms);
    s_boxMesh->Render();
}

void Gizmos::
DrawWireCapsule(const Vector3& center,
                const float radius,
                const float height,
                const Matrix4x4& modelMatrix)
{
    Vector3 c1 = center+Vector3(0, height*0.5f-radius, 0);
    Vector3 c2 = center-Vector3(0, height*0.5f-radius, 0);
//    glBindVertexArray(s_circleMesh->m_VAO);
//    glDrawArrays(GL_LINE, 0, s_circleMesh->m_positionBuffer.size()/3/2+1);
//    glBindVertexArray(0);
    DrawHalfWireSphere(c1, radius);
    DrawHalfWireSphere(c2, radius, Matrix4x4::Scale(-1, 1, 1));
    
    Vector3 offset(radius, 0, 0);
    DrawLine(c1+offset, c2+offset);
    offset.Set(-radius, 0, 0);
    DrawLine(c1+offset, c2+offset);
    offset.Set(0, 0, radius);
    DrawLine(c1+offset, c2+offset);
    offset.Set(0, 0, -radius);
    DrawLine(c1+offset, c2+offset);
}

void FishEngine::Gizmos::
DrawCircle(
    const Vector3&  center,
    const float     radius,
    const Vector3&  direction)
{
    const auto& shader = Shader::builtinShader("SolidColor");
    shader->Use();
    ShaderUniforms uniforms;
    auto v = Camera::main()->worldToCameraMatrix();
    auto p = Camera::main()->projectionMatrix();
    uniforms.vec4s["Color"] = s_color;
    shader->BindUniforms(uniforms);
    Matrix4x4 m;
    m.SetTRS(center, Quaternion::FromToRotation(Vector3::up, direction), Vector3(radius, radius, radius));
    Pipeline::perDrawUniformData.MATRIX_MVP = p * v * m;
    Pipeline::BindPerDrawUniforms();
    s_circleMesh->Render();
}

void FishEngine::Gizmos::DrawLight(const Vector3& center, const Vector3& direction)
{
    const auto& shader = Shader::builtinShader("SolidColor");
    shader->Use();
    ShaderUniforms uniforms;
    auto v = Camera::main()->worldToCameraMatrix();
    auto p = Camera::main()->projectionMatrix();
    float dist = Vector3::Distance(center, Camera::main()->transform()->position());
    uniforms.vec4s["Color"] = s_color;
    shader->BindUniforms(uniforms);
    Matrix4x4 m;
    m.SetTRS(center, Quaternion::FromToRotation(Vector3::up, direction), Vector3::one * 5.f / dist);
    Pipeline::perDrawUniformData.MATRIX_MVP = p * v * m;
    Pipeline::BindPerDrawUniforms();
    s_light->Render();
}

void FishEngine::Gizmos::DrawFrustum(
    const Matrix4x4&    localToWorld,
    const float         zNear,
    const float         zFar,
    const float         fovy,
    const float         aspect)
{
    static DynamicMesh frustumMesh;
    constexpr int numLines = 4 * 3;
    float vertices[numLines*2*3];
    float tan2 = Mathf::Tan(fovy * 0.5f);
    float h1 = tan2 * zNear;
    float w1 = h1 * aspect;
    float h2 = tan2 * zFar;
    float w2 = h2 * aspect;

    Vector3 v[8];
    v[0].Set(-w1,  h1, zNear);
    v[1].Set( w1,  h1, zNear);
    v[2].Set( w1, -h1, zNear);
    v[3].Set(-w1, -h1, zNear);
    v[4].Set(-w2,  h2, zFar);
    v[5].Set( w2,  h2, zFar);
    v[6].Set( w2, -h2, zFar);
    v[7].Set(-w2, -h2, zFar);
    for (int i = 0; i < 8; ++i)
    {
        v[i] = localToWorld.MultiplyPoint(v[i]);
    }

    const int indices[numLines * 2] = {
        0, 1, 1, 2, 2, 3, 3, 0,
        4, 5, 5, 6, 6, 7, 7, 4,
        0, 4, 1, 5, 2, 6, 3, 7,
    };

    float *p = vertices;
    for (int i = 0; i < numLines * 2; ++i)
    {
        auto v3 = v[indices[i]];
        *p = v3.x; p++;
        *p = v3.y; p++;
        *p = v3.z; p++;
    }

    auto view = Camera::main()->worldToCameraMatrix();
    auto proj = Camera::main()->projectionMatrix();
    s_solidColorShader->Use();
    s_solidColorShader->BindUniformMat4("MATRIX_MVP", proj*view);
    s_solidColorShader->BindUniformVec4("_Color", s_color);
    frustumMesh.Render(vertices, numLines * 2, GL_LINES);
}
