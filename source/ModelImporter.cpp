#include "ModelImporter.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <boost/lexical_cast.hpp>

#include "Debug.hpp"
#include "Common.hpp"

namespace FishEngine {
    
    Matrix4x4 ConvertMatrix(aiMatrix4x4& m) {
        Matrix4x4 result;
        memcpy(result.m, &m.a1, 16*sizeof(float));
        return result;
    }
    
    void CreateBoneTree(aiNode* pNode
                        //,const std::map<std::string,uint32_t>& boneMappin
                        //,std::vector<Bone>& bones
                        )
    {
        std::string name = pNode->mName.C_Str();
        for (int i = 0; i < pNode->mNumChildren; ++i) {
            Debug::Log("parent: %s, child: %s", pNode->mName.C_Str(), pNode->mChildren[i]->mName.C_Str());
            CreateBoneTree(pNode->mChildren[i]);
        }
    }
    
    std::shared_ptr<Model>
    ModelImporter::LoadFromFile(const std::string path,
                                   int vertexUsage,
                                   MeshLoadFlags flags)
    {
        Assimp::Importer importer;
        unsigned int load_option =
        aiProcess_Triangulate
        //| aiProcess_SortByPType
        | aiProcess_CalcTangentSpace
        //| aiProcess_JoinIdenticalVertices
        //| aiProcess_FixInfacingNormals
        //| aiProcess_OptimizeGraph
        //| aiProcess_OptimizeMeshes
        ;
        load_option |= aiProcess_ConvertToLeftHanded;
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
            Debug::LogError("Can not open file %s", path.c_str());
            abort();
        }
        
        auto model = std::make_shared<Model>();
        model->m_name = split(path, "/").back();
        
        bool loadAnimation = scene->HasAnimations();
        
        for (unsigned int meshIndex = 0; meshIndex < scene->mNumMeshes; meshIndex++)
        {
            auto mesh = std::make_shared<Mesh>();
            aiMesh* assimp_mesh = scene->mMeshes[meshIndex];
            bool has_uv = assimp_mesh->HasTextureCoords(0);
            //if (!has_uv)
            //	printf("mesh[%d] do not have uv!\n", i);
            assert(!((!has_uv) && (vertexUsage & VertexUsageUV)));
            bool load_uv = (vertexUsage & VertexUsageUV) != 0;
            
            auto n_vertices = assimp_mesh->mNumVertices;
            auto n_triangles = assimp_mesh->mNumVertices;
            auto n_bones = assimp_mesh->mNumBones;
            mesh->m_positionBuffer.reserve(n_vertices * 3);
            mesh->m_normalBuffer.reserve(n_vertices * 3);
            mesh->m_uvBuffer.reserve(n_vertices * 2);
            mesh->m_indexBuffer.reserve(n_triangles * 3);
            mesh->m_tangentBuffer.reserve(n_triangles * 3);
            mesh->m_bones.resize(n_bones);
            mesh->m_boneWeights.resize(n_vertices);
            
            // Vertex
            for (unsigned int j = 0; j < assimp_mesh->mNumVertices; ++j) {
                auto& v = assimp_mesh->mVertices[j];
                mesh->m_positionBuffer.push_back(v.x);
                mesh->m_positionBuffer.push_back(v.y);
                mesh->m_positionBuffer.push_back(v.z);
                
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
            
            std::map<std::string, uint32_t> boneMapping;
            uint32_t numBones = 0;

            for (int j = 0; j < assimp_mesh->mNumBones; ++j) {
                uint32_t boneIndex = 0;
                auto& bone = assimp_mesh->mBones[j];
                std::string boneName(bone->mName.C_Str());
                
                if (boneMapping.find(boneName) == boneMapping.end()) {
                    boneIndex = numBones;
                    boneMapping[boneName] = boneIndex;
                    numBones++;
                    //mesh->m_bones.push_back({ConvertMatrix(bone->mOffsetMatrix), Matrix4x4::identity});
                    mesh->m_bones[boneIndex].boneOffse = ConvertMatrix(bone->mOffsetMatrix);
                } else {
                    boneIndex = boneMapping[boneName];
                }
                
                for (int k = 0; k < bone->mNumWeights; ++k) {
                    uint vextexID = bone->mWeights[k].mVertexId;
                    float weight = bone->mWeights[k].mWeight;
                    mesh->m_boneWeights[vextexID].AddBoneData(boneIndex, weight);
                }
            }
            
            mesh->GenerateBuffer(vertexUsage);
            mesh->BindBuffer(vertexUsage);
            mesh->m_name = std::string(assimp_mesh->mName.C_Str());
            if (mesh->m_name.size() == 0) {
                mesh->m_name = "Mesh" + boost::lexical_cast<std::string>(meshIndex);
            }
            model->AddMesh(mesh);
        }
        
        CreateBoneTree(scene->mRootNode);
        
        return model;
    }
}
