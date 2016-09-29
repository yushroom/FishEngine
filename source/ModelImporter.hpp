#ifndef ModelImporter_hpp
#define ModelImporter_hpp

#include "AssetImporter.hpp"
#include "Mesh.hpp"
#include "GameObject.hpp"
#include "Scene.hpp"
#include "MeshRenderer.hpp"
#include "Material.hpp"
#include "MeshFilter.hpp"

struct aiNode;
struct aiMesh;

namespace FishEngine {

    struct ModelNode
    {
        typedef std::shared_ptr<ModelNode> PModelNode;
        std::string             name;
        ModelNode*              parent;
        std::vector<PModelNode> children;
        std::vector<uint32_t>   meshesIndices;
        Matrix4x4               transform;
    };
    
    enum class BuiltinModelTyep {
        Cube, Sphere, Capsule, Cylinder, Plane, Quad, Cone,
    };
    
    class Model : public Object
    {
    public:
        Model() = default;
        
        typedef std::shared_ptr<Model> PModel;
        
        std::shared_ptr<GameObject> CreateGameObject() const;
        
        Mesh::PMesh mainMesh() const {
            return m_meshes[0];
        }
        
        static void Init();
        
        static Model::PModel builtinModel(const BuiltinModelTyep type);
        
    private:
        friend class ModelImporter;

        void AddMesh(Mesh::PMesh& mesh) {
            m_meshes.push_back(mesh);
        }

        std::shared_ptr<GameObject>
        ResursivelyCreateGameObject(const ModelNode::PModelNode& node) const;
        
        std::vector<std::shared_ptr<Mesh>> m_meshes;
        //std::vector<ModelNode::PModelNode> m_modelNodes;
        ModelNode::PModelNode m_rootNode;
        
        static std::map<BuiltinModelTyep, PModel> s_builtinModels;
    };

    
    class ModelImporter : public AssetImporter
    {
    public:
        ModelImporter() = default;
        
        void
        setFileScale(const float fileScale)
        {
            m_fileScale = fileScale;
        }
        
        std::shared_ptr<Model>
        LoadFromFile(const std::string path,
                     int vertexUsage = VertexUsagePNUT,
                     MeshLoadFlags flags = 0);
        
    private:
        
        float m_fileScale = 1.0f;
        
        ModelNode::PModelNode
        buildModelTree(const aiNode* assimp_node, const ModelNode* parentNode);
        
        
        std::shared_ptr<Mesh>
        ParseMesh(const aiMesh* assimp_mesh,
                  int vertexUsage,
                  bool load_uv,
                  bool load_tangent);
    };
}

#endif /* ModelImporter_hpp */
