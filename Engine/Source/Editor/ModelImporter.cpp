#include "ModelImporter.hpp"

#include <set>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include <Debug.hpp>
#include <GameObject.hpp>
#include <Common.hpp>
#include <MeshRenderer.hpp>
#include <SkinnedMeshRenderer.hpp>

#define REMOVE_FBX_PIVOT
#define DEBUG_ANIMATION

constexpr int MAX_BONE_SIZE = 100;

static const std::set<std::string> DummyNodeNames = { "Translation", "PreRotation", "Rotation", "PostRotation", "Scaling" };

using namespace FishEngine;

namespace FishEditor
{
    void Model::AddMesh(MeshPtr& mesh)
    {
        m_meshes.push_back(mesh);
    }

    Matrix4x4
    ConvertMatrix(
        const aiMatrix4x4& m)
    {
        Matrix4x4 result;
        memcpy(result.m, &m.a1, 16*sizeof(float));
        //result *= Matrix4x4(1, 0, 0, 0,   0, 1, 0, 0,   0, 0, -1, 0,   0, 0, 0, 1);
        return result;
    }


    ModelNodePtr ModelImporter::
    buildModelTree(
        const aiNode*   assimp_node)
    {
        std::string name = assimp_node->mName.C_Str();
        Matrix4x4 transform = ConvertMatrix(assimp_node->mTransformation);
        transform.m[0][3] *= m_fileScale;
        transform.m[1][3] *= m_fileScale;
        transform.m[2][3] *= m_fileScale;

#ifdef REMOVE_FBX_PIVOT
        // remove dummyNode
        auto pos = name.find("_$AssimpFbx$");
        if (pos != std::string::npos)
        {
            auto trueName = name.substr(0, pos);
            auto typeName = name.substr(pos + 13);
            assert(DummyNodeNames.find(typeName) != DummyNodeNames.end());
            auto it = m_nodeTransformations.find(trueName);
            if (it != m_nodeTransformations.end()) {
                it->second[typeName] = transform;
            } else {
                m_nodeTransformations[trueName] = { { typeName, transform } };
            }

            assert(assimp_node->mNumChildren == 1);
            auto child = assimp_node->mChildren[0];
            assert(boost::starts_with(child->mName.C_Str(), trueName));
            auto c = buildModelTree(child);
            c->transform = transform * c->transform;
            return c;
        }
#endif

        auto node = std::make_shared<ModelNode>();
        node->name = name;
        //Debug::LogWarning("Node: %s", node->name.c_str());
        node->transform = transform;
        node->isBone = false;
        uint32_t index = static_cast<uint32_t>(m_model->m_avatar->m_boneToIndex.size());
        m_model->m_avatar->m_boneToIndex[node->name] = index;
        m_model->m_avatar->m_indexToBone[index] = node->name;
        node->index = index;
        
        for (uint32_t i = 0; i < assimp_node->mNumMeshes; ++i)
        {
            node->meshesIndices.push_back(assimp_node->mMeshes[i]);
        }

        for (uint32_t i = 0; i < assimp_node->mNumChildren; ++i)
        {
            auto child = buildModelTree(assimp_node->mChildren[i]);
            node->children.push_back(child);
            child->parent = node.get();
        }
        return node;
    }


