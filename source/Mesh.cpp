#include "Mesh.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Shader.hpp"
#include "Debug.hpp"
#include "Common.hpp"

using namespace std;

NAMESPACE_FISHENGINE_BEGIN

//std::map<std::string, Mesh::PMesh> Mesh::m_meshes;

Mesh::Mesh(const int n_vertex, const int n_face, float* positions, uint32_t* indices)
    :   m_positionBuffer(positions, positions+n_vertex*3),
        m_indexBuffer(indices, indices+n_face*3)
{
    GenerateBuffer(VertexUsagePosition);
    BindBuffer(VertexUsagePosition);
}

Mesh::Mesh(std::vector<float> position_buffer, std::vector<uint32_t> index_buffer)
: m_positionBuffer(position_buffer), m_indexBuffer(index_buffer)
{
    GenerateBuffer(VertexUsagePosition);
    BindBuffer(VertexUsagePosition);
}

Mesh::Mesh(const int n_vertex, const int n_face, float* positions, float* normals, uint32_t* indices)
    :   m_positionBuffer(positions, positions+n_vertex*3),
        m_normalBuffer(normals, normals+n_vertex*3),
        m_indexBuffer(indices, indices+n_face*3)
{
    GenerateBuffer(VertexUsagePN);
    BindBuffer(VertexUsagePN);
}

Mesh::Mesh(Mesh&& m) 
{
    m_positionBuffer = std::move(m.m_positionBuffer);
    m_normalBuffer = std::move(m.m_normalBuffer);
    m_uvBuffer = std::move(m.m_uvBuffer);
    m_tangentBuffer = std::move(m.m_tangentBuffer);
    m_indexBuffer = std::move(m.m_indexBuffer);
    m_VAO = m.m_VAO;
    m_indexVBO = m.m_indexVBO;
    m_positionVBO = m.m_positionVBO;
    m_normalVBO = m.m_normalVBO;
    m_uvVBO = m.m_uvVBO;
    m_tangentVBO = m.m_tangentVBO;
    m.m_VAO = 0;
    m.m_indexVBO = 0;
    m.m_positionVBO = 0;
    m.m_normalVBO = 0;
    m.m_uvVBO = 0;
    m.m_tangentVBO = 0;
}


//Mesh::Mesh(const std::string& objModelPath, int vertexUsage)
//{
//    FromObjFile(objModelPath, vertexUsage);
//}

Mesh::~Mesh() {
    // Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_positionVBO);
    glDeleteBuffers(1, &m_normalVBO);
    glDeleteBuffers(1, &m_tangentVBO);
    glDeleteBuffers(1, &m_indexVBO);
}


