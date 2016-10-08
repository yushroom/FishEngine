#ifndef ModelImporter_hpp
#define ModelImporter_hpp

#include "AssetImporter.hpp"
#include "Mesh.hpp"
#include "GameObject.hpp"
#include "Scene.hpp"
#include "MeshRenderer.hpp"
#include "Material.hpp"
#include "MeshFilter.hpp"
#include "Animator.hpp"

struct aiNode;
struct aiMesh;

//namespace fbxsdk
//{
//    class FbxNode;
//    class FbxMesh;
//}


namespace FishEngine {

    struct ModelNode
    {
        typedef std::shared_ptr<ModelNode> PModelNode;
        uint32_t                index;
        std::string             name;
        ModelNode*              parent;
        std::vector<PModelNode> children;
        std::vector<uint32_t>   meshesIndices;
        Matrix4x4               transform;
        bool                    isBone;
    };
    
    enum class BuiltinModelTyep {
        Cube, Sphere, Capsule, Cylinder, Plane, Quad, Cone,
    };
    
    class Model : public Object
    {
    public:
        Model() : m_avatar(std::make_shared<Avatar>()) {}
        
        typedef std::shared_ptr<Model> PModel;
        
        std::shared_ptr<GameObject> CreateGameObject() const;
        
        Mesh::PMesh mainMesh() const {
            return m_meshes[0];
        }
        
        Animation::PAnimation mainAnimation() const {
            if (m_animations.empty())
                return nullptr;
            return m_animations.front();
        }

        std::shared_ptr<Avatar> avatar() const {
            return m_avatar;
        }
        
        static void Init();
        
        static Model::PModel builtinModel(const BuiltinModelTyep type);
        
    private:
        friend class ModelImporter;

        void AddMesh(Mesh::PMesh& mesh);

        std::shared_ptr<GameObject>
        ResursivelyCreateGameObject(const ModelNode::PModelNode& node) const;
        
        std::vector<std::shared_ptr<Mesh>> m_meshes;
        std::vector<std::shared_ptr<Animation>> m_animations;
        //std::vector<ModelNode::PModelNode> m_modelNodes;
        ModelNode::PModelNode m_rootNode;
        //std::vector<ModelNode::PModelNode> m_bones;
        //std::map<std::string, int> m_boneToIndex;
        std::shared_ptr<Avatar> m_avatar;
        
        static std::map<BuiltinModelTyep, PModel> s_builtinModels;
    };
    
    class ModelImporter : public AssetImporter
    {
    public:
        ModelImporter() = default;
        
        void
        setFileScale(
            const float fileScale)
        {
            m_fileScale = fileScale;
        }
        
        std::shared_ptr<Model>
        LoadFromFile(
            const std::string& path,
            int                vertexUsage = VertexUsagePNUT,
            MeshLoadFlags      flags = 0);

        std::shared_ptr<Model>
        LoadFBX(
            const std::string&  path,
            int                 vertexUsage = VertexUsagePNUT,
            MeshLoadFlags       flags = 0);

    private:
        
        float m_fileScale = 1.0f;

        std::map<std::string, ModelNode::PModelNode> m_nodes; // temp
        
        ModelNode::PModelNode
        buildModelTree(
            const aiNode*   assimp_node);
        
//        ModelNode::PModelNode
//        buildModelTree(
//            fbxsdk::FbxNode* pNode);
//
//        Mesh::PMesh
//        ParseMesh(
//            fbxsdk::FbxMesh* pMesh);

        std::shared_ptr<Mesh>
        ParseMesh(
            const aiMesh*   assimp_mesh,
            int             vertexUsage,
            bool            load_uv,
            bool            load_tangent);

        std::shared_ptr<Model> m_model;
    };
}

#endif /* ModelImporter_hpp */
