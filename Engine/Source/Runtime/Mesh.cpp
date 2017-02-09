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
#include "ShaderVariables_gen.hpp"
#include "Serialization/archives/binary.hpp"

using namespace std;

namespace FishEngine
{

    BoneWeight::BoneWeight()
    {
        for (int i = 0; i < MaxBoneForEachVertex; ++i)
        {
            boneIndex[i] = 0;
            weight[i] = 0.f;
        }
    }

    void BoneWeight::AddBoneData(uint32_t boneIndex, float weight)
    {
        for (int i = 0; i < MaxBoneForEachVertex; ++i)
        {
            if (this->weight[i] == 0.0f)
            {
                this->boneIndex[i] = boneIndex;
                this->weight[i] = weight;
                return;
            }
        }
        Debug::LogError("BoneWeight::AddBoneData");
    }


    //std::map<std::string, Mesh::PMesh> Mesh::m_meshes;

    //Mesh::Mesh(const int n_vertex, const int n_face, float* positions, uint32_t* indices)
    //    : m_positionBuffer(positions, positions + n_vertex * 3),
    //    m_indexBuffer(indices, indices + n_face * 3)
    //{
    //    GenerateBuffer((int)VertexUsage::Position);
    //    BindBuffer((int)VertexUsage::Position);
    //}

    //Mesh::Mesh(std::vector<float> position_buffer, std::vector<uint32_t> index_buffer)
    //    : m_positionBuffer(position_buffer), m_indexBuffer(index_buffer)
    //{
    //    GenerateBuffer((int)VertexUsage::Position);
    //    BindBuffer((int)VertexUsage::Position);
    //}

    //Mesh::Mesh(const int n_vertex, const int n_face, float* positions, float* normals, uint32_t* indices)
    //    : m_positionBuffer(positions, positions + n_vertex * 3),
    //    m_normalBuffer(normals, normals + n_vertex * 3),
    //    m_indexBuffer(indices, indices + n_face * 3)
    //{
    //    GenerateBuffer((int)VertexUsage::PN);
    //    BindBuffer((int)VertexUsage::PN);
    //}

    Mesh::Mesh(std::vector<float> &&positionBuffer, std::vector<float> &&normalBuffer, std::vector<float> &&uvBuffer, std::vector<float> &&tangentBuffer, std::vector<uint32_t> &&indexBuffer)
        : m_positionBuffer(positionBuffer),
          m_normalBuffer(normalBuffer),
          m_uvBuffer(uvBuffer),
          m_tangentBuffer(tangentBuffer),
          m_indexBuffer(indexBuffer)
    {
		m_vertexCount = positionBuffer.size() / 3;
		m_triangleCount = indexBuffer.size() / 3;
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

    Mesh::~Mesh()
    {
        // Properly de-allocate all resources once they've outlived their purpose
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_positionVBO);
        glDeleteBuffers(1, &m_normalVBO);
        glDeleteBuffers(1, &m_tangentVBO);
        glDeleteBuffers(1, &m_indexVBO);
    }


    void Mesh::UploadMeshData(bool markNoLogerReadable /*= true*/)
    {
		if (m_uploaded)
			return;
        GenerateBuffer();
        BindBuffer();
        glCheckError();

        m_vertexCount = static_cast<uint32_t>(m_positionBuffer.size() / 3);
        m_triangleCount = static_cast<uint32_t>(m_indexBuffer.size() / 3);
        m_isReadable = !markNoLogerReadable;
        if (markNoLogerReadable)
        {
            Clear();
        }
        m_uploaded = true;
    }

    void Mesh::Clear()
    {
        m_positionBuffer.clear();
        m_normalBuffer.clear();
        m_uvBuffer.clear();
        m_tangentBuffer.clear();
        m_indexBuffer.clear();
        //m_boneIndexBuffer.clear();
        //m_boneWeightBuffer.clear();
    }

