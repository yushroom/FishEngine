#include "ModelImporter.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
//#include <boost/lexical_cast.hpp>

#include "Debug.hpp"
#include "Common.hpp"

namespace FishEngine {
    
    std::map<BuiltinModelTyep, Model::PModel> Model::s_builtinModels;
    
    void Model::Init() {
#if FISHENGINE_PLATFORM_WINDOWS
        const std::string root_dir = "../../assets/models/";
#else
        const std::string root_dir = "/Users/yushroom/program/graphics/FishEngine/assets/models/";
#endif
        ModelImporter importer;
        s_builtinModels[BuiltinModelTyep::Cube]     = importer.LoadFromFile(root_dir+"cube.obj");
        s_builtinModels[BuiltinModelTyep::Sphere]   = importer.LoadFromFile(root_dir+"sphere.obj");
        s_builtinModels[BuiltinModelTyep::Plane]    = importer.LoadFromFile(root_dir+"plane.obj");
        s_builtinModels[BuiltinModelTyep::Quad]     = importer.LoadFromFile(root_dir+"quad.obj");
        s_builtinModels[BuiltinModelTyep::Cone]     = importer.LoadFromFile(root_dir+"cone.obj");
    }
    
    Model::PModel
    Model::builtinModel(const BuiltinModelTyep type) {
        return s_builtinModels[type];
    }
    
    Matrix4x4
    ConvertMatrix(const aiMatrix4x4& m) {
        Matrix4x4 result;
        memcpy(result.m, &m.a1, 16*sizeof(float));
        //result *= Matrix4x4(1, 0, 0, 0,   0, 1, 0, 0,   0, 0, -1, 0,   0, 0, 0, 1);
        return result;
    }

    ModelNode::PModelNode 
    ModelImporter::buildModelTree(const aiNode* assimp_node, Model::PModel& model)
    {
        //Debug::Log("NODE: %s mesh count: %d ", assimp_node->mName.C_Str(), assimp_node->mNumMeshes);
        //for (int i = 0; i < assimp_node->mNumChildren; ++i) {
        //    Debug::Log("    CHILD: %s", assimp_node->mChildren[i]->mName.C_Str());
        //}
        auto node = std::make_shared<ModelNode>();
        node->name = assimp_node->mName.C_Str();
        node->transform = ConvertMatrix(assimp_node->mTransformation);
        node->isBone = false;
        //model->m_bones.push_back(node);
        //node->index = model->m_bones.size();
        int index = model->m_avatar->m_boneToIndex.size();
        model->m_avatar->m_boneToIndex[node->name] = index;
        m_nodes[node->name] = node;
        
        for (uint32_t i = 0; i < assimp_node->mNumMeshes; ++i) {
            node->meshesIndices.push_back(assimp_node->mMeshes[i]);
        }

        for (uint32_t i = 0; i < assimp_node->mNumChildren; ++i) {
            //Debug::Log("    CHILD: %s", assimp_node->mName.C_Str());
            auto child = buildModelTree(assimp_node->mChildren[i], model);
            node->children.push_back(child);
            child->parent = node.get();
        }
        return node;
    }

