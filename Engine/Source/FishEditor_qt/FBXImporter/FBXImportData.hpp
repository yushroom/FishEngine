#pragma once
#include <FishEngine/Matrix4x4.hpp>
#include <FishEngine/Vector3.hpp>
#include <FishEngine/Quaternion.hpp>
#include <FishEngine/Animation/TAnimationCurve.hpp>
#include <FishEngine/BoneWeight.hpp>


namespace fbxsdk
{
	class FbxScene;
	class FbxNode;
	class FbxMesh;
	class FbxGeometry;
	class FbxAnimLayer;
	class FbxSurfaceMaterial;
}

namespace FishEngine
{
	struct AnimationCurves;
	struct RootMotion;
}

namespace FishEditor
{
	constexpr int FBX_IMPORT_MAX_BONE_INFLUENCES = 4;

	/**	Options that control FBX import */
	struct FBXImportOptions
	{
		bool importAnimation = true;
		bool importSkin = true;
		bool importBlendShapes = true;
		bool importNormals = true;
		bool importTangents = true;
		float importScale = 0.01f;
		float animSampleRate = 1.0f / 60.0f;
		bool animResample = false;
		bool reduceKeyframes = true;
	};

	struct FBXImportNode
	{
		~FBXImportNode()
		{
			for (auto child : children)
				delete child;
		}

		//FishEngine::Matrix4x4 localTransform;
		//FishEngine::Matrix4x4 worldTransform;
		FishEngine::Vector3			localPosition;
		FishEngine::Quaternion		localQuaternion;
		FishEngine::Vector3			localScale;

		std::string					name;
		fbxsdk::FbxNode *			fbxNode;
		std::vector<FBXImportNode*> children;
	};

	/**	Contains geometry from one blend shape frame. */
	struct FBXBlendShapeFrame
	{
		std::vector<FishEngine::Vector3> positions;
		std::vector<FishEngine::Vector3> normals;
		std::vector<FishEngine::Vector3> tangents;
		std::vector<FishEngine::Vector3> bitangents;

		float weight;
		String name;
	};

	/**	Contains all geometry for a single blend shape. */
	struct FBXBlendShape
	{
		String name;
		std::vector<FBXBlendShapeFrame> frames;
	};

	struct FBXBone
	{
		FBXImportNode * node;
		FishEngine::Matrix4x4 bindPose;
	};

	struct FBXBoneAnimation
	{
		FBXImportNode * node;
		FishEngine::TAnimationCurve<FishEngine::Vector3> translation;
		FishEngine::TAnimationCurve<FishEngine::Quaternion> rotation;
		FishEngine::TAnimationCurve<FishEngine::Vector3> scale;
	};

	/**	Animation curve required to animate a blend shape. */
	struct FBXBlendShapeAnimation
	{
		String blendShape;
		FishEngine::TAnimationCurve<float> curve;
	};

	struct FBXAnimationClip
	{
		std::string name;
		float start;
		float end;
		uint32_t sampleRate;

		std::vector<FBXBoneAnimation> boneAnimations;
		std::vector<FBXBlendShapeAnimation> blendShapeAnimations;
	};


	/** All information required for creating an animation clip. */
	struct FBXAnimationClipData
	{
		FBXAnimationClipData(
			const std::string& name,
			bool isAdditive,
			uint32_t sampleRate,
			const std::shared_ptr<FishEngine::AnimationCurves>& curves,
			const std::shared_ptr<FishEngine::RootMotion>& rootMotion)
			:name(name), isAdditive(isAdditive), sampleRate(sampleRate), curves(curves), rootMotion(rootMotion)
		{ }

		std::string name;
		bool isAdditive;
		uint32_t sampleRate;
		std::shared_ptr<FishEngine::AnimationCurves> curves;
		std::shared_ptr<FishEngine::RootMotion> rootMotion;
	};

	///**	Imported mesh data. */
	//struct FBXImportMesh
	//{
	//	FbxMesh* fbxMesh;

	//	std::vector<int> indices;
	//	std::vector<Vector3> positions;
	//	std::vector<Vector3> normals;
	//	std::vector<Vector3> tangents;
	//	std::vector<Vector3> bitangents;
	//	std::vector<Color> colors;
	//	std::vector<Vector2> UV[FBX_IMPORT_MAX_UV_LAYERS];
	//	std::vector<int> materials;

	//	std::vector<int> smoothingGroups;
	//	std::vector<FBXBlendShape> blendShapes;

	//	std::vector<FishEngine::BoneWeight> boneInfluences;
	//	std::vector<FBXBone> bones;

	//	//SPtr<MeshData> meshData;
	//	std::vector<SubMesh> subMeshes;

	//	std::vector<FBXImportNode*> referencedBy;
	//};

	///**	Scene information used and modified during FBX import. */
	//struct FBXImportScene
	//{
	//	FBXImportScene();
	//	~FBXImportScene();

	//	std::vector<FBXImportMesh*> meshes;
	//	FBXImportNode* rootNode;

	//	UnorderedMap<FbxNode*, FBXImportNode*> nodeMap;
	//	UnorderedMap<FbxMesh*, UINT32> meshMap;

	//	std::vector<FBXAnimationClip> clips;

	//	float scaleFactor;
	//	Matrix4 globalScale;
	//};
}
