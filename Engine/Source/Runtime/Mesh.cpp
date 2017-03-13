#include "Mesh.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>

#include "Shader.hpp"
#include "Debug.hpp"
#include "Common.hpp"
#include "ShaderVariables_gen.hpp"
//#include "Serialization/archives/binary.hpp"
#include "generate/Enum_PrimitiveType.hpp"

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
		// more than MaxBoneForEachVertex
		Debug::LogWarning("[BoneWeight::AddBoneData] more than %d bones", MaxBoneForEachVertex);

		int minId = 0;
		float minWeight = this->weight[0];
		for (int i = 1; i < MaxBoneForEachVertex; ++i)
		{
			if (this->weight[i] < minWeight)
			{
				minId = i;
				minWeight = this->weight[i];
			}
		}
		if (minWeight < weight)
		{
			this->boneIndex[minId] = boneIndex;
			this->weight[minId] = weight;
		}
	}

	std::map<PrimitiveType, MeshPtr> Mesh::s_builtinMeshes;

	//std::map<std::string, Mesh::PMesh> Mesh::m_meshes;

	//Mesh::Mesh(const int n_vertex, const int n_face, float* positions, uint32_t* indices)
	//    : m_vertices(positions, positions + n_vertex * 3),
	//    m_triangles(indices, indices + n_face * 3)
	//{
	//    GenerateBuffer((int)VertexUsage::Position);
	//    BindBuffer((int)VertexUsage::Position);
	//}

	//Mesh::Mesh(std::vector<float> position_buffer, std::vector<uint32_t> index_buffer)
	//    : m_vertices(position_buffer), m_triangles(index_buffer)
	//{
	//    GenerateBuffer((int)VertexUsage::Position);
	//    BindBuffer((int)VertexUsage::Position);
	//}

	//Mesh::Mesh(const int n_vertex, const int n_face, float* positions, float* normals, uint32_t* indices)
	//    : m_vertices(positions, positions + n_vertex * 3),
	//    m_normals(normals, normals + n_vertex * 3),
	//    m_triangles(indices, indices + n_face * 3)
	//{
	//    GenerateBuffer((int)VertexUsage::PN);
	//    BindBuffer((int)VertexUsage::PN);
	//}

	Mesh::Mesh(std::vector<Vector3>	&& vertices,
		std::vector<Vector3>	&& normals,
		std::vector<Vector2>	&& uv,
		std::vector<Vector3>	&& tangents,
		std::vector<uint32_t>	&& triangles)
		: m_vertices(vertices),
		m_normals(normals),
		m_uv(uv),
		m_tangents(tangents),
		m_triangles(triangles)
	{
		m_vertexCount = static_cast<uint32_t>(vertices.size());
		m_triangleCount = static_cast<uint32_t>(triangles.size() / 3);
	}

	//Mesh::Mesh(Mesh&& m)
	//{
	//    m_vertices = std::move(m.m_vertices);
	//    m_normals = std::move(m.m_normals);
	//    m_uv = std::move(m.m_uv);
	//    m_tangents = std::move(m.m_tangents);
	//    m_triangles = std::move(m.m_triangles);
	//    m_VAO = m.m_VAO;
	//    m_indexVBO = m.m_indexVBO;
	//    m_positionVBO = m.m_positionVBO;
	//    m_normalVBO = m.m_normalVBO;
	//    m_uvVBO = m.m_uvVBO;
	//    m_tangentVBO = m.m_tangentVBO;
	//    m.m_VAO = 0;
	//    m.m_indexVBO = 0;
	//    m.m_positionVBO = 0;
	//    m.m_normalVBO = 0;
	//    m.m_uvVBO = 0;
	//    m.m_tangentVBO = 0;
	//}


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

		//m_vertexCount = static_cast<uint32_t>(m_vertices.size());
		//m_triangleCount = static_cast<uint32_t>(m_triangles.size() / 3);
		m_isReadable = !markNoLogerReadable;
		if (markNoLogerReadable)
		{
			Clear();
		}
		m_uploaded = true;
	}

	void Mesh::Clear()
	{
		m_vertices.clear();
		m_normals.clear();
		m_uv.clear();
		m_tangents.clear();
		m_triangles.clear();
		//m_boneIndexBuffer.clear();
		//m_boneWeightBuffer.clear();
		m_boneWeights.clear();
	}

	void Mesh::Render()
	{
		//assert(m_uploaded);
		if (!m_uploaded)
			UploadMeshData();
		glBindVertexArray(m_VAO);
		glDrawElements(GL_TRIANGLES, m_triangleCount * 3, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	void Mesh::ToBinaryFile(std::ostream & os)
	{
		os.write((char*)&m_vertexCount, sizeof(m_vertexCount));
		os.write((char*)&m_triangleCount, sizeof(m_triangleCount));
		os.write((char*)m_vertices.data(), sizeof(decltype(m_vertices)::value_type) * m_vertices.size());
		os.write((char*)m_normals.data(), sizeof(decltype(m_normals)::value_type) * m_normals.size());
		os.write((char*)m_uv.data(), sizeof(decltype(m_uv)::value_type) * m_uv.size());
		os.write((char*)m_tangents.data(), sizeof(decltype(m_tangents)::value_type) * m_tangents.size());
		os.write((char*)m_triangles.data(), sizeof(decltype(m_triangles)::value_type) * m_triangles.size());
	}

	MeshPtr Mesh::FromBinaryFile(std::istream &is)
	{
		auto mesh = std::make_shared<Mesh>();
		is.read((char*)&mesh->m_vertexCount, sizeof(mesh->m_vertexCount));
		is.read((char*)&mesh->m_triangleCount, sizeof(mesh->m_triangleCount));
		mesh->m_vertices.resize(mesh->m_vertexCount);
		mesh->m_normals.resize(mesh->m_vertexCount);
		mesh->m_uv.resize(mesh->m_vertexCount);
		mesh->m_tangents.resize(mesh->m_vertexCount);
		mesh->m_triangles.resize(mesh->m_triangleCount * 3);
		is.read((char*)mesh->m_vertices.data(), sizeof(decltype(mesh->m_vertices)::value_type) * mesh->m_vertices.size());
		is.read((char*)mesh->m_normals.data(), sizeof(decltype(m_normals)::value_type) * mesh->m_normals.size());
		is.read((char*)mesh->m_uv.data(), sizeof(decltype(m_uv)::value_type) * mesh->m_uv.size());
		is.read((char*)mesh->m_tangents.data(), sizeof(decltype(m_tangents)::value_type) * mesh->m_tangents.size());
		is.read((char*)mesh->m_triangles.data(), sizeof(decltype(m_triangles)::value_type) * mesh->m_triangles.size());
		return mesh;
	}

	FishEngine::MeshPtr Mesh::FromTextFile(std::istream & is)
	{
		auto mesh = std::make_shared<Mesh>();
		is >> mesh->m_vertexCount >> mesh->m_triangleCount;
		mesh->m_vertices.resize(mesh->m_vertexCount);
		mesh->m_normals.resize(mesh->m_vertexCount);
		mesh->m_uv.resize(mesh->m_vertexCount);
		mesh->m_tangents.resize(mesh->m_vertexCount);
		mesh->m_triangles.resize(mesh->m_triangleCount * 3);
		float vx, vy, vz;
		Vector3 vmin(Mathf::Infinity, Mathf::Infinity, Mathf::Infinity);
		Vector3 vmax(Mathf::NegativeInfinity, Mathf::NegativeInfinity, Mathf::NegativeInfinity);
		for (uint32_t i = 0; i < mesh->m_vertexCount; ++i)
		{
			is >> vx >> vy >> vz;
			if (vmin.x > vx) vmin.x = vx;
			if (vmin.y > vy) vmin.y = vy;
			if (vmin.z > vz) vmin.z = vz;
			if (vmax.x < vx) vmax.x = vx;
			if (vmax.y < vy) vmax.y = vy;
			if (vmax.z < vz) vmax.z = vz;
			auto & v = mesh->m_vertices[i];
			v.x = vx;
			v.y = vy;
			v.z = vz;
		}
		mesh->m_bounds.SetMinMax(vmin, vmax);
		for (auto & f : mesh->m_normals)
			is >> f.x >> f.y >> f.z;
		for (auto & f : mesh->m_uv)
			is >> f.x >> f.y;
		for (auto & f : mesh->m_tangents)
			is >> f.x >> f.y >> f.z;
		for (auto & f : mesh->m_triangles)
			is >> f;
		return mesh;
	}

	void Mesh::Init(std::string const & rootDir)
	{
		Path dir = rootDir;

		for (auto & t : { PrimitiveType::Sphere, PrimitiveType::Capsule, PrimitiveType::Cylinder, PrimitiveType::Quad, PrimitiveType::Cube, PrimitiveType::Plane, PrimitiveType::Cone })
		{
			std::string n = FishEngine::EnumToString(t);
			std::ifstream is((dir / (n + ".txt")).string());
			auto mesh = Mesh::FromTextFile(is);
			mesh->setName(n);
			s_builtinMeshes[t] = mesh;
		}

		{
			std::vector<Vector3> p = { {-1,-1,0},  {1,-1,0},  {-1,1,0},  {1,1,0} };
			std::vector<Vector3> n = { {0,0,-1},  {0,0,-1},  {0,0,-1},  {0,0,-1} };
			std::vector<Vector2> uv = { {0,1},  {1,1},  {0,0},  {1,0} };
			std::vector<Vector3> t = { {1,0,0},  {1,0,0},  {1,0,0},  {1,0,0} };
			std::vector<uint32_t> index = { 2,1,0,  3,1,2 };
			auto mesh = std::make_shared<Mesh>(std::move(p), std::move(n), std::move(uv), std::move(t), std::move(index));
			mesh->setName("ScreenAlignedQuad");
			s_builtinMeshes[PrimitiveType::ScreenAlignedQuad] = mesh;
		}

		for (auto& pair : s_builtinMeshes)
		{
			pair.second->UploadMeshData();
		}
	}

	FishEngine::MeshPtr Mesh::builtinMesh(const PrimitiveType type)
	{
		return s_builtinMeshes[type];
	}

	//void Model::renderPatch(const Shader &shader) {
	//    glBindVertexArray(m_VAO);
	//    glDrawElements(GL_PATCHES, (GLsizei)m_triangles.size(), GL_UNSIGNED_INT, 0);
	//    glBindVertexArray(0);
	//}

	void Mesh::GenerateBuffer()
	{
		// VAO
		assert(m_VAO == 0);
		glGenVertexArrays(1, &m_VAO);

		// index VBO
		glGenBuffers(1, &m_indexVBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexVBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_triangles.size() * 4, m_triangles.data(), GL_STATIC_DRAW);

		glGenBuffers(1, &m_positionVBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_positionVBO);
		glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * 3 * 4, m_vertices.data(), GL_STATIC_DRAW);

		glGenBuffers(1, &m_normalVBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_normalVBO);
		glBufferData(GL_ARRAY_BUFFER, m_normals.size() * 3 * 4, m_normals.data(), GL_STATIC_DRAW);

		glGenBuffers(1, &m_uvVBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_uvVBO);
		glBufferData(GL_ARRAY_BUFFER, m_uv.size() * 2 * 4, m_uv.data(), GL_STATIC_DRAW);

		glGenBuffers(1, &m_tangentVBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_tangentVBO);
		glBufferData(GL_ARRAY_BUFFER, m_tangents.size() * 3 * 4, m_tangents.data(), GL_STATIC_DRAW);

		if (m_skinned)
		{
			std::vector<Int4> boneIndexBuffer;
			boneIndexBuffer.reserve(m_boneWeights.size());
			std::vector<Vector4> boneWeightBuffer;
			boneIndexBuffer.reserve(m_boneWeights.size());

			for (auto const & b : m_boneWeights)
			{
				boneIndexBuffer.emplace_back(b.boneIndex[0], b.boneIndex[1], b.boneIndex[2], b.boneIndex[3]);
				boneWeightBuffer.emplace_back(b.weight[0], b.weight[1], b.weight[2], b.weight[3]);
			}

			glGenBuffers(1, &m_boneIndexVBO);
			glBindBuffer(GL_ARRAY_BUFFER, m_boneIndexVBO);
			glBufferData(GL_ARRAY_BUFFER, boneIndexBuffer.size() * 4 * sizeof(int), boneIndexBuffer.data(), GL_STATIC_DRAW);
			glGenBuffers(1, &m_boneWeightVBO);
			glBindBuffer(GL_ARRAY_BUFFER, m_boneWeightVBO);
			glBufferData(GL_ARRAY_BUFFER, boneWeightBuffer.size() * 4 * sizeof(float), boneWeightBuffer.data(), GL_STATIC_DRAW);
		}
	}

	void Mesh::BindBuffer()
	{
		glBindVertexArray(m_VAO);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexVBO);

		glBindBuffer(GL_ARRAY_BUFFER, m_positionVBO);
		glVertexAttribPointer(PositionIndex, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(PositionIndex);

		glBindBuffer(GL_ARRAY_BUFFER, m_normalVBO);
		glVertexAttribPointer(NormalIndex, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(NormalIndex);

		glBindBuffer(GL_ARRAY_BUFFER, m_uvVBO);
		glVertexAttribPointer(UVIndex, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(UVIndex);

		glBindBuffer(GL_ARRAY_BUFFER, m_tangentVBO);
		glVertexAttribPointer(TangentIndex, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(TangentIndex);

		if (m_skinned)
		{
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
		glBufferData(GL_ARRAY_BUFFER, vertexCount * 3 * sizeof(GLfloat), positionBuffer, GL_DYNAMIC_DRAW);
		glVertexAttribPointer(PositionIndex, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(PositionIndex);
		glDrawArrays(m_drawMode, 0, vertexCount);
		glBindVertexArray(0);
	}
}