    Mesh::PMesh 
    ModelImporter::ParseMesh(
        const aiMesh* assimp_mesh, 
        int vertexUsage, 
        bool load_uv, 
        bool load_tangent)
    {
        auto mesh = std::make_shared<Mesh>();
        bool has_uv = assimp_mesh->HasTextureCoords(0);

        auto n_vertices = assimp_mesh->mNumVertices;
        auto n_triangles = assimp_mesh->mNumVertices;
        auto n_bones = assimp_mesh->mNumBones;
        mesh->m_positionBuffer.reserve(n_vertices * 3);
        mesh->m_normalBuffer.reserve(n_vertices * 3);
        mesh->m_uvBuffer.reserve(n_vertices * 2);
        mesh->m_indexBuffer.reserve(n_triangles * 3);
        mesh->m_tangentBuffer.reserve(n_triangles * 3);


        // Vertex
        for (unsigned int j = 0; j < assimp_mesh->mNumVertices; ++j) {
            auto& v = assimp_mesh->mVertices[j];
            mesh->m_positionBuffer.push_back(v.x * m_fileScale);
            mesh->m_positionBuffer.push_back(v.y * m_fileScale);
            mesh->m_positionBuffer.push_back(v.z * m_fileScale);

            auto& n = assimp_mesh->mNormals[j];
            mesh->m_normalBuffer.push_back(n.x);
            mesh->m_normalBuffer.push_back(n.y);
            mesh->m_normalBuffer.push_back(n.z);

            if (has_uv) {
                auto& uv = assimp_mesh->mTextureCoords[0][j];
                mesh->m_uvBuffer.push_back(uv.x);
                mesh->m_uvBuffer.push_back(uv.y);
            }

            if (load_tangent) {
                auto& t = assimp_mesh->mTangents[j];
                mesh->m_tangentBuffer.push_back(t.x);
                mesh->m_tangentBuffer.push_back(t.y);
                mesh->m_tangentBuffer.push_back(t.z);
            }
        }

        // face index
        for (unsigned int j = 0; j < assimp_mesh->mNumFaces; j++) {
            auto& face = assimp_mesh->mFaces[j];
            assert(face.mNumIndices == 3);
            for (int fi = 0; fi < 3; ++fi) {
                mesh->m_indexBuffer.push_back(face.mIndices[fi]);
            }
        }

        mesh->m_skinned = assimp_mesh->mNumBones > 0;
        
        if (mesh->m_skinned)
        {
            //mesh->m_bones.resize(n_bones);
            mesh->m_boneWeights.resize(n_vertices);
            mesh->m_boneIndexBuffer.resize(n_vertices);
            mesh->m_boneWeightBuffer.resize(n_vertices);
            Debug::Log("Bone count: %d", assimp_mesh->mNumBones);
            for (uint32_t j = 0; j < assimp_mesh->mNumBones; ++j) {
                auto& bone = assimp_mesh->mBones[j];
                std::string boneName(bone->mName.C_Str());
                Debug::Log("    bone name: %s", boneName.c_str());
                auto it = m_nodes.find(boneName);
                assert(it != m_nodes.end());
                it->second->isBone = true;
                uint32_t boneIndex = it->second->index;
                
                for (uint32_t k = 0; k < bone->mNumWeights; ++k) {
                    uint32_t vextexID = bone->mWeights[k].mVertexId;
                    float weight = bone->mWeights[k].mWeight;
                    mesh->m_boneWeights[vextexID].AddBoneData(boneIndex, weight);
                }
            }
            
            for (uint32_t i = 0; i < n_vertices; ++i) {
                auto& b = mesh->m_boneWeights[i];
                mesh->m_boneIndexBuffer[i] = Int4{b.boneIndex[0], b.boneIndex[1], b.boneIndex[2], b.boneIndex[3]};
                mesh->m_boneWeightBuffer[i] = Vector4{b.weight[0], b.weight[1], b.weight[2], b.weight[3]};
            }
        }

        mesh->GenerateBuffer(vertexUsage);
        mesh->BindBuffer(vertexUsage);
        //mesh->m_name = std::string(assimp_mesh->mName.C_Str());
        //if (mesh->m_name.size() == 0) {
        //    mesh->m_name = "Mesh" + boost::lexical_cast<std::string>(meshIndex);
        //}
        return mesh;
    }
    

