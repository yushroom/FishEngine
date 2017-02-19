#pragma once

#include "FishEditor.hpp"
#include "ModelImporter.hpp"
#include <boost/filesystem.hpp>

namespace fbxsdk
{
	class FbxNode;
	class FbxMesh;
}

namespace FishEditor
{
	struct ModelCollection
	{
		FishEngine::GameObjectPtr			m_rootNode;
		FishEngine::AvatarPtr				m_avatar;
		std::vector<FishEngine::MeshPtr>	m_meshes;
	};
	
	class FBXImporter : public ModelImporter
	{
	public:
		FBXImporter() = default;
		
		FishEngine::GameObjectPtr Load(boost::filesystem::path const & path);
		
	private:
		FishEngine::GameObjectPtr ParseNodeRecursively(fbxsdk::FbxNode* pNode);

		FishEngine::MeshPtr MeshFromFbxMesh(fbxsdk::FbxMesh* fbxMesh);

		void GetLinkData(fbxsdk::FbxMesh* pGeometry, FishEngine::MeshPtr mesh);

		
		int m_boneCount = 0;
		//std::map<std::string, int> m_boneToIndex;
		//std::map<int, std::string> m_indexToBone;
		
		ModelCollection m_model;
	};
}
