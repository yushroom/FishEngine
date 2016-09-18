#ifndef Model_hpp
#define Model_hpp

#include "Object.hpp"
#include "GLEnvironment.hpp"

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

    static PMesh CreateFromObjFile(const std::string path, int vertexUsage = VertexUsagePNUT, MeshLoadFlags flags = 0) {
        auto m = std::make_shared<Mesh>();
        m->FromObjFile(path, vertexUsage, flags);
        m_meshes.push_back(m);
        return m;
    }

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

    
private:
    friend class EditorGUI;
    std::vector<float>      m_positionBuffer;
    std::vector<float>      m_normalBuffer;
    std::vector<float>      m_uvBuffer;
    std::vector<float>      m_tangentBuffer;
    std::vector<uint32_t>   m_indexBuffer;
    GLuint m_VAO;
    GLuint m_indexVBO;
    GLuint m_positionVBO;
    GLuint m_normalVBO;
    GLuint m_uvVBO;
    GLuint m_tangentVBO;
    
    static std::vector<PMesh> m_meshes;
    
    void GenerateBuffer(int vertexUsage);
    void BindBuffer(int vertexUsage);
};

NAMESPACE_FISHENGINE_END

#endif /* Model_hpp */
