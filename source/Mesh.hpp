#ifndef Model_hpp
#define Model_hpp

#include "Object.hpp"
#include "GLEnvironment.hpp"
#include "Matrix4x4.hpp"

NAMESPACE_FISHENGINE_BEGIN

enum VertexUsage {
    VertexUsagePosition = 1<<0,
    VertexUsageNormal   = 1<<1,
    VertexUsageUV       = 1<<2,
    VertexUsageTangent  = 1<<3,
    VertexUsagePN       = VertexUsagePosition | VertexUsageNormal,
    VertexUsagePNU      = VertexUsagePN | VertexUsageUV,
    VertexUsagePNUT     = VertexUsagePNU | VertexUsageTangent
};

typedef int VertexUsages;

enum MeslLoadFlag {
    MeshLoadFlag_RegenerateNormal = 1<<0,
};

typedef int MeshLoadFlags;

struct Bone
{
    //uint32_t index;
    uint32_t parentIndex;
    Matrix4x4 boneOffse;
    Matrix4x4 finalTransformation;
    std::vector<uint32_t> children;
    std::string name;
};

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
    
    BoneWeight() {
        for (int i = 0; i < MaxBoneForEachVertex; ++i) {
            boneIndex[i] = 0;
            weight[i] = 0.f;
        }
    }
    
    void AddBoneData(uint32_t boneIndex, float weight) {
        for (int i = 0; i < MaxBoneForEachVertex; ++i) {
            if (this->weight[i] == 0.0f) {
                this->boneIndex[i] = boneIndex;
                this->weight[i] = weight;
                return;
            }
        }
        abort();
    }
};

class Mesh : public Object
{
public:
    Mesh() = default;
    Mesh(std::vector<float> position_buffer, std::vector<uint32_t> index_buffer);
    Mesh(const int n_vertex, const int n_face, float* positions, uint32_t* indices);
    Mesh(const int n_vertex, const int n_face, float* positions, float* normals, uint32_t* indices);
    Mesh(const std::string& objModelPath, int vertexUsage);
    
    Mesh(const Mesh&) = delete;
    void operator=(const Mesh&) = delete;

    Mesh(Mesh&& m);
    
    ~Mesh();
    
    typedef std::shared_ptr<Mesh> PMesh;

    static PMesh CreateFromObjFile(const std::string path, int vertexUsage = VertexUsagePNUT, MeshLoadFlags flags = 0);

    void FromObjFile(const std::string path, int vertexUsage = VertexUsagePNUT, MeshLoadFlags flags = 0);

    void SetVertexUsage(int vertexUsage) {
        BindBuffer(vertexUsage);
    }
    
    void Render();
    //void renderPatch(const Shader& shader);
    
    //static Model& getQuad();
    //static Model& getBox();
    //static Model& getSphere();
    //static Model& getIcosahedron();

    static void Init();
    static PMesh builtinMesh(const std::string& name);
    
private:
    friend class FishEditor::EditorGUI;
    friend class ModelImporter;
    std::vector<float>      m_positionBuffer;
    std::vector<float>      m_normalBuffer;
    std::vector<float>      m_uvBuffer;
    std::vector<float>      m_tangentBuffer;
    std::vector<uint32_t>   m_indexBuffer;
    std::vector<Bone>       m_bones;
    std::vector<BoneWeight> m_boneWeights;
    GLuint m_VAO;
    GLuint m_indexVBO;
    GLuint m_positionVBO;
    GLuint m_normalVBO;
    GLuint m_uvVBO;
    GLuint m_tangentVBO;
    
    static std::map<std::string, PMesh> m_meshes;
    
    void GenerateBuffer(int vertexUsage);
    void BindBuffer(int vertexUsage);
};

NAMESPACE_FISHENGINE_END

#endif /* Model_hpp */