    void Mesh::Render()
    {
        //if (!m_uploaded)
        //{
        //    UploadMeshData();
        //}
        assert(m_uploaded);
        glBindVertexArray(m_VAO);
        glDrawElements(GL_TRIANGLES, m_triangleCount*3, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

	void Mesh::ToBinary(std::ostream & os)
    {
#if 0
		printf("%s", name().c_str());
		auto p = m_positionBuffer.data();
		for (int i = 0; i < vertexCount(); ++i)
		{
			printf("%ff,%ff,%fff,", *p, *(p+1), *(p+2));
			p +=3;
		}
		printf("\n");
		p = m_normalBuffer.data();
		for (int i = 0; i < vertexCount(); ++i)
		{
			printf("%ff,%ff,%ff,", *p, *(p+1), *(p+2));
			p +=3;
		}
		printf("\n");
		p = m_uvBuffer.data();
		for (int i = 0; i < vertexCount(); ++i)
		{
			printf("%ff,%ff,", *p, *(p+1));
			p +=2;
		}
		printf("\n");
		p = m_tangentBuffer.data();
		for (int i = 0; i < vertexCount(); ++i)
		{
			printf("%ff,%ff,%ff,", *p, *(p+1), *(p+2));
			p +=3;
		}
		printf("\n");
		auto pp = m_indexBuffer.data();
		for (int i = 0; i < m_triangleCount; ++i)
		{
			printf("%d,%d,%d,", *pp, *(pp+1), *(pp+2));
			pp +=3;
		}
#else
		//std::ofstream fout(m_name+".bin", std::ios::binary);
		//os << m_vertexCount << m_triangleCount;
		os.write((char*)&m_vertexCount, sizeof(m_vertexCount));
		os.write((char*)&m_triangleCount, sizeof(m_triangleCount));
		os.write((char*)m_positionBuffer.data(), sizeof(decltype(m_positionBuffer)::value_type) * m_positionBuffer.size());
		os.write((char*)m_normalBuffer.data(), sizeof(decltype(m_normalBuffer)::value_type) * m_normalBuffer.size());
		os.write((char*)m_uvBuffer.data(), sizeof(decltype(m_uvBuffer)::value_type) * m_uvBuffer.size());
		os.write((char*)m_tangentBuffer.data(), sizeof(decltype(m_tangentBuffer)::value_type) * m_tangentBuffer.size());
		os.write((char*)m_indexBuffer.data(), sizeof(decltype(m_indexBuffer)::value_type) * m_indexBuffer.size());
//		for (auto const f : m_positionBuffer)
//			os << f;
//		for (auto const f : m_normalBuffer)
//			os << f;
//		for (auto const f : m_uvBuffer)
//			os << f;
//		for (auto const f : m_tangentBuffer)
//			os << f;
//		for (auto const f : m_indexBuffer)
//			os << f;
#endif
    }

    MeshPtr Mesh::FromBinary(std::istream &is)
    {
		auto mesh = std::make_shared<Mesh>();
        //is >> mesh->m_vertexCount >> mesh->m_triangleCount;
		is.read((char*)&mesh->m_vertexCount, sizeof(mesh->m_vertexCount));
		is.read((char*)&mesh->m_triangleCount, sizeof(mesh->m_triangleCount));
        mesh->m_positionBuffer.resize(mesh->m_vertexCount * 3);
        mesh->m_normalBuffer.resize(mesh->m_vertexCount * 3);
        mesh->m_uvBuffer.resize(mesh->m_vertexCount * 2);
        mesh->m_tangentBuffer.resize(mesh->m_vertexCount * 3);
        mesh->m_indexBuffer.resize(mesh->m_triangleCount * 3);
//        for (auto & f : mesh->m_positionBuffer)
//            is >> f;
//        for (auto & f : mesh->m_normalBuffer)
//            is >> f;
//        for (auto & f : mesh->m_uvBuffer)
//            is >> f;
//        for (auto & f : mesh->m_tangentBuffer)
//            is >> f;
//        for (auto & f : mesh->m_indexBuffer)
//            is >> f;
		is.read((char*)mesh->m_positionBuffer.data(), sizeof(decltype(mesh->m_positionBuffer)::value_type) * mesh->m_positionBuffer.size());
		is.read((char*)mesh->m_normalBuffer.data(), sizeof(decltype(m_normalBuffer)::value_type) * mesh->m_normalBuffer.size());
		is.read((char*)mesh->m_uvBuffer.data(), sizeof(decltype(m_uvBuffer)::value_type) * mesh->m_uvBuffer.size());
		is.read((char*)mesh->m_tangentBuffer.data(), sizeof(decltype(m_tangentBuffer)::value_type) * mesh->m_tangentBuffer.size());
		is.read((char*)mesh->m_indexBuffer.data(), sizeof(decltype(m_indexBuffer)::value_type) * mesh->m_indexBuffer.size());
		return mesh;
    }

    //void Model::renderPatch(const Shader &shader) {
    //    glBindVertexArray(m_VAO);
    //    glDrawElements(GL_PATCHES, (GLsizei)m_indexBuffer.size(), GL_UNSIGNED_INT, 0);
    //    glBindVertexArray(0);
    //}

    void Mesh::GenerateBuffer()
    {
        // VAO
		assert(m_VAO == 0);
        glGenVertexArrays(1, &m_VAO);

        // index vbo
        glGenBuffers(1, &m_indexVBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexVBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer.size() * 4, m_indexBuffer.data(), GL_STATIC_DRAW);

        glGenBuffers(1, &m_positionVBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_positionVBO);
        glBufferData(GL_ARRAY_BUFFER, m_positionBuffer.size() * 4, m_positionBuffer.data(), GL_STATIC_DRAW);

        //if (vertexUsage & (int)VertexUsage::Normal) {
            glGenBuffers(1, &m_normalVBO);
            glBindBuffer(GL_ARRAY_BUFFER, m_normalVBO);
            glBufferData(GL_ARRAY_BUFFER, m_normalBuffer.size() * 4, m_normalBuffer.data(), GL_STATIC_DRAW);
        //}

        //if (vertexUsage & (int)VertexUsage::UV) {
            glGenBuffers(1, &m_uvVBO);
            glBindBuffer(GL_ARRAY_BUFFER, m_uvVBO);
            glBufferData(GL_ARRAY_BUFFER, m_uvBuffer.size() * 4, m_uvBuffer.data(), GL_STATIC_DRAW);
        //}

        //if (vertexUsage & (int)VertexUsage::Tangent) {
            glGenBuffers(1, &m_tangentVBO);
            glBindBuffer(GL_ARRAY_BUFFER, m_tangentVBO);
            glBufferData(GL_ARRAY_BUFFER, m_tangentBuffer.size() * 4, m_tangentBuffer.data(), GL_STATIC_DRAW);
        //}

        if (m_skinned) {
            //glGenTransformFeedbacks(1, &m_TFBO);

            //glGenVertexArrays(1, &m_animationOutputVAO);

            ////glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_TFBO);
            //glGenBuffers(1, &m_animationOutputVBO);
            //glBindBuffer(GL_ARRAY_BUFFER, m_animationOutputVBO);
            //glBufferData(GL_ARRAY_BUFFER, m_positionBuffer.size() * 4, NULL, GL_DYNAMIC_COPY);
            ////glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_animationOutputVAO); //bind as TF output

            glGenBuffers(1, &m_boneIndexVBO);
            glBindBuffer(GL_ARRAY_BUFFER, m_boneIndexVBO);
            glBufferData(GL_ARRAY_BUFFER, m_boneIndexBuffer.size() * 4 * sizeof(int), m_boneIndexBuffer.data(), GL_STATIC_DRAW);
            glGenBuffers(1, &m_boneWeightVBO);
            glBindBuffer(GL_ARRAY_BUFFER, m_boneWeightVBO);
            glBufferData(GL_ARRAY_BUFFER, m_boneWeightBuffer.size() * 4 * sizeof(float), m_boneWeightBuffer.data(), GL_STATIC_DRAW);
        }
    }

    void Mesh::BindBuffer() {
        glBindVertexArray(m_VAO);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexVBO);

        glBindBuffer(GL_ARRAY_BUFFER, m_positionVBO);
        glVertexAttribPointer(PositionIndex, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(PositionIndex);

        //if (vertexUsage & (int)VertexUsage::Normal) {
            glBindBuffer(GL_ARRAY_BUFFER, m_normalVBO);
            glVertexAttribPointer(NormalIndex, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
            glEnableVertexAttribArray(NormalIndex);
        //}

        //if (vertexUsage & (int)VertexUsage::UV) {
            glBindBuffer(GL_ARRAY_BUFFER, m_uvVBO);
            glVertexAttribPointer(UVIndex, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
            glEnableVertexAttribArray(UVIndex);
        //}

        //if (vertexUsage & (int)VertexUsage::Tangent) {
            glBindBuffer(GL_ARRAY_BUFFER, m_tangentVBO);
            glVertexAttribPointer(TangentIndex, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
            glEnableVertexAttribArray(TangentIndex);
        //}

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
        : m_positionBuffer(positionBuffer, positionBuffer + vertexCount * 3),
        m_drawMode(drawMode)
    {
        glGenVertexArrays(1, &m_VAO);
        glBindVertexArray(m_VAO);
        glGenBuffers(1, &m_VBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, m_positionBuffer.size() * sizeof(GLfloat), m_positionBuffer.data(), GL_DYNAMIC_DRAW);
        glVertexAttribPointer(PositionIndex, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(PositionIndex);
    }

    //void SimpleMesh::BindNewBuffer(const float* positionBuffer, uint32_t vertexCount)
    //{
    //    glBindVertexArray(m_VAO);
    //    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    //    glBufferData(GL_ARRAY_BUFFER, vertexCount*3*sizeof(GLfloat), positionBuffer, GL_DYNAMIC_DRAW);
    //    //glVertexAttribPointer(PositionIndex, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    //    //glEnableVertexAttribArray(PositionIndex);
    //}

    void SimpleMesh::Render() const
    {
        glBindVertexArray(m_VAO);
        glDrawArrays(m_drawMode, 0, static_cast<GLsizei>(m_positionBuffer.size() / 3));
        glBindVertexArray(0);
    }

    void DynamicMesh::Render(const float* positionBuffer, uint32_t vertexCount, GLenum drawMode)
    {
        if (m_VAO == 0)
            glGenVertexArrays(1, &m_VAO);
        glBindVertexArray(m_VAO);
        if (m_VBO == 0)
            glGenBuffers(1, &m_VBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, vertexCount*3*sizeof(GLfloat), positionBuffer, GL_DYNAMIC_DRAW);
        glVertexAttribPointer(PositionIndex, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(PositionIndex);
        glDrawArrays(m_drawMode, 0, vertexCount);
        glBindVertexArray(0);
    }
}
