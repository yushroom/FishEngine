#include "ModelImporter.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

//#include <fbxsdk.h>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include "Debug.hpp"
#include "Common.hpp"

#define PRESERVE_PIVOT

namespace FishEngine {
    
    void Model::AddMesh(Mesh::PMesh& mesh)
    {
        m_meshes.push_back(mesh);
    }

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
    

    Model::PModel Model::
    builtinModel(
        const BuiltinModelTyep type) {
        return s_builtinModels[type];
    }
    

    Matrix4x4
    ConvertMatrix(
        const aiMatrix4x4& m) {
        Matrix4x4 result;
        memcpy(result.m, &m.a1, 16*sizeof(float));
        //result *= Matrix4x4(1, 0, 0, 0,   0, 1, 0, 0,   0, 0, -1, 0,   0, 0, 0, 1);
        return result;
    }


    ModelNode::PModelNode ModelImporter::
    buildModelTree(
        const aiNode*   assimp_node)
    {
        //Debug::Log("NODE: %s mesh count: %d ", assimp_node->mName.C_Str(), assimp_node->mNumMeshes);
        //for (int i = 0; i < assimp_node->mNumChildren; ++i) {
        //    Debug::Log("    CHILD: %s", assimp_node->mChildren[i]->mName.C_Str());
        //}
        auto node = std::make_shared<ModelNode>();
        node->name = assimp_node->mName.C_Str();
        Debug::LogWarning("Node: %s", node->name.c_str());
        node->transform = ConvertMatrix(assimp_node->mTransformation);
        node->isBone = false;
        //model->m_bones.push_back(node);
        //node->index = model->m_bones.size();
        int index = m_model->m_avatar->m_boneToIndex.size();
        m_model->m_avatar->m_boneToIndex[node->name] = index;
        node->index = index;
        m_nodes[node->name] = node;
        
        for (uint32_t i = 0; i < assimp_node->mNumMeshes; ++i) {
            node->meshesIndices.push_back(assimp_node->mMeshes[i]);
        }

        for (uint32_t i = 0; i < assimp_node->mNumChildren; ++i) {
            //Debug::Log("    CHILD: %s", assimp_node->mName.C_Str());
            auto child = buildModelTree(assimp_node->mChildren[i]);
            node->children.push_back(child);
            child->parent = node.get();
        }
        return node;
    }


//    ModelNode::PModelNode ModelImporter::
//    buildModelTree(
//        fbxsdk::FbxNode* pNode)
//    {
//        const char* nodeName = pNode->GetName();
//        auto node = std::make_shared<ModelNode>();
//        node->isBone = false;
//        node->name = nodeName;
//        Debug::Log("FBX node: %s", nodeName);
//        
//        if (pNode->GetNodeAttribute())
//        {
//            switch (pNode->GetNodeAttribute()->GetAttributeType())
//            {
//            case FbxNodeAttribute::eMesh:
//                auto pMesh = pNode->GetMesh();
//                auto mesh = ParseMesh(pMesh);
//                m_model->AddMesh(mesh);
//                break;
//            }
//        }
//        for (int j = 0; j < pNode->GetChildCount(); j++) {
//            auto child = buildModelTree(pNode->GetChild(j));
//            node->children.push_back(child);
//            child->parent = node.get();
//        }
//        return node;
//    }