void Mesh::Render() {
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, (GLsizei)m_indexBuffer.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

//void Model::renderPatch(const Shader &shader) {
//    glBindVertexArray(m_VAO);
//    glDrawElements(GL_PATCHES, (GLsizei)m_indexBuffer.size(), GL_UNSIGNED_INT, 0);
//    glBindVertexArray(0);
//}

void Mesh::GenerateBuffer(int vertexUsage) {
    // VAO
    glGenVertexArrays(1, &m_VAO);

    // index vbo
    glGenBuffers(1, &m_indexVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexVBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer.size() * 4, m_indexBuffer.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &m_positionVBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_positionVBO);
    glBufferData(GL_ARRAY_BUFFER, m_positionBuffer.size() * 4, m_positionBuffer.data(), GL_STATIC_DRAW);

    if (vertexUsage & VertexUsageNormal) {
        glGenBuffers(1, &m_normalVBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_normalVBO);
        glBufferData(GL_ARRAY_BUFFER, m_normalBuffer.size() * 4, m_normalBuffer.data(), GL_STATIC_DRAW);
    }

    if (vertexUsage & VertexUsageUV) {
        glGenBuffers(1, &m_uvVBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_uvVBO);
        glBufferData(GL_ARRAY_BUFFER, m_uvBuffer.size() * 4, m_uvBuffer.data(), GL_STATIC_DRAW);
    }

    if (vertexUsage & VertexUsageTangent) {
        glGenBuffers(1, &m_tangentVBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_tangentVBO);
        glBufferData(GL_ARRAY_BUFFER, m_tangentBuffer.size() * 4, m_tangentBuffer.data(), GL_STATIC_DRAW);
    }
    
    if (m_skinned) {
        glGenBuffers(1, &m_boneIndexVBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_boneIndexVBO);
        glBufferData(GL_ARRAY_BUFFER, m_boneIndexBuffer.size() * 4 * sizeof(int), m_boneIndexBuffer.data(), GL_STATIC_DRAW);
        glGenBuffers(1, &m_boneWeightVBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_boneWeightVBO);
        glBufferData(GL_ARRAY_BUFFER, m_boneWeightBuffer.size() * 4 * sizeof(float), m_boneWeightBuffer.data(), GL_STATIC_DRAW);
    }
}

void Mesh::BindBuffer(int vertexUsage/* = VertexUsagePN*/) {
    //glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexVBO);
    
    glBindBuffer(GL_ARRAY_BUFFER, m_positionVBO);
    glVertexAttribPointer(PositionIndex, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(PositionIndex);
    
    if (vertexUsage & VertexUsageNormal) {
        glBindBuffer(GL_ARRAY_BUFFER, m_normalVBO);
        glVertexAttribPointer(NormalIndex, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(NormalIndex);
    }
    
    if (vertexUsage & VertexUsageUV) {
        glBindBuffer(GL_ARRAY_BUFFER, m_uvVBO);
        glVertexAttribPointer(UVIndex, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(UVIndex);
    }
    
    if (vertexUsage & VertexUsageTangent) {
        glBindBuffer(GL_ARRAY_BUFFER, m_tangentVBO);
        glVertexAttribPointer(TangentIndex, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(TangentIndex);
    }
    
    if (m_skinned) {
        glBindBuffer(GL_ARRAY_BUFFER, m_boneIndexVBO);
        glVertexAttribIPointer(BoneIndexIndex, 4, GL_INT, 4 * sizeof(GLint), (GLvoid*)0);
        glEnableVertexAttribArray(BoneIndexIndex);

        glBindBuffer(GL_ARRAY_BUFFER, m_boneWeightVBO);
        glVertexAttribPointer(BoneWeightIndex, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(BoneWeightIndex);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
    
    glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO
}

//Mesh::PMesh Mesh::builtinMesh(const std::string& name)
//{
//    auto it = m_meshes.find(name);
//    if (it != m_meshes.end()) {
//        return it->second;
//    }
//    Debug::LogWarning("No built-in mesh called %d", name.c_str());
//    return nullptr;
//}
//
//void Mesh::Init()
//{
//#if FISHENGINE_PLATFORM_WINDOWS
//    const std::string root_dir = "../../assets/models/";
//#else
//    const std::string root_dir = "/Users/yushroom/program/graphics/FishEngine/assets/models/";
//#endif
//    for (auto& n : std::vector<std::string>{"cone", "cube", "plane", "quad", "sphere"}) {
//        m_meshes[n] = Mesh::CreateFromObjFile(root_dir+n+".obj");
//    }
//}

SimpleMesh::SimpleMesh(const float* positionBuffer, uint32_t vertexCount, GLenum drawMode)
    : m_positionBuffer(positionBuffer, positionBuffer+vertexCount*3),
    m_drawMode(drawMode)
{
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);
    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, m_positionBuffer.size()*sizeof(GLfloat), m_positionBuffer.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(PositionIndex, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(PositionIndex);
}

void SimpleMesh::Render() const
{
    glBindVertexArray(m_VAO);
    glDrawArrays(m_drawMode, 0, m_positionBuffer.size()/3);
    glBindVertexArray(0);
}

NAMESPACE_FISHENGINE_END