    MeshPtr ModelImporter::
    ParseMesh(
        const aiMesh*   assimp_mesh,
        bool            load_uv,
        bool            load_tangent)
    {
        auto mesh = std::make_shared<Mesh>();
        mesh->m_name = assimp_mesh->mName.C_Str();
        bool has_uv = assimp_mesh->HasTextureCoords(0);

        auto n_vertices = assimp_mesh->mNumVertices;
        auto n_triangles = assimp_mesh->mNumFaces;
        auto n_bones = assimp_mesh->mNumBones;
        mesh->m_vertexCount = n_vertices;
        mesh->m_triangleCount = n_triangles;
        mesh->m_positionBuffer.reserve(n_vertices * 3);
        mesh->m_normalBuffer.reserve(n_vertices * 3);
        mesh->m_uvBuffer.reserve(n_vertices * 2);
        mesh->m_indexBuffer.reserve(n_triangles * 3);
        mesh->m_tangentBuffer.reserve(n_vertices * 3);

        Vector3 vmin(Mathf::Infinity, Mathf::Infinity, Mathf::Infinity);
        Vector3 vmax(Mathf::NegativeInfinity, Mathf::NegativeInfinity, Mathf::NegativeInfinity);

        // Vertex
        for (unsigned int j = 0; j < assimp_mesh->mNumVertices; ++j)
        {
            auto& v = assimp_mesh->mVertices[j];
            float vx = v.x * m_fileScale;
            float vy = v.y * m_fileScale;
            float vz = v.z * m_fileScale;
            if (vmin.x > vx) vmin.x = vx;
            if (vmin.y > vy) vmin.y = vy;
            if (vmin.z > vz) vmin.z = vz;
            if (vmax.x < vx) vmax.x = vx;
            if (vmax.y < vy) vmax.y = vy;
            if (vmax.z < vz) vmax.z = vz;
            mesh->m_positionBuffer.push_back(vx);
            mesh->m_positionBuffer.push_back(vy);
            mesh->m_positionBuffer.push_back(vz);

            auto& n = assimp_mesh->mNormals[j];
            mesh->m_normalBuffer.push_back(n.x);
            mesh->m_normalBuffer.push_back(n.y);
            mesh->m_normalBuffer.push_back(n.z);

            if (has_uv)
            {
                auto& uv = assimp_mesh->mTextureCoords[0][j];
                mesh->m_uvBuffer.push_back(uv.x);
                mesh->m_uvBuffer.push_back(uv.y);
            }

            if (load_tangent)
            {
                auto& t = assimp_mesh->mTangents[j];
                mesh->m_tangentBuffer.push_back(t.x);
                mesh->m_tangentBuffer.push_back(t.y);
                mesh->m_tangentBuffer.push_back(t.z);
            }
        }

        mesh->m_bounds.SetMinMax(vmin, vmax);

        // face index
        for (unsigned int j = 0; j < assimp_mesh->mNumFaces; j++)
        {
            auto& face = assimp_mesh->mFaces[j];
            assert(face.mNumIndices == 3);
            for (int fi = 0; fi < 3; ++fi)
            {
                mesh->m_indexBuffer.push_back(face.mIndices[fi]);
            }
        }

        mesh->m_skinned = assimp_mesh->mNumBones > 0;

        if (mesh->m_skinned)
        {
            assert(assimp_mesh->mNumBones <= MAX_BONE_SIZE);
            //std::vector<BoneWeight> boneWeights;
            //mesh->m_bones.resize(n_bones);
            mesh->m_boneWeights.resize(n_vertices);
            mesh->m_boneIndexBuffer.resize(n_vertices);
            mesh->m_boneWeightBuffer.resize(n_vertices);
            mesh->bindposes().resize(assimp_mesh->mNumBones);
            //Debug::Log("Bone count: %d", assimp_mesh->mNumBones);
            for (uint32_t boneIndex = 0; boneIndex < assimp_mesh->mNumBones; ++boneIndex)
            {
                auto& bone = assimp_mesh->mBones[boneIndex];
                std::string boneName(bone->mName.C_Str());
                //Debug::Log("    bone name: %s", boneName.c_str());
                mesh->m_boneNameToIndex[boneName] = boneIndex;

                auto offsetMat = ConvertMatrix(bone->mOffsetMatrix);
                offsetMat.m[0][3] *= m_fileScale;
                offsetMat.m[1][3] *= m_fileScale;
                offsetMat.m[2][3] *= m_fileScale;
                mesh->bindposes()[boneIndex] = offsetMat;
                for (uint32_t k = 0; k < bone->mNumWeights; ++k)
                {
                    uint32_t vextexID = bone->mWeights[k].mVertexId;
                    float weight = bone->mWeights[k].mWeight;
                    mesh->m_boneWeights[vextexID].AddBoneData(boneIndex, weight);
                }
            }
            
            for (uint32_t i = 0; i < n_vertices; ++i)
            {
                auto& b = mesh->m_boneWeights[i];
                mesh->m_boneIndexBuffer[i] = Int4{b.boneIndex[0], b.boneIndex[1], b.boneIndex[2], b.boneIndex[3]};
                mesh->m_boneWeightBuffer[i] = Vector4{b.weight[0], b.weight[1], b.weight[2], b.weight[3]};
            }
        }

        return mesh;
    }


