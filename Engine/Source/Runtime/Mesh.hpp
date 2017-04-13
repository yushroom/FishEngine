#ifndef Mesh_hpp
#define Mesh_hpp

#include "Object.hpp"
#include "GLEnvironment.hpp"
#include "Matrix4x4.hpp"
#include "Bounds.hpp"
#include "IntVector.hpp"
#include "ReflectClass.hpp"
#include "PrimitiveType.hpp"
#include "BoneWeight.hpp"
#include "Vector3.hpp"
#include "Vector2.hpp"

namespace FishEditor
{
	class FBXImporter;
}

namespace FishEngine
{
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

		// noncopyable
		Mesh(const Mesh&) = delete;
		void operator=(const Mesh&) = delete;

		Mesh(Mesh&& m) = delete;

		~Mesh();

		
		// Returns state of the Read / Write Enabled checkbox when model was imported.
		bool isReadable() const
		{
			return m_isReadable;
		}
		
		bool canAccess() const;
		int blendShapeCount() const;
		int vertexBufferCount() const;
		
		
		// The bounding volume of the mesh.
		// This is the axis-aligned bounding box of the mesh in its local space (that is, not affected by the transform).
		// Note that the Renderer.bounds property is similar but returns the bounds in world space.
		Bounds bounds() const
		{
			return m_bounds;
		}

		// Returns the number of vertices in the Mesh (Read Only).
		uint32_t vertexCount() const
		{
			return m_vertexCount;
		}
		
		// The number of sub-Meshes. Every Mesh has a separate triangle list.
		// See Also: GetTriangles, SetTriangles.
		uint32_t subMeshCount() const
		{
			return m_subMeshCount;// static_cast<uint32_t>(m_subMeshIndexOffset.size());
		}
		
		
		const std::vector<BoneWeight> & boneWeights() const
		{
			return m_boneWeights;
		}
		
		
		// The bind pose is the inverse of the transformation matrix of the bone, when the bone is in the bind pose.
		const std::vector<Matrix4x4> & bindposes() const
		{
			return m_bindposes;
		}
		
		const std::vector<Vector3> & vertices() const
		{
			return m_vertices;
		}
		
		const std::vector<Vector3> & normals() const
		{
			return m_normals;
		}
		
		const std::vector<Vector3> & tangents() const
		{
			return m_tangents;
		}
		
		const std::vector<Vector2> & uv() const
		{
			return m_uv;
		}
		
		const std::vector<Vector2> & uv2() const;
		const std::vector<Vector2> & uv3() const;
		const std::vector<Vector2> & uv4() const;
		const std::vector<Color> & colors() const;
		//const std::vector<Color32> & colors32() const;
		
		
		// An array containing all triangles in the Mesh.
		// The array is a list of triangles that contains indices into the vertex array. The size of the triangle array must always be a multiple of 3. Vertices can be shared by simply indexing into the same vertex. If the Mesh contains multiple sub-Meshes (Materials), the triangle list will contain all triangles of all submeshes. When you assign a triangle array, the subMeshCount is set to 1. If you want to have multiple sub-Meshes, use subMeshCount and SetTriangles.
		// It is recommended to assign a triangle array after assigning the vertex array, in order to avoid out of bounds errors.
		const std::vector<uint32_t> & triangles() const
		{
			return m_triangles;
		}

		
		
		// Sets the index buffer for the sub-Mesh.
		// A sub-Mesh is simply a separate indices array. When the Mesh Renderer uses multiple Materials, you should ensure that there are as many sub-Meshes as Materials.
		// SetTriangles and ::triangles always set the Mesh to be composed of triangle faces. Use SetIndices to create a Mesh composed of lines or points.
		// See Also: subMeshCount, MeshTopology enum.
		//void SetIndices(std::vector<uint32_t> const & indices, int submesh, bool calculateBounds = true);
		
		
		// Retrieves a native (underlying graphics API) pointer to the vertex buffer.
		// bufferIndex	Which vertex buffer to get (some Meshes might have more than one). See vertexBufferCount.
		unsigned int GetNativeVertexBufferPtr(int bufferIndex);
		
		// Retrieves a native (underlying graphics API) pointer to the vertex buffer.
		unsigned int GetNativeIndexBufferPtr();
		
		// Recalculate the bounding volume of the Mesh from the vertices.
		void RecalculateBounds();
		
		// Recalculates the normals of the Mesh from the triangles and vertices.
		void RecalculateNormals();
		
		//Optimize mesh for frequent updates.
		// Call this before assigning vertices to get better performance when continually updating the Mesh. Internally, this makes the Mesh use "dynamic buffers" in the underlying graphics API, which are more efficient when Mesh data changes often.
		void MarkDynamic();
		
		// Upload previously done Mesh modifications to the graphics API.
		// <param name="markNoLongerReadable">Frees up system memory copy of mesh data when set to true.</param>
		void UploadMeshData(bool markNoLogerReadable = true);
		
		
		// Set the UVs for a given chanel.
		// Sets the UVs for a channel. Can either be Vector2, Vector3, or Vector4 list.
		void SetUVs(int channel, std::vector<Vector2> const & uvs);
		
		// Get the UVs for a given chanel.
		// Gets the UVs for a channel. Can either be Vector2, Vector3, or Vector4 list.
		void GetUVs(int channel, std::vector<Vector2> & uvs) const;
		
		// Returns the triangle list for the sub-Mesh.
		// A sub-Mesh is simply a separate triangle list. When the Mesh Renderer uses multiple Materials, you should ensure that there are as many sub-Meshes as Materials.
		const std::vector<uint32_t> & GetTriangles(int submesh) const;
		
		// Returns the index buffer for the sub-Mesh.
		// The layout of indices depends on the topology of a sub-Mesh. For example, for a triangular Mesh, each triangle results in three indices.
		//const std::vector<uint32_t> & GetIndices(int submesh) const;
		
	public:
		void Clear();
		
		// -1: reander all submeshes
		void Render(int subMeshIndex = -1);
		//void renderPatch(const Shader& shader);
		// Returns the number of vertices in the Mesh
		
		//static void Init();
		//static PMesh builtinMesh(const std::string& name);
		
		bool   m_skinned = false; // temp
		
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
		
	private:
		
		
		

	public:
		int						m_subMeshCount = 1;
		std::vector<Vector3>    m_vertices;
		std::vector<Vector3>    m_normals;
		std::vector<Vector2>    m_uv;
		std::vector<Vector3>    m_tangents;
		std::vector<uint32_t>   m_triangles;
		std::vector<uint32_t>	m_subMeshIndexOffset;	// index start position
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

		Meta(NonSerializable)
		GLuint m_TFBO = 0;              // transform feedback buffer object, for Animation
		
		Meta(NonSerializable)
		GLuint m_animationOutputVAO = 0;
		
		Meta(NonSerializable)
		GLuint m_animationOutputVBO = 0; // animation output buffer object

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