    Mesh::PMesh ModelImporter::
    ParseMesh(
        const aiMesh*   assimp_mesh, 
        int             vertexUsage, 
        bool            load_uv, 
        bool            load_tangent)
    {
        auto mesh = std::make_shared<Mesh>();
        mesh->m_name = assimp_mesh->mName.C_Str();
        bool has_uv = assimp_mesh->HasTextureCoords(0);

        auto n_vertices = assimp_mesh->mNumVertices;
        auto n_triangles = assimp_mesh->mNumVertices;
        auto n_bones = assimp_mesh->mNumBones;
        mesh->m_positionBuffer.reserve(n_vertices * 3);
        mesh->m_normalBuffer.reserve(n_vertices * 3);
        mesh->m_uvBuffer.reserve(n_vertices * 2);
        mesh->m_indexBuffer.reserve(n_triangles * 3);
        mesh->m_tangentBuffer.reserve(n_vertices * 3);


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

                mesh->m_bones[boneName].boneOffset = ConvertMatrix(bone->mOffsetMatrix);
                
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


//    Mesh::PMesh ModelImporter::
//    ParseMesh(
//        FbxMesh* pMesh)
//    {
//        assert(pMesh != nullptr);
//        auto mesh = std::make_shared<Mesh>();
//
//        //assert(pMesh->IsTriangleMesh());
//
//        //auto n_controlPoints = pMesh->GetControlPointsCount();
//        //auto controlPoints = pMesh->GetControlPoints();
//        auto n_vertices = pMesh->GetPolygonVertexCount();
//        auto n_triangles = pMesh->GetPolygonCount();;
//        mesh->m_positionBuffer.reserve(n_vertices * 3);
//        mesh->m_normalBuffer.reserve(n_vertices * 3);
//        mesh->m_uvBuffer.reserve(n_vertices * 2);
//        mesh->m_indexBuffer.resize(n_triangles * 3);
//        mesh->m_tangentBuffer.reserve(n_triangles * 3);
//
//        int* vertices = pMesh->GetPolygonVertices();
//        memcpy(mesh->m_indexBuffer.data(), vertices, n_triangles * 3 * sizeof(int));
//
//        auto positions = pMesh->GetControlPoints();
//        for (int i = 0; i < n_vertices; ++i)
//        {
//            auto& p = positions[i];
//            mesh->m_positionBuffer.push_back(p.mData[0]);
//            mesh->m_positionBuffer.push_back(p.mData[1]);
//            mesh->m_positionBuffer.push_back(p.mData[2]);
//        }
//
//        auto n_uvs = pMesh->GetElementUVCount();
//        auto n_normals = pMesh->GetElementNormalCount();
//        auto n_tangents = pMesh->GetElementTangentCount();
//        auto normals = pMesh->GetElementNormal(0);
//        switch (normals->GetMappingMode()) 
//        {
//        case FbxLayerElement::eByControlPoint:
//            Debug::Log("by control point");
//            switch (normals->GetReferenceMode())
//            {
//            case FbxLayerElement::eDirect:
//                n_normals = normals->GetDirectArray().GetCount();
//                for (int i = 0; i < n_normals; ++i) {
//                    auto n = normals->GetDirectArray().GetAt(i);
//                    mesh->m_normalBuffer.push_back(n.mData[0]);
//                    mesh->m_normalBuffer.push_back(n.mData[1]);
//                    mesh->m_normalBuffer.push_back(n.mData[2]);
//                }
//                Debug::Log("eDirect");
//                break;
//            case FbxLayerElement::eIndexToDirect:
//                Debug::Log("eIndexToDirect");
//                abort();
//                break;
//            }
//            break;
//        case FbxLayerElement::eByPolygonVertex:
//            Debug::Log("by polygon vertex");
//            switch (normals->GetReferenceMode())
//            {
//            case FbxLayerElement::eDirect:
//                n_normals = normals->GetDirectArray().GetCount();
//                for (int i = 0; i < n_normals; ++i) {
//                    auto n = normals->GetDirectArray().GetAt(i);
//                    mesh->m_normalBuffer.push_back(n.mData[0]);
//                    mesh->m_normalBuffer.push_back(n.mData[1]);
//                    mesh->m_normalBuffer.push_back(n.mData[2]);
//                }
//                Debug::Log("eDirect");
//                break;
//            case FbxLayerElement::eIndexToDirect:
//                Debug::Log("eIndexToDirect");
//                abort();
//                break;
//            }
//            break;
//        }
//
//        Debug::Log("triangles %ld", n_triangles);
//        //for (int i = 0; i < triangleCount; ++i) {
//        //    int idx = pMesh->GetPolygonVertexIndex()
//        //}
//        return mesh;
//    }
//
//
//    /* Tab character ("\t") counter */
//    int numTabs = 0;
//
//    /**
//    * Print the required number of tabs.
//    */
//    void PrintTabs() {
//        for (int i = 0; i < numTabs; i++)
//            printf("\t");
//    }
//
//
//    /**
//    * Return a string-based representation based on the attribute type.
//    */
//    FbxString GetAttributeTypeName(FbxNodeAttribute::EType type) {
//        switch (type) {
//        case FbxNodeAttribute::eUnknown: return "unidentified";
//        case FbxNodeAttribute::eNull: return "null";
//        case FbxNodeAttribute::eMarker: return "marker";
//        case FbxNodeAttribute::eSkeleton: return "skeleton";
//        case FbxNodeAttribute::eMesh: return "mesh";
//        case FbxNodeAttribute::eNurbs: return "nurbs";
//        case FbxNodeAttribute::ePatch: return "patch";
//        case FbxNodeAttribute::eCamera: return "camera";
//        case FbxNodeAttribute::eCameraStereo: return "stereo";
//        case FbxNodeAttribute::eCameraSwitcher: return "camera switcher";
//        case FbxNodeAttribute::eLight: return "light";
//        case FbxNodeAttribute::eOpticalReference: return "optical reference";
//        case FbxNodeAttribute::eOpticalMarker: return "marker";
//        case FbxNodeAttribute::eNurbsCurve: return "nurbs curve";
//        case FbxNodeAttribute::eTrimNurbsSurface: return "trim nurbs surface";
//        case FbxNodeAttribute::eBoundary: return "boundary";
//        case FbxNodeAttribute::eNurbsSurface: return "nurbs surface";
//        case FbxNodeAttribute::eShape: return "shape";
//        case FbxNodeAttribute::eLODGroup: return "lodgroup";
//        case FbxNodeAttribute::eSubDiv: return "subdiv";
//        default: return "unknown";
//        }
//    }
//
//
//    /**
//    * Print an attribute.
//    */
//    void PrintAttribute(FbxNodeAttribute* pAttribute) {
//        if (!pAttribute) return;
//
//        FbxString typeName = GetAttributeTypeName(pAttribute->GetAttributeType());
//        FbxString attrName = pAttribute->GetName();
//        PrintTabs();
//        // Note: to retrieve the character array of a FbxString, use its Buffer() method.
//        printf("<attribute type='%s' name='%s'/>\n", typeName.Buffer(), attrName.Buffer());
//    }


    std::shared_ptr<Model> ModelImporter::
    LoadFromFile(
        const std::string&  path,
        int                 vertexUsage,
        MeshLoadFlags       flags)
    {
        Assimp::Importer importer;
        unsigned int load_option = aiProcess_Triangulate;
        load_option |= aiProcess_LimitBoneWeights;
        //load_option |= aiProcess_JoinIdenticalVertices;
        //load_option |= aiProcess_ValidateDataStructure;

        //load_option |= aiProcess_ImproveCacheLocality;
        //load_option |= aiProcess_RemoveRedundantMaterials;
        //load_option |= aiProcess_FindDegenerates;
        //load_option |= aiProcess_FindInvalidData;
        //load_option |= aiProcess_GenUVCoords;
        //load_option |= aiProcess_TransformUVCoords;
        //load_option |= aiProcess_FindInstances;
        //load_option |= aiProcess_OptimizeMeshes;
        //load_option |= aiProcess_SplitByBoneCount;
        //load_option |= aiProcess_SortByPType;
        //load_option |= aiProcess_SplitLargeMeshes;
        //load_option |= aiProcess_FixInfacingNormals;
        //load_option |= aiProcess_OptimizeMeshes;
        //load_option |= aiProcess_SortByPType;
        //load_option |= aiProcess_JoinIdenticalVertices;
        //load_option |= aiProcess_OptimizeGraph;
        load_option |= aiProcess_ConvertToLeftHanded;
        //load_option |= aiProcess_FlipUVs;
#ifndef PRESERVE_PIVOT
        importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);
#endif
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
        
        
        m_model = std::make_shared<Model>();
        m_model->m_name = split(path, "/").back();
        
        bool loadAnimation = scene->HasAnimations();
        if (loadAnimation)
            Debug::Log("%s has animation", path.c_str());

        m_model->m_rootNode = buildModelTree(scene->mRootNode);
        
        for (unsigned int meshIndex = 0; meshIndex < scene->mNumMeshes; meshIndex++)
        {
            auto mesh = ParseMesh(scene->mMeshes[meshIndex], vertexUsage, load_uv, load_tangent);
            if (mesh->m_name.empty()) {
                mesh->m_name = "mesh" + boost::lexical_cast<std::string>(m_model->m_meshes.size());
            }
            m_model->AddMesh(mesh);
        }

        auto ConvertVector3 = [](const aiVector3D& avec3) {
            return Vector3(avec3.x, avec3.y, avec3.z);
        };

        auto ConvertQuaternion = [](const aiQuaternion& aquat) {
            return Quaternion(aquat.x, aquat.y, aquat.z, aquat.w);
        };

        for (uint32_t animIndex = 0; animIndex < scene->mNumAnimations; ++animIndex) {
            m_model->m_animations.push_back(std::make_shared<Animation>());
            auto& animation = m_model->m_animations.back();
            auto a = scene->mAnimations[animIndex];
            animation->name = a->mName.C_Str();
            animation->duration = (float)a->mDuration;
            animation->ticksPerSecond = (float)a->mTicksPerSecond;
            //animation->channels.resize(a->mNumChannels);

            for (uint32_t i = 0; i < a->mNumChannels; ++i) {
                auto an = a->mChannels[i];
                std::string name = an->mNodeName.C_Str();
#ifndef PRESERVE_PIVOT
                auto pos = name.find("_$AssimpFbx$");
                Debug::Log("old name: %s", name.c_str());
                if (pos != std::string::npos) {
                    name = name.substr(0, pos);
                    Debug::Log("new name: %s", name.c_str());
                }
#endif
                auto it = animation->channels.find(name);
                if (it == animation->channels.end()) {
                    animation->channels[name] = AnimationNode();
                }
                auto& n = animation->channels[name];

                n.name = name;
                //if (an->mNumPositionKeys > 1) {
                    //n.positionKeys.resize(an->mNumPositionKeys);
                    for (uint32_t j = 0; j < an->mNumPositionKeys; ++j) {
                        n.positionKeys.emplace_back(Vector3Key{ (float)an->mPositionKeys[j].mTime, ConvertVector3(an->mPositionKeys[j].mValue) });
                    }
                //}
                //n.rotationKeys.resize(an->mNumRotationKeys);
                //if (an->mNumRotationKeys > 1) {
                    for (uint32_t j = 0; j < an->mNumRotationKeys; ++j) {
                        n.rotationKeys.emplace_back(QuaternionKey{ (float)an->mRotationKeys[j].mTime, ConvertQuaternion(an->mRotationKeys[j].mValue) });
                    }
                //}
                //n.scalingKeys.resize(an->mNumScalingKeys);
                //if (an->mNumScalingKeys > 1) {
                    for (uint32_t j = 0; j < an->mNumScalingKeys; ++j) {
                        n.scalingKeys.emplace_back(Vector3Key{ (float)an->mScalingKeys[j].mTime, ConvertVector3(an->mScalingKeys[j].mValue) });
                    }
                //}
                std::vector<float> all_key_time;
                for (uint32_t j = 0; j < an->mNumPositionKeys; ++j) {
                    all_key_time.push_back((float)an->mPositionKeys[j].mTime);
                }
                for (uint32_t j = 0; j < an->mNumRotationKeys; ++j) {
                    all_key_time.push_back((float)an->mRotationKeys[j].mTime);
                }
                for (uint32_t j = 0; j < an->mNumScalingKeys; ++j) {
                    all_key_time.push_back((float)an->mScalingKeys[j].mTime);
                }
                std::sort(all_key_time.begin(), all_key_time.end());
                auto last = std::unique(all_key_time.begin(), all_key_time.end());
                all_key_time.erase(last, all_key_time.end());
                
                n.transformationKeys.reserve(all_key_time.size());
                for (auto& t : all_key_time) {
                    Matrix4x4 m;
                    auto pos = Vector3::zero;
                    auto rot = Quaternion::identity;
                    auto scale = Vector3::one;

                    uint32_t j = 0;
                    for (j = 1; j < an->mNumPositionKeys && t > (float)an->mPositionKeys[j].mTime; ++j) {}
                    pos = ConvertVector3(an->mPositionKeys[j-1].mValue);

                    for (j = 1; j < an->mNumRotationKeys && t > (float)an->mRotationKeys[j].mTime; ++j) {}
                    rot = ConvertQuaternion(an->mRotationKeys[j-1].mValue);

                    for (j = 1; j < an->mNumScalingKeys && t > (float)an->mScalingKeys[j].mTime; ++j) {}
                    scale = ConvertVector3(an->mScalingKeys[j-1].mValue);

                    m.SetTRS(pos, rot, scale);
                    n.transformationKeys.emplace_back(TransformationKey{ t, m });
                }
            }
            Debug::Log("animation name: %s", a->mName.C_Str());
        }

        for (auto& animation : m_model->m_animations) {
            for (auto & channel : animation->channels) {

            }
        }

        return m_model;
    }


//    std::shared_ptr<Model> ModelImporter::
//    LoadFBX(
//        const std::string&  path, 
//        int                 vertexUsage /*= VertexUsagePNUT*/, 
//        MeshLoadFlags       flags /*= 0*/)
//    {
//        // Initialize the SDK manager. This object handles all our memory management.
//        FbxManager* lSdkManager = FbxManager::Create();
//
//        // Create the IO settings object.
//        FbxIOSettings *ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
//        lSdkManager->SetIOSettings(ios);
//
//        // Create an importer using the SDK manager.
//        FbxImporter* lImporter = FbxImporter::Create(lSdkManager, "");
//
//        // Use the first argument as the filename for the importer.
//        if (!lImporter->Initialize(path.c_str(), -1, lSdkManager->GetIOSettings())) {
//            printf("Call to FbxImporter::Initialize() failed.\n");
//            printf("Error returned: %s\n\n", lImporter->GetStatus().GetErrorString());
//            exit(-1);
//        }
//
//        // Create a new scene so that it can be populated by the imported file.
//        FbxScene* lScene = FbxScene::Create(lSdkManager, "myScene");
//
//        // Import the contents of the file into the scene.
//        lImporter->Import(lScene);
//
//        // The file is imported; so get rid of the importer.
//        lImporter->Destroy();
//
//        m_model = std::make_shared<Model>();
//        m_model->m_name = split(path, "/").back();
//
//        // Print the nodes of the scene and their attributes recursively.
//        // Note that we are not printing the root node because it should
//        // not contain any attributes.
//        FbxNode* lRootNode = lScene->GetRootNode();
//        buildModelTree(lRootNode);
//        //if (lRootNode) {
//        //    for (int i = 0; i < lRootNode->GetChildCount(); i++)
//        //        buildModelTree(lRootNode->GetChild(i));
//        //}
//        // Destroy the SDK manager and all the other objects it was handling.
//        lSdkManager->Destroy();
//
//        return m_model;
//    }