    Vector3 ConvertVector3(const aiVector3D& avec3)
    {
        return Vector3(avec3.x, avec3.y, avec3.z);
    };


    Quaternion ConvertQuaternion(const aiQuaternion& aquat)
    {
        return Quaternion(aquat.x, aquat.y, aquat.z, aquat.w);
    };


    AnimationPtr
    ParseAnimation(
        const aiAnimation* assimp_animation,
        const float fileScale)
    {
        auto animation = std::make_shared<Animation>();
        auto& a = assimp_animation;
        animation->name = a->mName.C_Str();
#ifdef DEBUG_ANIMATION
        // remove dummyNode
        auto pos = animation->name.find("_$AssimpFbx$");
        if (pos != std::string::npos)
        {
            //auto trueName = animation->name.substr(0, pos);
            auto typeName = animation->name.substr(pos + 13);
            assert(typeName == "Translation" || typeName == "Rotation" || typeName == "Scaling");
        }
#endif
        animation->duration = (float)a->mDuration;
        animation->ticksPerSecond = (float)a->mTicksPerSecond;
        if (animation->ticksPerSecond <= 0.f) {
            animation->ticksPerSecond = 25.f;
        }

        for (uint32_t i = 0; i < a->mNumChannels; ++i) {
            auto an = a->mChannels[i];
            std::string name = an->mNodeName.C_Str();

            auto it = animation->channels.find(name);
            if (it == animation->channels.end()) {
                animation->channels[name] = AnimationNode();
            }
            auto& n = animation->channels[name];

            n.name = name;
            for (uint32_t j = 0; j < an->mNumPositionKeys; ++j) {
                n.positionKeys.emplace_back(Vector3Key{ (float)an->mPositionKeys[j].mTime, ConvertVector3(an->mPositionKeys[j].mValue)*fileScale });
            }
            for (uint32_t j = 0; j < an->mNumRotationKeys; ++j) {
                n.rotationKeys.emplace_back(QuaternionKey{ (float)an->mRotationKeys[j].mTime, ConvertQuaternion(an->mRotationKeys[j].mValue) });
            }
            for (uint32_t j = 0; j < an->mNumScalingKeys; ++j) {
                n.scalingKeys.emplace_back(Vector3Key{ (float)an->mScalingKeys[j].mTime, ConvertVector3(an->mScalingKeys[j].mValue) });
            }
        }
        Debug::Log("animation name: %s", a->mName.C_Str());
        return animation;
    }


