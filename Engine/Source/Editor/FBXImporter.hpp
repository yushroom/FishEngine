#pragma once

#include <unordered_map>
#include "FishEditor.hpp"
#include "ModelImporter.hpp"
#include <Path.hpp>
#include <Prefab.hpp>

namespace fbxsdk
{
	class FbxScene;
	class FbxNode;
	class FbxMesh;
	class FbxGeometry;
	class FbxAnimLayer;
	class FbxSurfaceMaterial;
}

class FBXAnimationClip;
class FBXImportScene;

namespace FishEditor
{
	struct Meta(NonSerializable) ModelCollection
	{
		FishEngine::PrefabPtr					m_modelPrefab;
		FishEngine::GameObjectPtr				m_rootNode;
		FishEngine::AvatarPtr					m_avatar;

		std::vector<FishEngine::MeshPtr>		m_meshes;
		std::unordered_map<fbxsdk::FbxMesh*, size_t> 
												m_fbxMeshLookup; // fbxmesh -> index in m_meshes

		std::vector<FishEngine::MaterialPtr>	m_materials;
		std::unordered_map<fbxsdk::FbxSurfaceMaterial*, size_t> 
												m_fbxMaterialLookup;

		std::map<FishEngine::MeshPtr, std::vector<uint32_t>>
												m_boneIndicesForEachMesh;
		std::vector<FishEngine::TransformPtr>	m_bones;
		std::vector<FishEngine::Matrix4x4>		m_bindposes;
		std::vector<FishEngine::SkinnedMeshRendererPtr> 
												m_skinnedMeshRenderers;
	};
	
	class Meta(NonSerializable) FBXImporter : public ModelImporter
	{
		//InjectClassName(FBXImporter)
	public:
		FBXImporter() = default;
		
		FishEngine::PrefabPtr Load(FishEngine::Path const & path);
		
	protected:
		void ImportTo(FishEngine::GameObjectPtr & model);
		virtual void Reimport() override;
		
		void RecursivelyBuildFileIDToRecycleName(FishEngine::TransformPtr const & transform);
		virtual void BuildFileIDToRecycleName() override;
		
	private:
		FishEngine::GameObjectPtr ParseNodeRecursively(fbxsdk::FbxNode* pNode);

		FishEngine::MeshPtr ParseMesh(fbxsdk::FbxMesh* fbxMesh);

		FishEngine::MaterialPtr ParseMaterial(fbxsdk::FbxSurfaceMaterial * pMaterial);

		FishEngine::TexturePtr GetTextureInfo(fbxsdk::FbxGeometry* pGeometry);

		void GetLinkData(fbxsdk::FbxMesh* pGeometry, FishEngine::MeshPtr mesh, std::map<uint32_t, uint32_t> const & vertexIndexRemapping);

		void UpdateBones(FishEngine::TransformPtr const & node);
		
		void ImportAnimations(fbxsdk::FbxScene* scene);
		
		void ImportAnimations(fbxsdk::FbxAnimLayer* layer, fbxsdk::FbxNode * node, FBXAnimationClip & clip);
		
		int m_boneCount = 0;
		//std::vector<FishEngine::TransformPtr> m_bones;
		
		ModelCollection m_model;
		//std::vector<FishEngine::Matrix4x4> m_bindPoses;
	};
}
