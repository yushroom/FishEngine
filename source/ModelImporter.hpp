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
        std::string     name;
        ModelNode*      parent;
        //uint32_t        numChildren;
        //ModelNode*      children;
        std::vector<PModelNode> children;
        std::vector<uint32_t> meshes;
    };
    
    class Model : public Object
    {
    public:
        Model() = default;
        
        std::shared_ptr<GameObject> CreateGameObject() const;
        
    private:
        friend class ModelImporter;
        //void AddNode(ModelNode::PModelNode& node) {
        //    m_modelNodes.push_back(node);
        //}

        void AddMesh(Mesh::PMesh& mesh) {
            m_meshes.push_back(mesh);
        }

        std::shared_ptr<GameObject>
        ResursivelyCreateGameObject(const ModelNode::PModelNode& node) const;
        
        std::vector<std::shared_ptr<Mesh>> m_meshes;
        //std::vector<ModelNode::PModelNode> m_modelNodes;
        ModelNode::PModelNode m_rootNode;
    };

    
    class ModelImporter : public AssetImporter
    {
    public:
        ModelImporter() = delete;
        
        static std::shared_ptr<Model>
        LoadFromFile(const std::string path,
                     int vertexUsage = VertexUsagePNUT,
                     MeshLoadFlags flags = 0);
    private:
        static ModelNode::PModelNode
        buildModelTree(const aiNode* assimp_node);
        
        
        static std::shared_ptr<Mesh>
        ParseMesh(const aiMesh* assimp_mesh,
                  int vertexUsage,
                  bool load_uv,
                  bool load_tangent);
    };
}

#endif /* ModelImporter_hpp */
