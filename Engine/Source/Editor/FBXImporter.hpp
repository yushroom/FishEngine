#pragma once

#include "FishEditor.hpp"
#include "ModelImporter.hpp"
#include <Path.hpp>

namespace fbxsdk
{
	class FbxNode;
	class FbxMesh;
}

namespace FishEditor
{
	struct Meta(NonSerializable) ModelCollection
	{
		FishEngine::GameObjectPtr			m_rootNode;
		FishEngine::AvatarPtr				m_avatar;
		std::vector<FishEngine::MeshPtr>	m_meshes;
	};
	
	class Meta(NonSerializable) FBXImporter : public ModelImporter
	{
		//InjectClassName(FBXImporter)
	public:
		FBXImporter() = default;
		
		FishEngine::GameObjectPtr Load(boost::filesystem::path const & path);
		
	private:
		FishEngine::GameObjectPtr ParseNodeRecursively(fbxsdk::FbxNode* pNode);

		FishEngine::MeshPtr MeshFromFbxMesh(fbxsdk::FbxMesh* fbxMesh);

		void GetLinkData(fbxsdk::FbxMesh* pGeometry, FishEngine::MeshPtr mesh, std::map<uint32_t, uint32_t> vertexIndexRemapping);

		void UpdateBones(FishEngine::TransformPtr const & node);
		
		int m_boneCount = 0;
		std::vector<FishEngine::TransformPtr> m_bones;
		
		ModelCollection m_model;
		std::vector<FishEngine::Matrix4x4> m_bindPoses;
	};
}