    std::shared_ptr<GameObject> Model::
    CreateGameObject() const
    {
        auto root = ResursivelyCreateGameObject(m_rootNode);
        if (m_animations.size() > 0) {
            auto animator = std::make_shared<Animator>();
            animator->m_animation = m_animations.front();
            root->AddComponent(animator);
        }
        return root;
    }

    std::shared_ptr<GameObject> Model::
    ResursivelyCreateGameObject(
        const ModelNode::PModelNode & node) const
    {
        //auto pos = node->name.find("_$AssimpFbx$");
        //if (pos != std::string::npos) { // remove dummy node
        //    auto trueName = node->name.substr(0, pos);
        //    auto typeName = node->name.substr(pos + 13);
        //    //dummyNodeMap[trueName] = std::map<std::string, Matrix4x4>{ {typeName, node->transform} };
        //    Matrix4x4 transform = node->transform;
        //    assert(node->children.size() == 1);
        //    auto child = node->children[0];
        //    while (child->name != trueName) {
        //        assert(boost::starts_with(child->name, trueName));
        //        assert(child->children.size() == 1);
        //        transform *= child->transform;
        //        child = child->children[0];
        //    }
        //    transform *= child->transform;
        //    child->transform = transform;
        //    return ResursivelyCreateGameObject(child);
        //}

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
                auto material = Material::builtinMaterial("SkinnedMesh");
                auto meshRenderer = std::make_shared<MeshRenderer>(material);
                auto meshFilter = std::make_shared<MeshFilter>(m_meshes[idx]);
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
