#ifndef ModelImporter_hpp
#define ModelImporter_hpp

#include "AssetImporter.hpp"
#include "Mesh.hpp"
#include "GameObject.hpp"
#include "Scene.hpp"
#include "MeshRenderer.hpp"
#include "Material.hpp"
#include "MeshFilter.hpp"

namespace FishEngine {
    
    class Model : public Object
    {
    public:
        Model() = default;
        
        std::shared_ptr<GameObject> CreateGameObject() const {
            auto go = Scene::CreateGameObject(m_name);
            
            for (auto& m : m_meshes) {
                auto child = Scene::CreateGameObject(m->name());
                child->transform()->SetParent(go->transform());
                auto material = Material::builtinMaterial("PBR");
                material->SetVector3("albedo", Vector3(1, 1, 1));
                auto meshRenderer = std::make_shared<MeshRenderer>(material);
                auto meshFilter = std::make_shared<MeshFilter>(m);
                child->AddComponent(meshRenderer);
                child->AddComponent(meshFilter);
            }
            
            return go;
        }
        
    private:
        friend class ModelImporter;
        void AddMesh(std::shared_ptr<Mesh>& mesh) {
            m_meshes.push_back(mesh);
        }
        
        std::vector<std::shared_ptr<Mesh>> m_meshes;
    };

    
    class ModelImporter : public AssetImporter
    {
    public:
        ModelImporter() = delete;
        
        static std::shared_ptr<Model>
        LoadFromFile(const std::string path,
                     int vertexUsage = VertexUsagePNUT,
                     MeshLoadFlags flags = 0);
    };
}

#endif /* ModelImporter_hpp */
