#pragma once

namespace FishEngine
{
    struct Meta(NonSerializable) ModelCollection
    {
        FishEngine::GameObjectPtr               m_rootNode;
        FishEngine::AvatarPtr                   m_avatar;
        std::vector<FishEngine::MeshPtr>        m_meshes;
        std::map<FishEngine::MeshPtr, std::vector<uint32_t>>
                                                m_boneIndicesForEachMesh;
        std::vector<FishEngine::TransformPtr>   m_bones;
        std::vector<FishEngine::Matrix4x4>      m_bindPoses;
    };
}