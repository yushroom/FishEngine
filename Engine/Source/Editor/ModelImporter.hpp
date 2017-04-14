#ifndef ModelImporter_hpp
#define ModelImporter_hpp

#include "AssetImporter.hpp"
#include <Mesh.hpp>
#include <Scene.hpp>
#include <Material.hpp>
#include <MeshFilter.hpp>
#include <Animator.hpp>
#include <PrimitiveType.hpp>

//struct aiNode;
//struct aiMesh;
//struct aiAnimation;

class ModelImporterInspector;

namespace FishEditor
{
#if 0
	struct ModelNode
	{
		uint32_t                index;
		std::string             name;
		
		Meta(NonSerializable)
		ModelNode*              parent;
		
		Meta(NonSerializable)
		std::vector<std::shared_ptr<ModelNode>> children;
		
		std::vector<uint32_t>   meshesIndices;
		//uint32_t                materialIndices;
		
		Meta(NonSerializable)
		FishEngine::Matrix4x4	transform;
		bool                    isBone;
	};


	class Meta(NonSerializable) Model
	{
	public:
		//InjectClassName(Model)

		Model() : m_avatar(std::make_shared<FishEngine::Avatar>()) {}
		
		Model(const Model&) = delete;
		Model& operator=(const Model& model) = delete;
		
		FishEngine::GameObjectPtr CreateGameObject() const;
		
		FishEngine::MeshPtr mainMesh() const
		{
			return m_meshes[0];
		}
		
		FishEngine::AnimationChannelPtr mainAnimation() const
		{
			if (m_animations.empty())
				return nullptr;
			return m_animations.front();
		}

		FishEngine::AvatarPtr avatar() const
		{
			return m_avatar;
		}
		
	private:
		friend class ModelImporter;
		std::string m_name;

		void AddMesh(FishEngine::MeshPtr& mesh);

		FishEngine::GameObjectPtr
		ResursivelyCreateGameObject(const ModelNodePtr& node,
									std::map<std::string, std::weak_ptr<FishEngine::GameObject>>& nameToGameObject) const;
		
		std::vector<FishEngine::MaterialPtr> m_materials;
		std::vector<uint32_t> m_meterailIndexForEachMesh; // same size as m_meshes;
		std::vector<FishEngine::MeshPtr> m_meshes;

		Meta(NonSerializable)
		std::vector<FishEngine::AnimationChannelPtr> m_animations;
		//std::vector<ModelNode::PModelNode> m_modelNodes;

		Meta(NonSerializable)
		ModelNodePtr m_rootNode;
		//std::vector<ModelNode::PModelNode> m_bones;
		//std::map<std::string, int> m_boneToIndex;
		FishEngine::AvatarPtr m_avatar;

		mutable std::weak_ptr<FishEngine::GameObject>   m_rootGameObject; // temp
		//mutable std::vector<std::weak_ptr<SkinnedMeshRenderer>> m_skinnedMeshRenderersToFindLCA;
	};
#endif
	
	// Vertex normal generation options for ModelImporter.
	enum class ModelImporterNormals
	{
		Import,     // Import vertex normals from model file(default).
		Calculate,  // Calculate vertex normals.
		None,       // Do not import vertex normals.
	};


	// Vertex tangent generation options for ModelImporter.
	enum class ModelImporterTangents
	{
		Import,         // Import vertex tangents from model file.
		//CalculateMikk,// Calculate tangents using MikkTSpace(default).
		Calculate,      // Calculate tangents.
		None,           // Do not import vertex tangents.
	};
	
	// Material naming options for ModelImporter.
	enum class ModelImporterMaterialName
	{
		BasedOnTextureName,                 // Use material names in the form <textureName>.mat.
		BasedOnMaterialName,                // Use a material name of the form <materialName>.mat.
		BasedOnModelNameAndMaterialName,    // Use material names in the form <modelFileName>-<materialName>.mat.
	};
	
	enum class ModelImporterAnimationType
	{
		None,       // Generate no animation data.
		//Legacy,     // Generate a legacy animation type.
		Generic,    // Generate a generic animator.
		Human,      // Generate a human animator.
	};
	
	enum class ModelImporterMaterialSearch
	{
		Local,          // Search in local Materials folder.
		RecursiveUp,    // Recursive-up search in Materials folders.
		Everywhere,     // Search in all project.
	};
	
	enum class ModelImporterMeshCompression
	{
		Off,        // No mesh compression (default).
		Low,        // Low amount of mesh compression.
		Medium,     // Medium amount of mesh compression.
		High,       // High amount of mesh compression.
	};
	
	enum class ModelImporterAnimationCompression
	{
		Off,                                // No animation compression.
		KeyframeReduction,                  // Perform keyframe reduction.
		KeyframeReductionAndCompression,    // Perform keyframe reduction and compression.
		Optimal,                            //Perform keyframe reduction and choose the best animation curve representation at runtime to reduce memory footprint (default).
	};
	
	class ModelImporter : public AssetImporter
	{
	public:
		InjectClassName(ModelImporter)

		ModelImporter() = default;
		
		ModelImporter& operator=(ModelImporter const & rhs);
		
		void setFileScale( const float fileScale )
		{
			m_fileScale = fileScale;
		}
		
		ModelPtr LoadFromFile( const FishEngine::Path& path );

		void setImportNormals( ModelImporterNormals importNormals )
		{
			m_importNormals = importNormals;
		}

		void setImportTangents( ModelImporterTangents importTangents )
		{
			m_importTangents = importTangents;
		}

	protected:

		Meta(NonSerializable)
		int m_nextMeshFileID = FishEngine::ClassID<FishEngine::Mesh>() * 100000;
		
		virtual void Reimport() override;

	protected:
		
		friend class Inspector;
		friend class MainEditor;
		friend class ::ModelImporterInspector;

		static void Init();

		// Global scale factor for importing.
		float m_globalScale = 1.0f;
		
		// File scale factor (if available) or default one. (Read-only).
		// fileSscale is determined by model file and can not be modified by user.
		Meta(NonSerializable)
		float m_fileScale = 1.0f;

		// Vertex normal import options.
		ModelImporterNormals m_importNormals    = ModelImporterNormals::Import;

		// Vertex tangent import options.
		ModelImporterTangents m_importTangents  = ModelImporterTangents::Calculate;
		
		// Existing material search setting.
		ModelImporterMaterialSearch m_materialSearch;

		// remove dummy nodes
		Meta(NonSerializable)
		std::map<std::string, std::map<std::string, FishEngine::Matrix4x4>> m_nodeTransformations;
		
		
#if 0
		ModelNodePtr
		buildModelTree(const aiNode*   assimp_node);
		
		FishEngine::MeshPtr
		ParseMesh(const aiMesh* assimp_mesh, bool load_uv, bool load_tangent);
		
		void RemoveDummyNodeFBX(FishEngine::AnimationChannelPtr animation );
		
		Meta(NonSerializable)
		ModelPtr      m_model;
#endif

	}; // end of class ModelImporter
}

#endif /* ModelImporter_hpp */
