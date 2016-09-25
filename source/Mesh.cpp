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

std::map<std::string, Mesh::PMesh> Mesh::m_meshes;

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


Mesh::Mesh(const std::string& objModelPath, int vertexUsage)
{
    FromObjFile(objModelPath, vertexUsage);
}

Mesh::~Mesh() {
    // Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_positionVBO);
    glDeleteBuffers(1, &m_normalVBO);
    glDeleteBuffers(1, &m_tangentVBO);
    glDeleteBuffers(1, &m_indexVBO);
}

FishEngine::Mesh::PMesh FishEngine::Mesh::CreateFromObjFile(const std::string path, int vertexUsage /*= VertexUsagePNUT*/, MeshLoadFlags flags /*= 0*/)
{
    auto m = std::make_shared<Mesh>();
    m->FromObjFile(path, vertexUsage, flags);
    //m_meshes.push_back(m);
    return m;
}

void Mesh::FromObjFile(const std::string path, int vertexUsage, MeshLoadFlags flags)
{
    Assimp::Importer importer;
    unsigned int load_option =
        aiProcess_Triangulate
        //| aiProcess_SortByPType
        | aiProcess_CalcTangentSpace
        //| aiProcess_JoinIdenticalVertices
        //| aiProcess_FixInfacingNormals
        //| aiProcess_OptimizeGraph
        //| aiProcess_OptimizeMeshes
        //| aiProcess_FlipUVs
        //| aiProcess_ConvertToLeftHanded
        ;
//#ifdef GLM_FORCE_LEFT_HANDED
    load_option |= aiProcess_ConvertToLeftHanded;
//#endif
    if (flags & MeshLoadFlag_RegenerateNormal) {
        importer.SetPropertyInteger(AI_CONFIG_PP_RVC_FLAGS, aiComponent_NORMALS);
        load_option |= aiProcess_GenSmoothNormals |aiProcess_RemoveComponent;
    } else {
        load_option |= aiProcess_GenNormals;
    }
    bool load_tangent = (vertexUsage & VertexUsageTangent) != 0;
    if (load_tangent)
        load_option |= aiProcess_CalcTangentSpace;
    const aiScene* scene = importer.ReadFile(path.c_str(), load_option);
    if (!scene) {
        Debug::LogError("Can not open file %s", path.c_str());
        abort();
    }
    
    m_name = split(path, "/").back();

    int n_vertices = 0;
    int n_triangles = 0;
    int n_bones = 0;

    for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
        aiMesh* mesh = scene->mMeshes[i];
        n_vertices += mesh->mNumVertices;
        n_triangles += mesh->mNumFaces;
        n_bones += mesh->mNumBones;
    }

    m_positionBuffer.reserve(n_vertices * 3);
    m_normalBuffer.reserve(n_vertices * 3);
    m_uvBuffer.reserve(n_vertices * 2);
    m_indexBuffer.reserve(n_triangles * 3);
    m_tangentBuffer.reserve(n_triangles * 3);
    m_boneWeights.reserve(n_bones);
    
    int idx = 0;
    for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[i];
        bool has_uv = mesh->HasTextureCoords(0);
        //if (!has_uv)
        //	printf("mesh[%d] do not have uv!\n", i);
        assert(!((!has_uv) && (vertexUsage & VertexUsageUV)));
        bool load_uv = (vertexUsage & VertexUsageUV) != 0;

        for (unsigned int j = 0; j < mesh->mNumVertices; ++j) {
            auto& v = mesh->mVertices[j];
            m_positionBuffer.push_back(v.x);
            m_positionBuffer.push_back(v.y);
            m_positionBuffer.push_back(v.z);

            auto& n = mesh->mNormals[j];
            m_normalBuffer.push_back(n.x);
            m_normalBuffer.push_back(n.y);
            m_normalBuffer.push_back(n.z);

            if (has_uv) {
                auto& uv = mesh->mTextureCoords[0][j];
                m_uvBuffer.push_back(uv.x);
                m_uvBuffer.push_back(uv.y);
            }

            if (load_tangent) {
                auto& t = mesh->mTangents[j];
                m_tangentBuffer.push_back(t.x);
                m_tangentBuffer.push_back(t.y);
                m_tangentBuffer.push_back(t.z);
            }
        }
        for (unsigned int j = 0; j < mesh->mNumFaces; j++) {
            auto& face = mesh->mFaces[j];
            assert(face.mNumIndices == 3);
            for (int fi = 0; fi < 3; ++fi)
                m_indexBuffer.push_back(face.mIndices[fi] + idx);
        }
        idx += mesh->mNumFaces*3;
    }

    GenerateBuffer(vertexUsage);
    BindBuffer(vertexUsage);
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

    glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
    
    glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO
}

