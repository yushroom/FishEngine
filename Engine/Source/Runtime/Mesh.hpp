#ifndef Mesh_hpp
#define Mesh_hpp

#include "Object.hpp"
#include "GLEnvironment.hpp"
#include "Matrix4x4.hpp"
#include "Bounds.hpp"
#include "IntVector.hpp"
#include "ReflectClass.hpp"
#include "PrimitiveType.hpp"

namespace FishEngine
{
    static constexpr int MaxBoneForEachVertex = 4;

    struct BoneWeight
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

        int boneIndex[MaxBoneForEachVertex];
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
        Mesh(std::vector<float>      && positionBuffer,
             std::vector<float>      && normalBuffer,
             std::vector<float>      && uvBuffer,
             std::vector<float>      && tangentBuffer,
             std::vector<uint32_t>   && indexBuffer);

        Mesh(const Mesh&) = delete;
        void operator=(const Mesh&) = delete;

        Mesh(Mesh&& m);

        ~Mesh();
        
        // Returns state of the Read / Write Enabled checkbox when model was imported.
        bool isReadable() const
        {
            return m_isReadable;
        }

        uint32_t vertexCount()
        {
            if (m_vertexCount == 0)
            {
                m_vertexCount = static_cast<uint32_t>(m_indexBuffer.size() / 3);
            }
            return m_vertexCount;
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
        std::vector<Matrix4x4>& bindposes()
        {
            return m_bindposes;
        }

		// temp
        void ToBinary(std::ostream & os);
        static MeshPtr FromBinary(std::istream & is);

		static MeshPtr FromText(std::istream & is);

		static void Init();
		static MeshPtr builtinMesh(const PrimitiveType type);
		
    private:
        friend class FishEditor::Inspector;
        friend class FishEditor::ModelImporter;
        friend class MeshRenderer;
        friend class SkinnedMeshRenderer;
		//friend class Model;

		static std::map<PrimitiveType, MeshPtr> s_builtinMeshes;

//        enum class InternalShaderChannel
//        {
//            Vertex,
//            Normal,
//            //Color,
//            TexCoord0,
//            //TexCoord1,
//            //TexCoord2,
//            //TexCoord3,
//            Tangent
//        };

        bool m_isReadable = false;
        bool m_uploaded = false;
        uint32_t m_vertexCount = 0;
        uint32_t m_triangleCount = 0;

        Bounds m_bounds;

        std::vector<float>      m_positionBuffer;
        std::vector<float>      m_normalBuffer;
        std::vector<float>      m_uvBuffer;
        std::vector<float>      m_tangentBuffer;
        std::vector<uint32_t>   m_indexBuffer;
        std::vector<Int4>       m_boneIndexBuffer;
        std::vector<Vector4>    m_boneWeightBuffer;

        std::map<std::string, int> m_boneNameToIndex;

        std::vector<Matrix4x4>  m_bindposes;

		Meta(NonSerializable)
        std::vector<BoneWeight> m_boneWeights;

		Meta(NonSerializable)
        GLuint m_VAO			= 0;
		
		Meta(NonSerializable)
        GLuint m_indexVBO		= 0;
		
		Meta(NonSerializable)
        GLuint m_positionVBO	= 0;
		
		Meta(NonSerializable)
        GLuint m_normalVBO		= 0;
		
		Meta(NonSerializable)
        GLuint m_uvVBO			= 0;
		
		Meta(NonSerializable)
        GLuint m_tangentVBO		= 0;
		
		Meta(NonSerializable)
        GLuint m_boneIndexVBO	= 0;
		
		Meta(NonSerializable)
        GLuint m_boneWeightVBO	= 0;

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