    ModelPtr ModelImporter::
    LoadFromFile(
        const Path&  path)
    {
        auto fileExtention = path.extension();
        Assimp::Importer importer;
        unsigned int load_option = aiProcess_Triangulate;
#if 1
        load_option |= aiProcess_LimitBoneWeights;
        load_option |= aiProcess_JoinIdenticalVertices;
        load_option |= aiProcess_ValidateDataStructure;

        load_option |= aiProcess_ImproveCacheLocality;
        load_option |= aiProcess_RemoveRedundantMaterials;
        load_option |= aiProcess_FindDegenerates;
        load_option |= aiProcess_FindInvalidData;
        load_option |= aiProcess_FindInstances;
        load_option |= aiProcess_OptimizeMeshes;
        load_option |= aiProcess_ConvertToLeftHanded;
        load_option |= aiProcess_GenUVCoords;
        //load_option |= aiProcess_TransformUVCoords;
        //load_option |= aiProcess_SplitByBoneCount;
        //load_option |= aiProcess_SortByPType;
        //load_option |= aiProcess_SplitLargeMeshes;
        //load_option |= aiProcess_FixInfacingNormals;
        //load_option |= aiProcess_OptimizeGraph;
        //load_option |= aiProcess_FlipUVs;

        if (fileExtention == ".obj")
        {
            load_option |= aiProcess_OptimizeGraph;
        }
#endif
        //importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);
        //importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_READ_MATERIALS, false);
        importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_READ_TEXTURES, false);
        importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_READ_LIGHTS, false);
        importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_READ_CAMERAS, false);
        if (m_importNormals == ModelImporterNormals::Calculate)
        {
            importer.SetPropertyInteger(AI_CONFIG_PP_RVC_FLAGS, aiComponent_NORMALS);
            load_option |= aiProcess_GenSmoothNormals |aiProcess_RemoveComponent;
        }
        else
        {
            load_option |= aiProcess_GenNormals;
        }
        bool load_tangent = (m_importTangents != ModelImporterTangents::None);
        if (load_tangent)
        {
            load_option |= aiProcess_CalcTangentSpace;
        }
        
        const aiScene* scene = importer.ReadFile(path.string().c_str(), load_option);
        if (!scene)
        {
            Debug::LogError(importer.GetErrorString());
            Debug::LogError("Can not open file %s", path.c_str());
            abort();
        }

        //bool load_uv = (m_vertexUsages & (int)VertexUsage::UV) != 0;
        bool load_uv = true;
        
        m_model = std::make_shared<Model>();
        m_model->m_name = path.stem().string();
        bool isFBX = fileExtention == ".fbx";
        
        bool loadAnimation = scene->HasAnimations();
        if (loadAnimation)
        {
            Debug::Log("%s has animation", path.c_str());
        }

        m_model->m_rootNode = buildModelTree(scene->mRootNode);
        
        for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; meshIndex++)
        {
            auto mesh = ParseMesh(scene->mMeshes[meshIndex], load_uv, load_tangent);
            if (mesh->m_name.empty())
            {
                mesh->m_name = "mesh" + boost::lexical_cast<std::string>(m_model->m_meshes.size());
            }
            m_model->m_meterailIndexForEachMesh.push_back(scene->mMeshes[meshIndex]->mMaterialIndex);
            m_model->AddMesh(mesh);
        }

        for (uint32_t animIndex = 0; animIndex < scene->mNumAnimations; ++animIndex)
        {
            auto animation = ParseAnimation(scene->mAnimations[animIndex], m_fileScale);
#ifdef REMOVE_FBX_PIVOT
            if (isFBX)
                RemoveDummyNodeFBX(animation);
#endif
            m_model->m_animations.push_back(animation);
        }
        
        for (uint32_t material_idx = 0; material_idx < scene->mNumMaterials; ++material_idx)
        {
            aiMaterial* mtl = scene->mMaterials[material_idx];
            aiString name;
            mtl->Get(AI_MATKEY_NAME, name);
            //Debug::Log("%s", name.C_Str());
            MaterialPtr material = Material::InstantiateBuiltinMaterial("PBR");
            material->SetFloat("Metallic", 0);
            material->SetFloat("Roughness", 0.5f);
            material->SetFloat("Specular", 0.5f);
            material->SetVector3("BaseColor", Vector3(1, 1, 1));
            material->setName(std::string(name.C_Str()));
            m_model->m_materials.push_back(material);
        }
        
        return m_model;
    }


    void ModelImporter::
    RemoveDummyNodeFBX(
        AnimationPtr animation)
    {
        for (auto& it : m_nodeTransformations)
        {
            auto& name = it.first;
            AnimationNode resultNode;
            resultNode.name = name;

            // position
            {
                auto it2 = it.second.find("Translation");
                if (it2 == it.second.end())
                {   // No translation subnode;
                    resultNode.positionKeys.emplace_back(Vector3Key{ 0.f, Vector3::zero });
                }
                else
                {
                    Matrix4x4 positionMat = it2->second;
                    Vector3 initPosition(positionMat.m[0][3], positionMat.m[1][3], positionMat.m[2][3]);
                    auto fullName = name + "_$AssimpFbx$_Translation";
                    auto it3 = animation->channels.find(fullName);
                    if (it3 == animation->channels.end())
                    { // no animation on Translation
                        resultNode.positionKeys.emplace_back(Vector3Key{ 0.f, initPosition });
                    }
                    else
                    {
#ifdef DEBUG_ANIMATION
                        assert(it3->second.rotationKeys.size() == 1);
                        assert(it3->second.rotationKeys[0].value == Quaternion::identity);
                        assert(it3->second.scalingKeys.size() == 1);
                        assert(it3->second.scalingKeys[0].value == Vector3::one);
#endif
                        resultNode.positionKeys = it3->second.positionKeys;
                        animation->channels.erase(it3);
                    }
                }
            }

            // rotation
            {
                Matrix4x4 preRotation, postRotation;
                {
                    auto it2 = it.second.find("PreRotation");
                    if (it2 != it.second.end())
                    {
                        preRotation = it2->second;
                    }
                    auto it3 = it.second.find("PostRotation");
                    if (it3 != it.second.end())
                    {
                        postRotation = it3->second;
                    }
                }

                auto it2 = it.second.find("Rotation");
                if (it2 == it.second.end())
                {   // No rotation subnode;
                    auto rotMat = preRotation * postRotation;
#ifdef DEBUG_ANIMATION
                    Vector3 pos;
                    Quaternion rot;
                    Vector3 scale;
                    Matrix4x4::Decompose(rotMat, &pos, &rot, &scale);
                    assert(pos == Vector3::zero);
                    assert(scale == Vector3::one);
                    assert(rot == rotMat.ToRotation());
#else
                    Quaternion rot = rotMat.ToRotation();
#endif
                    resultNode.rotationKeys.emplace_back(QuaternionKey{ 0.f, rot });
                }
                else
                {
                    auto fullName = name + "_$AssimpFbx$_Rotation";
                    auto it3 = animation->channels.find(fullName);
                    if (it3 == animation->channels.end())
                    { // no animation on rotation
                        Matrix4x4 rotMat = preRotation * it2->second * postRotation;
#ifdef DEBUG_ANIMATION
                        Vector3 pos;
                        Quaternion initRotation;
                        Vector3 scale;
                        Matrix4x4::Decompose(rotMat, &pos, &initRotation, &scale);
                        assert(pos == Vector3::zero);
                        assert(scale == Vector3::one);
                        assert(initRotation == rotMat.ToRotation());
#else
                        Quaternion initRotation = rotMat.ToRotation();
#endif
                        resultNode.rotationKeys.emplace_back(QuaternionKey{ 0.f, initRotation });
                    }
                    else
                    {
#ifdef DEBUG_ANIMATION
                        assert(it3->second.positionKeys.size() == 1);
                        assert(it3->second.positionKeys[0].value == Vector3::zero);
                        assert(it3->second.scalingKeys.size() == 1);
                        assert(it3->second.scalingKeys[0].value == Vector3::one);
#endif
                        for (auto& rk : it3->second.rotationKeys) {
                            auto rotMat = preRotation * Matrix4x4::FromRotation(rk.value) * postRotation;
                            Vector3 pos;
                            Quaternion rot;
                            Vector3 scale;
                            Matrix4x4::Decompose(rotMat, &pos, &rot, &scale);
                            resultNode.rotationKeys.emplace_back(QuaternionKey{ rk.time, rot });
                        }
                        //resultNode.rotationKeys = it3->second.rotationKeys;
                        animation->channels.erase(it3);
                    }
                }
            }

            // scale
            {
                auto it2 = it.second.find("Scaling");
                if (it2 == it.second.end())
                {   // No scaling subnode;
                    resultNode.scalingKeys.emplace_back(Vector3Key{ 0.f, Vector3::one });
                }
                else
                {
                    auto fullName = name + "_$AssimpFbx$_Scaling";
                    auto it3 = animation->channels.find(fullName);
                    if (it3 == animation->channels.end())
                    { // no animation on Scaling
                        Matrix4x4 scaleMat = it2->second;
                        Vector3 pos;
                        Quaternion rot;
                        Vector3 initScale;
                        Matrix4x4::Decompose(scaleMat, &pos, &rot, &initScale);
                        assert(pos == Vector3::zero);
                        assert(rot == Quaternion::identity);
                        resultNode.scalingKeys.emplace_back(Vector3Key{ 0.f, initScale });
                    }
                    else
                    {
#ifdef DEBUG_ANIMATION
                        assert(it3->second.positionKeys.size() == 1);
                        assert(it3->second.positionKeys[0].value == Vector3::zero);
                        assert(it3->second.rotationKeys.size() == 1);
                        assert(it3->second.rotationKeys[0].value == Quaternion::identity);
#endif
                        resultNode.scalingKeys = it3->second.scalingKeys;
                        animation->channels.erase(it3);
                    }
                }
            }

            animation->channels[name] = resultNode;
        }
    }

    GameObjectPtr Model::
    CreateGameObject() const
    {
        for (auto& mesh : m_meshes)
        {
            mesh->UploadMeshData();
        }

        std::map<std::string, std::weak_ptr<GameObject>> nameToGameObject;
        auto root = ResursivelyCreateGameObject(m_rootNode, nameToGameObject);
        if (m_animations.size() > 0)
        {
            auto animator = root->AddComponent<Animator>();
            animator->m_animation = m_animations.front();
        }
        root->setName(m_name);
        return root;
    }


    GameObjectPtr Model::
    ResursivelyCreateGameObject(
        const ModelNodePtr & node,
        std::map<std::string, std::weak_ptr<GameObject>>& nameToGameObject) const
    {
        auto go = Scene::CreateGameObject(node->name);
        go->transform()->setLocalToWorldMatrix(node->transform);
        nameToGameObject[node->name] = go;

        if (m_rootGameObject.expired())
        {
            m_rootGameObject = go;
        }

        if (node->meshesIndices.size() == 1)
        {
            uint32_t idx = node->meshesIndices.front();
            const auto& mesh = m_meshes[idx];
            mesh->setName(node->name);
            auto material = m_materials[m_meterailIndexForEachMesh[idx]];
            if (mesh->m_skinned)
            {
                auto meshRenderer = go->AddComponent<SkinnedMeshRenderer>();
                meshRenderer->SetMaterial(material);
                meshRenderer->setSharedMesh(mesh);
                meshRenderer->setAvatar(m_avatar);
                meshRenderer->setRootBone(m_rootGameObject.lock()->transform());
            }
            else
            {
                auto meshRenderer = go->AddComponent<MeshRenderer>();
                meshRenderer->SetMaterial(material);
                auto meshFilter = go->AddComponent<MeshFilter>();
                meshFilter->SetMesh(mesh);
            }
        }
        else if (node->meshesIndices.size() > 1)
        {
            for (auto& idx : node->meshesIndices)
            {
                auto& m = m_meshes[idx];
                auto child = Scene::CreateGameObject(m->name());
                child->transform()->SetParent(go->transform());
                nameToGameObject[m->name()] = child;
                const auto& mesh = m_meshes[idx];
                auto material = m_materials[m_meterailIndexForEachMesh[idx]];
                if (mesh->m_skinned)
                {
                    auto meshRenderer = child->AddComponent<SkinnedMeshRenderer>();
                    meshRenderer->SetMaterial(material);
                    meshRenderer->setSharedMesh(mesh);
                    meshRenderer->setAvatar(m_avatar);
                    meshRenderer->setRootBone(m_rootGameObject.lock()->transform());
                }
                else
                {
                    auto meshRenderer = child->AddComponent<MeshRenderer>();
                    meshRenderer->SetMaterial(material);
                    auto meshFilter = child->AddComponent<MeshFilter>();
                    meshFilter->SetMesh(mesh);
                }
            }
        }
        
        for (auto& c : node->children)
        {
            auto child = ResursivelyCreateGameObject(c, nameToGameObject);
            child->transform()->SetParent(go->transform());
        }
        
        return go;
    }
}