//Model& Model::getQuad()
//{
//#if defined(_WIN32)
//	//static Model quad("D:/program/RFGL/models/Quad.obj", VertexUsagePNUT);
//	static Model quad("../models/Quad.obj", VertexUsagePNUT);
//#else
//    static Model quad("/Users/yushroom/program/graphics/RFGL/models/Quad.obj", VertexUsagePNUT);
//#endif
//    return quad;
//}
//
//Model& Model::getBox()
//{
//#if defined(_WIN32)
//	static Model box("../models/cube.obj", VertexUsagePNUT);
//#else
//	static Model box("/Users/yushroom/program/graphics/RFGL/models/box.obj", VertexUsagePNUT);
//#endif
//    return box;
//}
//
//Model& Model::getSphere()
//{
//#if defined(_WIN32)
//	//static Model sphere("D:/program/RFGL/models/Sphere.obj", VertexUsagePNUT);
//	static Model sphere("../models/Sphere.obj", VertexUsagePNUT);
//#else
//    static Model sphere("/Users/yushroom/program/github/SeparableSSS/SeparableSSS/Models/Sphere.obj", VertexUsagePNUT);
//#endif
//    return sphere;
//}
//
//Model& Model::getIcosahedron()
//{
//    uint32_t indices[] = {
//        2, 1, 0,
//        3, 2, 0,
//        4, 3, 0,
//        5, 4, 0,
//        1, 5, 0,
//        
//        11, 6,  7,
//        11, 7,  8,
//        11, 8,  9,
//        11, 9,  10,
//        11, 10, 6,
//        
//        1, 2, 6,
//        2, 3, 7,
//        3, 4, 8,
//        4, 5, 9,
//        5, 1, 10,
//        
//        2,  7, 6,
//        3,  8, 7,
//        4,  9, 8,
//        5, 10, 9,
//        1, 6, 10 };
//    
//    float vertices[] = {
//        0.000f,  0.000f,  1.000f,
//        0.894f,  0.000f,  0.447f,
//        0.276f,  0.851f,  0.447f,
//        -0.724f,  0.526f,  0.447f,
//        -0.724f, -0.526f,  0.447f,
//        0.276f, -0.851f,  0.447f,
//        0.724f,  0.526f, -0.447f,
//        -0.276f,  0.851f, -0.447f,
//        -0.894f,  0.000f, -0.447f,
//        -0.276f, -0.851f, -0.447f,
//        0.724f, -0.526f, -0.447f,
//        0.000f,  0.000f, -1.000f };
//    static Model m(sizeof(vertices)/12, sizeof(indices)/12, vertices, indices);
//    return m;
//
//}

Mesh::PMesh Mesh::builtinMesh(const std::string& name)
{
    auto it = m_meshes.find(name);
    if (it != m_meshes.end()) {
        return it->second;
    }
    Debug::LogWarning("No built-in mesh called %d", name.c_str());
    return nullptr;
}

void Mesh::Init()
{
#if FISHENGINE_PLATFORM_WINDOWS
    const std::string root_dir = "../../assets/models/";
#else
    const std::string root_dir = "/Users/yushroom/program/graphics/FishEngine/assets/models/";
#endif
    for (auto& n : std::vector<std::string>{"cone", "cube", "plane", "quad", "sphere"}) {
        m_meshes[n] = Mesh::CreateFromObjFile(root_dir+n+".obj");
    }
}

NAMESPACE_FISHENGINE_END
