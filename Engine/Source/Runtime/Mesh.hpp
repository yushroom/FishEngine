#ifndef Mesh_hpp
#define Mesh_hpp

#include "Object.hpp"
#include "GLEnvironment.hpp"
#include "Matrix4x4.hpp"
#include "Bounds.hpp"
#include "IntVector.hpp"
#include "ReflectClass.hpp"
#include "PrimitiveType.hpp"

#include "Vector3.hpp"
#include "Vector2.hpp"

namespace FishEditor
{
	class FBXImporter;
}

namespace FishEngine
{
	static constexpr int MaxBoneForEachVertex = 4;

	struct FE_EXPORT BoneWeight
	{
		//union {
		//    struct {
		//        int boneIndex0;
		//        int boneIndex1;
		//        int boneIndex2;
		//        int boneIndex3;
		//    };
		//    int boneIndex[4];
		//};
		//union {
		//    struct {
		//        float weight0;
		//        float weight1;
		//        float weight2;
		//        float weight3;
		//    };
		//    float weight[4];
		//};

		Meta(NonSerializable)
		int boneIndex[MaxBoneForEachVertex];

		Meta(NonSerializable)
		float weight[MaxBoneForEachVertex];

		BoneWeight();

		void AddBoneData(uint32_t boneIndex, float weight);
	};

	class FE_EXPORT Mesh : public Object
	{
	public:
		InjectClassName(Mesh)

		Mesh() = default;
		//Mesh(std::vector<float> position_buffer, std::vector<uint32_t> index_buffer);
		//Mesh(const int n_vertex, const int n_face, float* positions, uint32_t* indices);
		//Mesh(const int n_vertex, const int n_face, float* positions, float* normals, uint32_t* indices);
		Mesh(std::vector<Vector3>	&& vertices,
			std::vector<Vector3>	&& normals,
			std::vector<Vector2>	&& uv,
			std::vector<Vector3>	&& tangents,
			std::vector<uint32_t>	&& triangles);

		Mesh(const Mesh&) = delete;
		void operator=(const Mesh&) = delete;

		Mesh(Mesh&& m) = delete;

		~Mesh();

		// Returns state of the Read / Write Enabled checkbox when model was imported.
		bool isReadable() const
		{
			return m_isReadable;
		}

		uint32_t vertexCount() const
		{
			return m_vertexCount;
		}

		uint32_t triangles() const
		{
			return m_triangleCount;
		}

		// Upload previously done Mesh modifications to the graphics API.
		// <param name="markNoLongerReadable">Frees up system memory copy of mesh data when set to true.</param>
		void UploadMeshData(bool markNoLogerReadable = true);

		void Clear();

		void Render();
		//void renderPatch(const Shader& shader);

		// The bounding volume of the mesh.
		Bounds bounds() const
		{
			return m_bounds;
		}

		//static void Init();
		//static PMesh builtinMesh(const std::string& name);

		bool   m_skinned = false; // temp

		// The bind pose is the inverse of the transformation matrix of the bone, when the bone is in the bind pose.
		std::vector<Matrix4x4> const & bindposes() const
		{
			return m_bindposes;
		}

		int boneCount() const
		{
			return static_cast<int>(m_boneNames.size());
		}

		// temp
		void ToBinaryFile(std::ostream & os);
		static MeshPtr FromBinaryFile(std::istream & is);

		static MeshPtr FromTextFile(std::istream & is);

		static void Init(std::string const & rootDir);
		static MeshPtr builtinMesh(const PrimitiveType type);

	public:
		std::vector<Vector3>    m_vertices;
		std::vector<Vector3>    m_normals;
		std::vector<Vector2>    m_uv;
		std::vector<Vector3>    m_tangents;
		std::vector<uint32_t>   m_triangles;
		//		std::vector<Int4>       m_boneIndexBuffer;
		//		std::vector<Vector4>    m_boneWeightBuffer;

		//std::map<std::string, int> m_boneNameToIndex;

		// The bind poses. The bind pose at each index refers to the bone with the same index.
		// The bind pose is the inverse of the transformation matrix of the bone, when the bone is in the bind pose.
		std::vector<Matrix4x4>  m_bindposes;

		std::vector<std::string> m_boneNames;	// same size with m_binposes;

		// The bone weights of each vertex.
		// The size of the array is either the same as vertexCount or empty.
		// Each vertex can be affected by up to 4 different bones.All 4 bone weights should sum up to 1.
		Meta(NonSerializable)
		std::vector<BoneWeight> m_boneWeights;

	private:
		friend class FishEditor::Inspector;
		friend class FishEditor::ModelImporter;
		friend class FishEditor::FBXImporter;
		friend class MeshRenderer;
		friend class SkinnedMeshRenderer;
		//friend class Model;

		static std::map<PrimitiveType, MeshPtr> s_builtinMeshes;

		bool m_isReadable = false;
		bool m_uploaded = false;
		uint32_t m_vertexCount = 0;
		uint32_t m_triangleCount = 0;

		Bounds m_bounds;

		Meta(NonSerializable)
		GLuint m_VAO = 0;

		Meta(NonSerializable)
		GLuint m_indexVBO = 0;

		Meta(NonSerializable)
		GLuint m_positionVBO = 0;

		Meta(NonSerializable)
		GLuint m_normalVBO = 0;

		Meta(NonSerializable)
		GLuint m_uvVBO = 0;

		Meta(NonSerializable)
			GLuint m_tangentVBO = 0;

		Meta(NonSerializable)
		GLuint m_boneIndexVBO = 0;

		Meta(NonSerializable)
		GLuint m_boneWeightVBO = 0;

		//GLuint m_TFBO;              // transform feedback buffer object, for Animation
		//GLuint m_animationOutputVAO;
		//GLuint m_animationOutputVBO; // animation output buffer object

		//static std::map<std::string, PMesh> m_meshes;

		void GenerateBuffer();
		void BindBuffer();
	};


	// position buffer array
	class FE_EXPORT Meta(NonSerializable) SimpleMesh
	{
	public:
		//InjectClassName(SimpleMesh)

		// 3 components(float) for each vertex
		SimpleMesh(const float* positionBuffer, uint32_t vertexCount, GLenum drawMode);
		//SimpleMesh(std::vector<float> positionBuffer);

		//void BindNewBuffer(const float* positionBuffer, uint32_t vertexCount);

		void Render() const;

	private:
		friend class Gizmos;
		std::vector<float> m_positionBuffer;

		GLenum m_drawMode = GL_LINES;   // mode in glDrawArrays
		GLuint m_VAO = 0;
		GLuint m_VBO = 0;
	};

	class Meta(NonSerializable) DynamicMesh
	{
	public:
		//InjectClassName(DynamicMesh)

		DynamicMesh() = default;

		void Render(const float* positionBuffer, uint32_t vertexCount, GLenum drawMode);

	private:
		GLenum m_drawMode = GL_LINES;   // mode in glDrawArrays
		GLuint m_VAO = 0;
		GLuint m_VBO = 0;
	};
}

#endif /* Mesh_hpp */