    std::shared_ptr<Model>
    ModelImporter::LoadFromFile(
        const std::string path,
        int vertexUsage,
        MeshLoadFlags flags)
    {
        //AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS
        Assimp::Importer importer;
        unsigned int load_option = aiProcess_Triangulate;
        load_option |= aiProcess_CalcTangentSpace;
        load_option |= aiProcess_LimitBoneWeights;
        //load_option |= aiProcess_FixInfacingNormals;
        //load_option |= aiProcess_OptimizeMeshes;
        //load_option |= aiProcess_SortByPType;
        //load_option |= aiProcess_JoinIdenticalVertices;
        //load_option |= aiProcess_OptimizeGraph;
        load_option |= aiProcess_ConvertToLeftHanded;
        load_option |= aiProcess_FlipUVs;
        importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);
        importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_READ_MATERIALS, false);
        importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_READ_TEXTURES, false);
        importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_READ_LIGHTS, false);
        importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_READ_CAMERAS, false);
        if (flags & MeshLoadFlag_RegenerateNormal) {
            importer.SetPropertyInteger(AI_CONFIG_PP_RVC_FLAGS, aiComponent_NORMALS);
            load_option |= aiProcess_GenSmoothNormals |aiProcess_RemoveComponent;
        } else {
            load_option |= aiProcess_GenNormals;
        }
        bool load_tangent = (vertexUsage & VertexUsageTangent) != 0;
        if (load_tangent)
            load_option |= aiProcess_CalcTangentSpace;
        const aiScene* scene = importer.ReadFile(path.c_str(), load_option);
        if (!scene) {
            Debug::LogError(importer.GetErrorString());
            Debug::LogError("Can not open file %s", path.c_str());
            abort();
        }

        bool load_uv = (vertexUsage & VertexUsageUV) != 0;
        
        
        auto model = std::make_shared<Model>();
        model->m_name = split(path, "/").back();
        
        bool loadAnimation = scene->HasAnimations();
        if (loadAnimation)
            Debug::Log("%s has animation", path.c_str());

        model->m_rootNode = buildModelTree(scene->mRootNode, model);
        
        for (unsigned int meshIndex = 0; meshIndex < scene->mNumMeshes; meshIndex++)
        {
            auto mesh = ParseMesh(scene->mMeshes[meshIndex], vertexUsage, load_uv, load_tangent);
            model->AddMesh(mesh);
        }

        auto ConvertVector3 = [](const aiVector3D& avec3) {
            return Vector3(avec3.x, avec3.y, avec3.z);
        };

        auto ConvertQuaternion = [](const aiQuaternion& aquat) {
            return Quaternion(aquat.x, aquat.y, aquat.z, aquat.w);
        };

        for (uint32_t animIndex = 0; animIndex < scene->mNumAnimations; ++animIndex) {
            model->m_animations.push_back(std::make_shared<Animation>());
            auto& animation = model->m_animations.back();
            auto a = scene->mAnimations[animIndex];
            animation->name = a->mName.C_Str();
            animation->duration = (float)a->mDuration;
            animation->ticksPerSecond = (float)a->mTicksPerSecond;
            //animation->channels.resize(a->mNumChannels);

            for (uint32_t i = 0; i < a->mNumChannels; ++i) {
                auto an = a->mChannels[i];
                std::string name = an->mNodeName.C_Str();
                //auto& n = animation->channels[i];
                auto pos = name.find("_$AssimpFbx$");
                //Debug::Log("old name: %s", name.c_str());
                if (pos != std::string::npos) {
                    name = name.substr(0, pos);
                    //Debug::Log("new name: %s", name.c_str());
                }
                auto it = animation->channels.find(name);
                if (it == animation->channels.end()) {
                    animation->channels[name] = AnimationNode();
                }
                auto& n = animation->channels[name];

                n.name = name;
                if (an->mNumPositionKeys > 1) {
                    //n.positionKeys.resize(an->mNumPositionKeys);
                    for (uint32_t j = 0; j < an->mNumPositionKeys; ++j) {
                        n.positionKeys.emplace_back(Vector3Key{ (float)an->mPositionKeys[j].mTime, ConvertVector3(an->mPositionKeys[j].mValue) });
                    }
                }
                //n.rotationKeys.resize(an->mNumRotationKeys);
                if (an->mNumRotationKeys > 1) {
                    for (uint32_t j = 0; j < an->mNumRotationKeys; ++j) {
                        n.rotationKeys.emplace_back(QuaternionKey{ (float)an->mRotationKeys[j].mTime, ConvertQuaternion(an->mRotationKeys[j].mValue) });
                    }
                }
                //n.scalingKeys.resize(an->mNumScalingKeys);
                if (an->mNumScalingKeys > 1) {
                    for (uint32_t j = 0; j < an->mNumScalingKeys; ++j) {
                        n.scalingKeys.emplace_back(Vector3Key{ (float)an->mScalingKeys[j].mTime, ConvertVector3(an->mScalingKeys[j].mValue) });
                    }
                }
            }
            Debug::Log("animation name: %s", a->mName.C_Str());
        }

        return model;
    }

    std::shared_ptr<GameObject> 
    Model::CreateGameObject() const
    {
        auto root = ResursivelyCreateGameObject(m_rootNode);
        if (m_animations.size() > 0) {
            auto animator = std::make_shared<Animator>();
            animator->m_animation = m_animations.front();
            root->AddComponent(animator);
        }
        return root;
    }

    std::shared_ptr<GameObject> 
    Model::ResursivelyCreateGameObject(const ModelNode::PModelNode & node) const
    {
        auto go = Scene::CreateGameObject(node->name);
        go->transform()->setLocalToWorldMatrix(node->transform);

        if (node->meshesIndices.size() == 1) {
            auto material = Material::defaultMaterial();
            auto meshRenderer = std::make_shared<MeshRenderer>(material);
            auto meshFilter = std::make_shared<MeshFilter>(m_meshes[node->meshesIndices.front()]);
            go->AddComponent(meshRenderer);
            go->AddComponent(meshFilter);
        } else if (node->meshesIndices.size() > 1) {
            for (auto& idx : node->meshesIndices) {
                auto& m = m_meshes[idx];
                auto child = Scene::CreateGameObject(m->name());
                child->transform()->SetParent(go->transform());
                auto material = Material::defaultMaterial();
                auto meshRenderer = std::make_shared<MeshRenderer>(material);
                auto meshFilter = std::make_shared<MeshFilter>(m_meshes[node->meshesIndices.front()]);
                child->AddComponent(meshRenderer);
                child->AddComponent(meshFilter);
            }
        } else if (node->isBone) {
            auto material = Material::defaultMaterial();
            auto meshRenderer = std::make_shared<MeshRenderer>(material);
            auto meshFilter = std::make_shared<MeshFilter>(Model::builtinModel(BuiltinModelTyep::Cube)->m_meshes.front());
            go->AddComponent(meshRenderer);
            go->AddComponent(meshFilter);
        }
        
        for (auto& c : node->children) {
            auto child = ResursivelyCreateGameObject(c);
            child->transform()->SetParent(go->transform());
        }
        
        return go;
    }
}
