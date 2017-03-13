#pragma once

#include "FishEditor.hpp"
#include "ModelImporter.hpp"
#include <Path.hpp>
#include <Prefab.hpp>

namespace fbxsdk
{
	class FbxNode;
	class FbxMesh;
	class FbxGeometry;
}

namespace FishEditor
{
	struct Meta(NonSerializable) ModelCollection
	{
		FishEngine::PrefabPtr					m_modelPrefab;
		FishEngine::GameObjectPtr				m_rootNode;
		FishEngine::AvatarPtr					m_avatar;
		std::vector<FishEngine::MeshPtr>		m_meshes;
		std::map<FishEngine::MeshPtr, std::vector<uint32_t>>
												m_boneIndicesForEachMesh;
		std::vector<FishEngine::TransformPtr>	m_bones;
		std::vector<FishEngine::Matrix4x4>		m_bindposes;
	};
	
	class Meta(NonSerializable) FBXImporter : public ModelImporter
	{
		//InjectClassName(FBXImporter)
	public:
		FBXImporter() = default;
		
		FishEngine::PrefabPtr Load(boost::filesystem::path const & path);
		
	protected:
		void ImportTo(FishEngine::GameObjectPtr & model);
		//virtual void Reimport() override;
		
		void RecursivelyBuildFileIDToRecycleName(FishEngine::TransformPtr const & transform);
		virtual void BuildFileIDToRecycleName() override;
		
	private:
		FishEngine::GameObjectPtr ParseNodeRecursively(fbxsdk::FbxNode* pNode);

		FishEngine::MeshPtr MeshFromFbxMesh(fbxsdk::FbxMesh* fbxMesh);

		FishEngine::TexturePtr GetTextureInfo(fbxsdk::FbxGeometry* pGeometry);

		void GetLinkData(fbxsdk::FbxMesh* pGeometry, FishEngine::MeshPtr mesh, std::map<uint32_t, uint32_t> const & vertexIndexRemapping);

		void UpdateBones(FishEngine::TransformPtr const & node);
		
		int m_boneCount = 0;
		//std::vector<FishEngine::TransformPtr> m_bones;
		
		ModelCollection m_model;
		//std::vector<FishEngine::Matrix4x4> m_bindPoses;
	};
}
