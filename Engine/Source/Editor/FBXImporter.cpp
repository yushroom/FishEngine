#include "FBXImporter.hpp"

#include <unordered_set>

#include <fbxsdk.h>
#include <fbxsdk/utils/fbxgeometryconverter.h>
#include <boost/algorithm/string/case_conv.hpp>

#include <Debug.hpp>
#include <GameObject.hpp>
#include <MeshFilter.hpp>
#include <MeshRenderer.hpp>
#include <SkinnedMeshRenderer.hpp>
#include <Texture.hpp>
#include <Texture2D.hpp>
#include <Application.hpp>

#include "AssetDataBase.hpp"
#include "FBXImporter/RawMesh.hpp"

//#include <Animation/AnimationUtility.hpp>
//#include <Animation/AnimationClip.hpp>
////#include <Animation/AnimationClipInfo.hpp>
//#include <Animation/AnimationSplitInfo.hpp>

#include <Animation.hpp>
#include <AnimationClip.hpp>
#include <Animation/AnimationCurve.hpp>
#include <Animation/AnimationCurveUtility.hpp>

using namespace FishEngine;
using namespace FishEditor;

Matrix4x4 FBXToNativeType(fbxsdk::FbxAMatrix const & fmatrix)
{
	float f44[4][4];
	auto d44 = fmatrix.Double44();
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			f44[i][j] = static_cast<float>(d44[j][i]);
		}
	}
	return Matrix4x4(f44);
}

Vector3 FBXToNativeType(const FbxVector4& value)
{
	Vector3 native;
	native.x = (float)value[0];
	native.y = (float)value[1];
	native.z = (float)value[2];

	return native;
}

Vector3 FBXToNativeType(const FbxDouble3& value)
{
	return Vector3(static_cast<float>(value[0]),
		static_cast<float>(value[1]),
		static_cast<float>(value[2]));
}

Vector2 FBXToNativeType(const FbxVector2& value)
{
	Vector2 native;
	native.x = (float)value[0];
	native.y = (float)value[1];

	return native;
}

Color FBXToNativeType(const FbxColor& value)
{
	Color native;
	native.r = (float)value[0];
	native.g = (float)value[1];
	native.b = (float)value[2];
	native.a = (float)value[3];

	return native;
}

FbxSurfaceMaterial* FBXToNativeType(FbxSurfaceMaterial* const& value)
{
	return value;
}


RotationOrder FBXToNativeType(EFbxRotationOrder order)
{
	RotationOrder rotationOrder;
	//EFbxRotationOrder order;
	//node->GetRotationOrder(FbxNode::eDestinationPivot, order);
	switch (order)
	{
		case eEulerXYZ:
			rotationOrder = RotationOrder::XYZ; break;
		case eEulerXZY:
			rotationOrder = RotationOrder::XZY; break;
		case eEulerYXZ:
			rotationOrder = RotationOrder::YXZ; break;
		case eEulerYZX:
			rotationOrder = RotationOrder::YZX; break;
		case eEulerZXY:
			rotationOrder = RotationOrder::ZXY; break;
		case eEulerZYX:
			rotationOrder = RotationOrder::ZYX; break;
		default: // do not support eSphericXYZ
			abort();
	}
	return rotationOrder;
}

int FBXToNativeType(const int & value)
{
	return value;
}


// skinned data
void FishEditor::FBXImporter::GetLinkData(FbxMesh* pMesh, MeshPtr mesh, std::map<uint32_t, uint32_t> const & vertexIndexRemapping)
{
	int lSkinCount = pMesh->GetDeformerCount(FbxDeformer::eSkin);
	if (lSkinCount <= 0)
	{
		mesh->m_skinned = false;
		return;
	}
	if (lSkinCount != 1)
	{
		// TODO: multiple skin
		abort();
	}
	mesh->m_skinned = true;
	
	int n_vertices = mesh->vertexCount();
	mesh->m_boneWeights.resize(n_vertices);

	FbxCluster::ELinkMode lClusterMode = ((FbxSkin*)pMesh->GetDeformer(0, FbxDeformer::eSkin))->GetCluster(0)->GetLinkMode();
	
	FbxSkin * lSkinDeformer = (FbxSkin *)pMesh->GetDeformer(0, FbxDeformer::eSkin);
	FbxSkin::EType lSkinningType = lSkinDeformer->GetSkinningType();

	// cluser == bone
	int lClusterCount = lSkinDeformer->GetClusterCount();
	std::vector<uint32_t> boneIndices(lClusterCount);

	mesh->m_boneNames.resize(lClusterCount);
	
	float scale = m_fileScale * m_globalScale;
	
	for (int lClusterIndex = 0; lClusterIndex != lClusterCount; ++lClusterIndex)
	{
		FbxCluster* lCluster= lSkinDeformer->GetCluster(lClusterIndex);
		int lIndexCount = lCluster->GetControlPointIndicesCount();
		int* lIndices = lCluster->GetControlPointIndices();
		double* lWeights = lCluster->GetControlPointWeights();
		
		auto & boneToIndex = m_model.m_avatar->m_boneToIndex;
		std::string boneName = (char *) lCluster->GetLink()->GetName();
		mesh->m_boneNames[lClusterIndex] = boneName;

		//fbxsdk::FbxAMatrix bindPoseMatrix;
		//lCluster->GetTransformLinkMatrix(bindPoseMatrix);	// this bind pose is in world(global) space
		////m_model.m_bindposes.push_back(FbxAMatrixToMatrix4x4(bindPoseMatrix));

		int boneId;
		auto it = boneToIndex.find(boneName);
		if ( it != boneToIndex.end() )
		{
			boneId = it->second;
		}
		else
		{
			// bone not in skeletion;
			abort();
			//boneId = m_boneCount;
			//m_model.m_avatar->m_boneToIndex[boneName] = boneId;
			//m_model.m_avatar->m_indexToBone[boneId] = boneName;
			////fbxsdk::FbxAMatrix transformMatrix;
			////lCluster->GetTransformMatrix(transformMatrix);
			//m_boneCount++;
		}

		fbxsdk::FbxAMatrix bindPoseMatrix;
		lCluster->GetTransformLinkMatrix(bindPoseMatrix);	// this bind pose is in world(global) space
		auto mat = FBXToNativeType(bindPoseMatrix);
		mat.m[0][3] *= scale;
		mat.m[1][3] *= scale;
		mat.m[2][3] *= scale;
		m_model.m_bindposes[boneId] = mat;
		
		boneIndices[lClusterIndex] = boneId;
		
		for ( int k = 0; k < lIndexCount; k++ )
		{
			int vertexId = lIndices[k];
			float weight = static_cast<float>(lWeights[k]);
			mesh->m_boneWeights[vertexId].AddBoneData(lClusterIndex, weight);
			auto it = vertexIndexRemapping.find(vertexId);
			while (it != vertexIndexRemapping.end())
			{
				vertexId = it->second;
				mesh->m_boneWeights[vertexId].AddBoneData(lClusterIndex, weight);
				it = vertexIndexRemapping.find(vertexId);
			}
		}
	}
	m_model.m_boneIndicesForEachMesh.emplace(mesh, std::move(boneIndices));
}

void FishEditor::FBXImporter::ImportSkeleton(fbxsdk::FbxScene* scene)
{
	std::deque<FbxNode*> todo;
	todo.push_back(scene->GetRootNode());
	while (!todo.empty())
	{
		FbxNode* node = todo.front();
		todo.pop_front();
		for (int i = 0; i < node->GetChildCount(); ++i)
		{
			auto child = node->GetChild(i);
			todo.push_back(child);
		}

		const char* nodeName = node->GetName();
		auto nodeAttributeCount = node->GetNodeAttributeCount();
		for (int i = 0; i < nodeAttributeCount; ++i)
		{
			auto nodeAttribute = node->GetNodeAttributeByIndex(i);
			auto type = nodeAttribute->GetAttributeType();
			if (type == FbxNodeAttribute::eSkeleton)
			{
				//FbxSkeleton* lSkeleton = (FbxSkeleton*)nodeAttribute;
				std::string boneName = nodeName;
				auto it = m_model.m_avatar->m_boneToIndex.find(boneName);
				if (it != m_model.m_avatar->m_boneToIndex.end())
				{
					// handle same name
					abort();
				}
				m_model.m_avatar->m_boneToIndex[boneName] = m_boneCount;
				m_model.m_avatar->m_indexToBone[m_boneCount] = boneName;
				m_boneCount++;
			}
		}
	}

	m_model.m_bindposes.resize(m_boneCount);
}

void FishEditor::FBXImporter::UpdateBones(FishEngine::TransformPtr const & node)
{
	auto boneName = node->gameObject()->name();
	auto & boneToIndex = m_model.m_avatar->m_boneToIndex;
	auto it = boneToIndex.find(boneName);
	if (it != boneToIndex.end())
	{
		int id = it->second;
		m_model.m_bones[id] = node;

		// world(global) space -> parent's local space
		if (node->parent() == nullptr)
		{
			node->setLocalToWorldMatrix(m_model.m_bindposes[id]);
		}
		else
		{
			auto l2w = node->parent()->worldToLocalMatrix() * m_model.m_bindposes[id];
			//bindPoses[id] = l2w.inverse();
			node->setLocalToWorldMatrix(l2w);
			m_model.m_bindposes[id] = node->worldToLocalMatrix();
		}
	}

	for (auto & child : node->children())
	{
		UpdateBones(child);
	}
}


MeshPtr FishEditor::FBXImporter::ParseMesh(FbxMesh* fbxMesh)
{
	assert(fbxMesh->IsTriangleMesh());

	auto it = m_model.m_fbxMeshLookup.find(fbxMesh);
	if (it != m_model.m_fbxMeshLookup.end())
	{
		auto meshIndex = it->second;
		return m_model.m_meshes[meshIndex];
	}

	fbxMesh->RemoveBadPolygons();
	fbxMesh->GenerateNormals(false, true, false);
	fbxMesh->GenerateTangentsDataForAllUVSets();
	//auto name1 = fbxMesh->GetName();
	//auto name2 = fbxMesh->GetNode()->GetName();

	// http://help.autodesk.com/view/FBX/2017/ENU/?guid=__cpp_ref_class_fbx_mesh_html
	// A control point is an XYZ coordinate, it is synonym of vertex.
	// A polygon vertex is an index to a control point(the same control point can be referenced by multiple polygon vertices).
	// A polygon is a group of polygon vertices.The minimum valid number of polygon vertices to define a polygon is 3.

	int polygonCount = fbxMesh->GetPolygonCount();
	int vertexCount = fbxMesh->GetControlPointsCount();
	FbxVector4* controlPoints = fbxMesh->GetControlPoints();

//	auto attributeCount = fbxMesh->GetNode()->GetNodeAttributeCount();
//	std::vector<FbxMesh*> submeshes;
//	for (int attributeIndex = 1; attributeIndex < attributeCount; ++attributeIndex)
//	{
//		auto nodeAttribute = fbxMesh->GetNode()->GetNodeAttributeByIndex(attributeIndex);
//		if (nodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh)
//		{
//			FbxMesh* lMesh = (FbxMesh*)nodeAttribute;
//			submeshes.push_back(lMesh);
//			int submeshPolygonCount = lMesh->GetPolygonCount();
//			int submeshVertexCount = lMesh->GetControlPointsCount();
//		}
//	}


	if (fbxMesh->GetElementUVCount() == 0)
	{
		abort();
	}

	// get material info
	int lMaterialCount = fbxMesh->GetNode()->GetMaterialCount();
	if (lMaterialCount == 0)
	{
		// no material from fbx, set default material
		abort();
	}

	// use RawMesh to construct Mesh
	RawMesh rawMesh;

	rawMesh.SetFaceCount(polygonCount);
	rawMesh.SetVertexCount(vertexCount);


	int subMeshCount = lMaterialCount;
	int hasSubMesh = subMeshCount > 1;

	// TODO:
	// if this mesh only has one material, we assume this material applies to all polygons.
	// Better choice is to apply default material to polygons without materials.

	// use material info to split submeshes
	
	//std::vector<std::vector<int>> submeshPolygonIds;
	if (hasSubMesh)
	{
		rawMesh.m_subMeshCount = subMeshCount;
		rawMesh.m_submeshMap.resize(polygonCount);
		//rawMesh.m_submeshPolygonIds.resize(subMeshCount);
		//Debug::LogError("eByPolygon mapping material");
		//auto lMaterialElement = fbxMesh->GetElementMaterial(l);
		for (int l = 0; l < fbxMesh->GetElementMaterialCount(); l++)
		{
			auto lMaterialElement = fbxMesh->GetElementMaterial(l);
			if (lMaterialElement->GetMappingMode() == FbxLayerElement::eByPolygon)
			{
				//int lMatId = lMaterialElement->GetIndexArray().GetAt(0);
				//if (lMatId >= 0)
				//{
				//	auto lMaterial = fbxMesh->GetNode()->GetMaterial(lMatId);
				//	ParseMaterial(lMaterial);
				//}
				//else
				//{
				//	abort();
				//}

				if (lMaterialElement->GetReferenceMode() == FbxLayerElement::eIndex ||
					lMaterialElement->GetReferenceMode() == FbxLayerElement::eIndexToDirect)
				{
					int lIndexArrayCount = lMaterialElement->GetIndexArray().GetCount();
					if (lIndexArrayCount != polygonCount)
					{
						abort();
					}
					for (int i = 0; i < polygonCount; i++)
					{
						int lMatId = lMaterialElement->GetIndexArray().GetAt(i);
						rawMesh.m_submeshMap[i] = lMatId;
						//rawMesh.m_submeshPolygonIds[lMatId].push_back(i);
					}
				}
				else
				{
					abort();
				}
			}
			else
			{
				abort();
			}
		}
	}

	
	// positions
	for (int controlPointIndex = 0; controlPointIndex < vertexCount; ++controlPointIndex)
	{
		auto & p = controlPoints[controlPointIndex];
		float scale = m_fileScale * m_globalScale;
		float x = p[0] * scale;
		float y = p[1] * scale;
		float z = p[2] * scale;
		rawMesh.m_vertexPositions.emplace_back(x, y, z);
	}
	
	// indices(triangles)
	for (int polygonIndex = 0; polygonIndex < polygonCount; ++polygonIndex)
	{
		rawMesh.m_wedgeIndices.push_back( fbxMesh->GetPolygonVertex(polygonIndex, 0) );
		rawMesh.m_wedgeIndices.push_back( fbxMesh->GetPolygonVertex(polygonIndex, 1) );
		rawMesh.m_wedgeIndices.push_back( fbxMesh->GetPolygonVertex(polygonIndex, 2) );
	}

	int vertexId = 0;
	for (int i = 0; i < polygonCount; i++)	// for each triangle
	{
		int lPolygonSize = fbxMesh->GetPolygonSize(i); // should be 3
		assert(lPolygonSize == 3);
		for (int j = 0; j < lPolygonSize; j++)
		{
			int lControlPointIndex = fbxMesh->GetPolygonVertex(i, j);

			// UV
//			for (int l = 0; l < fbxMesh->GetElementUVCount(); ++l)
//			{
				FbxGeometryElementUV* leUV = fbxMesh->GetElementUV(0);
				int lTextureUVIndex = fbxMesh->GetTextureUVIndex(i, j);
				if (leUV->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
				{
					auto mode = leUV->GetReferenceMode();
					if (mode == FbxGeometryElement::eDirect || mode == FbxGeometryElement::eIndexToDirect)
					{
						auto uv = leUV->GetDirectArray().GetAt(lTextureUVIndex);
						rawMesh.m_wedgeTexCoords.emplace_back((float)uv[0], (float)uv[1]);
					}
					else
					{
						abort();
					}
				}
				else
				{
					abort();
				}
//			}

			// Normal
			//for (int l = 0; l < fbxMesh->GetElementNormalCount(); ++l)
			//{
				FbxGeometryElementNormal* leNormal = fbxMesh->GetElementNormal(0);
				//FBXSDK_sprintf(header, 100, "            Normal: ");

				if (leNormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
				{
					auto mode = leNormal->GetReferenceMode();
					if (mode == FbxGeometryElement::eDirect)
					{
						//Display3DVector(header, leNormal->GetDirectArray().GetAt(vertexId));
						auto n = leNormal->GetDirectArray().GetAt(vertexId);
						rawMesh.m_wedgeNormals.emplace_back((float)n[0], (float)n[1], (float)n[2]);
					}
					else if (mode == FbxGeometryElement::eIndexToDirect)
					{
						int id = leNormal->GetIndexArray().GetAt(vertexId);
						//Display3DVector(header, leNormal->GetDirectArray().GetAt(id));
						auto n = leNormal->GetDirectArray().GetAt(id);
						rawMesh.m_wedgeNormals.emplace_back((float)n[0], (float)n[1], (float)n[2]);
					}
					else
					{
						abort();
					}
				}
				else if (leNormal->GetMappingMode() == FbxGeometryElement::eByControlPoint)
				{
					auto mode = leNormal->GetReferenceMode();
					if (mode == FbxGeometryElement::eDirect)
					{
						auto n = leNormal->GetDirectArray().GetAt(lControlPointIndex);
						rawMesh.m_wedgeNormals.emplace_back((float)n[0], (float)n[1], (float)n[2]);
					}
					else if (mode == FbxGeometryElement::eIndexToDirect)
					{
						int id = leNormal->GetIndexArray().GetAt(lControlPointIndex);
						auto n = leNormal->GetDirectArray().GetAt(id);
						rawMesh.m_wedgeNormals.emplace_back((float)n[0], (float)n[1], (float)n[2]);
					}
					else
					{
						abort();
					}
				}
				else
				{
					abort();
				}
			//}

			// Tangent
			//for (int l = 0; l < fbxMesh->GetElementTangentCount(); ++l)
			//{
				FbxGeometryElementTangent* leTangent = fbxMesh->GetElementTangent(0);
				//FBXSDK_sprintf(header, 100, "            Tangent: ");

				if (leTangent->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
				{
					auto mode = leTangent->GetReferenceMode();
					if (mode == FbxGeometryElement::eDirect)
					{
						//Display3DVector(header, leTangent->GetDirectArray().GetAt(vertexId));
						auto t = leTangent->GetDirectArray().GetAt(vertexId);
						rawMesh.m_wedgeTangents.emplace_back((float)t[0], (float)t[1], (float)t[2]);
					}
					else if (mode == FbxGeometryElement::eIndexToDirect)
					{
						int id = leTangent->GetIndexArray().GetAt(vertexId);
						//Display3DVector(header, leTangent->GetDirectArray().GetAt(id));
						auto t = leTangent->GetDirectArray().GetAt(id);
						rawMesh.m_wedgeTangents.emplace_back((float)t[0], (float)t[1], (float)t[2]);
					}
					else
					{
						abort();
					}
				}
				else if (leTangent->GetMappingMode() == FbxGeometryElement::eByControlPoint)
				{
					auto mode = leTangent->GetReferenceMode();
					if (mode == FbxGeometryElement::eDirect)
					{
						auto t = leTangent->GetDirectArray().GetAt(lControlPointIndex);
						rawMesh.m_wedgeTangents.emplace_back((float)t[0], (float)t[1], (float)t[2]);
					}
					else if (mode == FbxGeometryElement::eIndexToDirect)
					{
						int id = leTangent->GetIndexArray().GetAt(lControlPointIndex);
						auto t = leTangent->GetDirectArray().GetAt(id);
						rawMesh.m_wedgeTangents.emplace_back((float)t[0], (float)t[1], (float)t[2]);
					}
					else
					{
						abort();
					}
				}
				else
				{
					abort();
				}
			//}

#if 0
			// Binormal
			for (int l = 0; l < fbxMesh->GetElementBinormalCount(); ++l)
			{

				FbxGeometryElementBinormal* leBinormal = fbxMesh->GetElementBinormal(l);

				//FBXSDK_sprintf(header, 100, "            Binormal: ");
				if (leBinormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
				{
					switch (leBinormal->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
						//Display3DVector(header, leBinormal->GetDirectArray().GetAt(vertexId));
						break;
					case FbxGeometryElement::eIndexToDirect:
					{
						int id = leBinormal->GetIndexArray().GetAt(vertexId);
						//Display3DVector(header, leBinormal->GetDirectArray().GetAt(id));
					}
					break;
					default:
						break; // other reference modes not shown here!
					}
				}
			}
#endif
			vertexId++;
		} // for polygonSize
	} // for polygonCount
	


	auto mesh = rawMesh.ToMesh();
	GetLinkData(fbxMesh, mesh, rawMesh.m_vertexIndexRemapping);
	
	m_model.m_fbxMeshLookup[fbxMesh] = m_model.m_meshes.size();
	m_model.m_meshes.push_back(mesh);

	return mesh;
}


FishEngine::MaterialPtr FishEditor::FBXImporter::ParseMaterial(fbxsdk::FbxSurfaceMaterial * pMaterial)
{
	auto it = m_model.m_fbxMaterialLookup.find(pMaterial);
	if (it != m_model.m_fbxMaterialLookup.end())
	{
		// already parsed
		return m_model.m_materials[it->second];
	}

	FbxProperty lProperty;
	//Diffuse Textures
	lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);
	//DisplayTextureNames(lProperty, lConnectionString);
	
	std::string diffuseTexturePath;
	int lNbTextures = lProperty.GetSrcObjectCount<FbxTexture>();
	for (int j = 0; j < lNbTextures; ++j)
	{
		FbxTexture* lTexture = lProperty.GetSrcObject<FbxTexture>(j);
		if (lTexture)
		{
			std::string textureName = lTexture->GetName();
			
			auto * lFileTexture = FbxCast<FbxFileTexture>(lTexture);
			if (lFileTexture == nullptr)
			{
				// do not support non-file texture
				//abort();
				continue;
			}
			diffuseTexturePath = lFileTexture->GetFileName();
			//Debug::LogWarning("diffuse texture: %s", texturePath.c_str());
			break;
		}
	}

	MaterialPtr ret_material = Material::defaultMaterial();
	if (!diffuseTexturePath.empty())
	{
		auto textureName = Path(diffuseTexturePath).filename();
		auto texturePath = Application::dataPath() / "textures" / textureName;
		auto diffuseTexture = AssetDatabase::LoadAssetAtPath2<Texture>(texturePath);
		ret_material = Material::InstantiateBuiltinMaterial("Diffuse");
		ret_material->setName(textureName.string());
		if (diffuseTexture != nullptr)
		{
			ret_material->setMainTexture(diffuseTexture);
		}
		else
		{
			LogWarning("Texture not found: " + textureName.string());
			ret_material->setMainTexture(Texture2D::whiteTexture());
		}
	}
	m_model.m_fbxMaterialLookup[pMaterial] = m_model.m_materials.size();
	m_model.m_materials.push_back(ret_material);
	return ret_material;
}


void FishEditor::FBXImporter::BakeTransforms(FbxScene * scene)
{
	// FBX stores transforms in a more complex way than just translation-rotation-scale as used by Banshee.
	// Instead they also support rotations offsets and pivots, scaling pivots and more. We wish to bake all this data
	// into a standard transform so we can access it using node's local TRS properties (e.g. FbxNode::LclTranslation).

	// fbx sdk doc:
	// http://help.autodesk.com/view/FBX/2017/ENU/?guid=__files_GUID_C35D98CB_5148_4B46_82D1_51077D8970EE_htm

	double frameRate = FbxTime::GetFrameRate(scene->GetGlobalSettings().GetTimeMode());

	std::deque<FbxNode*> todo;
	todo.push_back(scene->GetRootNode());

	while (todo.size() > 0)
	{
		FbxNode* node = todo.front();
		todo.pop_front();

		FbxVector4 zero(0, 0, 0);
		FbxVector4 one(1, 1, 1);

		// Activate pivot converting
		node->SetPivotState(FbxNode::eSourcePivot, FbxNode::ePivotActive);
		node->SetPivotState(FbxNode::eDestinationPivot, FbxNode::ePivotActive);

		// We want to set all these to 0 (1 for scale) and bake them into the transforms
		node->SetPostRotation(FbxNode::eDestinationPivot, zero);
		node->SetPreRotation(FbxNode::eDestinationPivot, zero);
		node->SetRotationOffset(FbxNode::eDestinationPivot, zero);
		node->SetScalingOffset(FbxNode::eDestinationPivot, zero);
		node->SetRotationPivot(FbxNode::eDestinationPivot, zero);
		node->SetScalingPivot(FbxNode::eDestinationPivot, zero);
		node->SetGeometricTranslation(FbxNode::eDestinationPivot, zero);
		node->SetGeometricRotation(FbxNode::eDestinationPivot, zero);
		node->SetGeometricScaling(FbxNode::eDestinationPivot, one);

		// FishEngine assumes euler angles are in ZXY order
		// do not use this line, since it will mess up animation curves
		//node->SetRotationOrder(FbxNode::eDestinationPivot, eEulerZXY);
		
		EFbxRotationOrder lRotationOrder;
		node->GetRotationOrder(FbxNode::eSourcePivot , lRotationOrder);
		node->SetRotationOrder(FbxNode::eDestinationPivot , lRotationOrder);

		// Keep interpolation as is
		node->SetQuaternionInterpolation(FbxNode::eDestinationPivot, node->GetQuaternionInterpolation(FbxNode::eSourcePivot));

		for (int i = 0; i < node->GetChildCount(); i++)
		{
			FbxNode* childNode = node->GetChild(i);
			todo.push_back(childNode);
		}
	}

	scene->GetRootNode()->ConvertPivotAnimationRecursive(nullptr, FbxNode::eDestinationPivot, frameRate);
	//scene->GetRootNode()->ResetPivotSetAndConvertAnimation();
}

/**
* Print a node, its attributes, and all its children recursively.
*/
GameObjectPtr FishEditor::FBXImporter::ParseNodeRecursively(FbxNode* pNode)
{
	const char* nodeName = pNode->GetName();
	auto go = GameObject::Create();
	go->setName(nodeName);
	go->setPrefabInternal(m_model.m_modelPrefab);
	go->transform()->setPrefabInternal(m_model.m_modelPrefab);

	FbxDouble3 t = pNode->LclTranslation.Get();
	FbxDouble3 r = pNode->LclRotation.Get();
	FbxDouble3 s = pNode->LclScaling.Get();
	
	float scale = m_fileScale * m_globalScale;
	go->transform()->setLocalPosition(t[0] * scale, t[1] * scale, t[2] * scale);
	go->transform()->setLocalScale(s[0], s[1], s[2]);
	
	EFbxRotationOrder rotationOrder;
	pNode->GetRotationOrder(FbxNode::eSourcePivot, rotationOrder);
	RotationOrder order = FBXToNativeType(rotationOrder);
	Quaternion rot = Quaternion::Euler(order, r[0], r[1], r[2]);
	go->transform()->setLocalRotation(rot);
	
	m_model.m_fbxNodeLookup[pNode] = go->transform();
	FishEditor::AssetDatabase::s_allAssetObjects.insert(go);

	auto nodeAttributeCount = pNode->GetNodeAttributeCount();
	for (int i = 0; i < nodeAttributeCount; ++i)
	{
		auto nodeAttribute = pNode->GetNodeAttributeByIndex(i);
		auto type = nodeAttribute->GetAttributeType();
		if (type == FbxNodeAttribute::eMesh)
		{
			FbxMesh* lMesh = (FbxMesh*)nodeAttribute;
			auto mesh = ParseMesh(lMesh);
			m_model.m_meshes.push_back(mesh);
			if (mesh->name().empty())
			{
				mesh->setName(nodeName);
			}
			
			if (IsNewlyCreated())
			{
				m_recycleNameToFileID[mesh->name()] = m_nextMeshFileID;
				m_fileIDToRecycleName[m_nextMeshFileID] = mesh->name();
				m_nextMeshFileID += 2;
			}
			
			int lMaterialCount = pNode->GetMaterialCount();
			MaterialPtr material;
			if (lMaterialCount == 0)
			{
				material = Material::defaultMaterial();
			}
			else
			{
				auto lMaterial = pNode->GetMaterial(0);
				material = ParseMaterial(lMaterial);
			}
			
			RendererPtr renderer;
			if (mesh->m_skinned)
			{
				auto srenderer = go->AddComponent<SkinnedMeshRenderer>();
				m_model.m_skinnedMeshRenderers.push_back(srenderer);
				srenderer->SetMaterial(material);
				srenderer->setSharedMesh(mesh);
				srenderer->setAvatar(m_model.m_avatar);
				srenderer->setRootBone(m_model.m_rootNode->transform());
				renderer = srenderer;
			}
			else
			{
				go->AddComponent<MeshFilter>()->SetMesh(mesh);
				renderer = go->AddComponent<MeshRenderer>();
				renderer->SetMaterial(material);
			}
			
			for (int i = 1; i < lMaterialCount; ++i)
			{
				auto lMaterial = pNode->GetMaterial(i);
				material = ParseMaterial(lMaterial);
				renderer->AddMaterial(material);
			}
		}
	}


	// Recursively print the children.
	for (int j = 0; j < pNode->GetChildCount(); j++)
	{
		auto child = ParseNodeRecursively(pNode->GetChild(j));
		child->transform()->SetParent(go->transform(), false);
	}
	
	//boost::algorithm::is_iless compare;

	go->transform()->children().sort([](std::weak_ptr<Transform> const & lhs, std::weak_ptr<Transform> const & rhs) {
		return boost::algorithm::to_lower_copy(lhs.lock()->name()) < boost::algorithm::to_lower_copy(rhs.lock()->name());
	});
	
	return go;
}


void FishEditor::FBXImporter::ImportTo(FishEngine::GameObjectPtr & model)
{
	abort();
}

void FishEditor::FBXImporter::Reimport()
{
	Load(m_assetPath);
}


PrefabPtr FishEditor::FBXImporter::Load(FishEngine::Path const & path)
{
	if (m_model.m_modelPrefab == nullptr)
	{
		m_model.m_modelPrefab = std::make_shared<Prefab>();
		m_model.m_modelPrefab->setIsPrefabParent(true);
	}
	if (m_model.m_avatar == nullptr)
	{
		m_model.m_avatar = std::make_shared<Avatar>();
	}
	
	// http://help.autodesk.com/view/FBX/2017/ENU/?guid=__files_GUID_29C09995_47A9_4B49_9535_2F6BDC5C4107_htm
	
	// Initialize the SDK manager. This object handles memory management.
	FbxManager * lSdkManager = FbxManager::Create();

	// Create the IO settings object.
	FbxIOSettings * ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
	lSdkManager->SetIOSettings(ios);

	// Create an importer using the SDK manager.
	FbxImporter* lImporter = FbxImporter::Create(lSdkManager, "");

	// Use the first argument as the filename for the importer.
	if (!lImporter->Initialize(path.string().c_str(), -1, lSdkManager->GetIOSettings()))
	{
		LogError("Call to FbxImporter::Initialize() failed.");
		LogError(std::string( "Error returned: ") + lImporter->GetStatus().GetErrorString() );
		abort();
	}

	auto name = path.stem().string();

	// Create a new scene so that it can be populated by the imported file.
	FbxScene * lScene = FbxScene::Create(lSdkManager, name.c_str());

	// Import the contents of the file into the scene.
	lImporter->Import(lScene);

	// The file is imported, so get rid of the importer.
	lImporter->Destroy();


	
	//FbxAxisSystem::MayaYUp.ConvertScene(lScene);
	//FbxAxisSystem::DirectX.ConvertScene(lScene); // wrong!
	// FbxSystemUnit::m.ConvertScene(lScene);
	// http://help.autodesk.com/view/FBX/2017/ENU/?guid=__files_GUID_CC93340E_C4A1_49EE_B048_E898F856CFBF_htm
	// do NOT use FbxSystemUnit::ConvertScene(lScene), which just simply set transform.scale of root nodes.
	if ( lScene->GetGlobalSettings().GetSystemUnit() == FbxSystemUnit::mm )
	{
		m_fileScale = 0.001f;
	}
	else if ( lScene->GetGlobalSettings().GetSystemUnit() == FbxSystemUnit::dm)
	{
		m_fileScale = 0.1f;
	}
	else if (lScene->GetGlobalSettings().GetSystemUnit() == FbxSystemUnit::m)
	{
		m_fileScale = 1.0f;
	}
	else if (lScene->GetGlobalSettings().GetSystemUnit() == FbxSystemUnit::cm)
	{
		m_fileScale = 0.01f;
	}
	else
	{
		abort();
	}
//	auto sceneSystemUnit = lScene->GetGlobalSettings().GetSystemUnit();
//	if (sceneSystemUnit.GetScaleFactor() != 1.0f)
//	{
//		FbxSystemUnit::m.ConvertScene(lScene);
//	}
	
	FbxGeometryConverter converter(lSdkManager);
	converter.Triangulate(lScene, true);
	//converter.SplitMeshesPerMaterial(lScene, true);

	BakeTransforms(lScene);

	ImportSkeleton(lScene);

	// Print the nodes of the scene and their attributes recursively.
	// Note that we are not printing the root node because it should
	// not contain any attributes.
	FbxNode* lRootNode = lScene->GetRootNode();
	auto & root = m_model.m_rootNode;
	root = FishEngine::GameObject::Create();
	root->setName(name);
	root->setPrefabInternal(m_model.m_modelPrefab);
	root->transform()->setPrefabInternal(m_model.m_modelPrefab);
	AssetDatabase::s_allAssetObjects.insert(root);
	if (lRootNode)
	{
		for (int i = 0; i < lRootNode->GetChildCount(); i++)
		{
			auto child = ParseNodeRecursively(lRootNode->GetChild(i));
			child->transform()->SetParent(root->transform(), false);
		}
	}
	
	ImportAnimations(lScene);

	
	// Destroy the SDK manager and all the other objects it was handling.
	lSdkManager->Destroy();

	m_model.m_bones.resize(m_boneCount);
	UpdateBones(root->transform());

	for (auto & renderer : m_model.m_skinnedMeshRenderers)
	{
		auto mesh = renderer->sharedMesh();
		renderer->bones().reserve(mesh->m_boneNames.size());
		//mesh->m_bindposes = m_model.m_bindposes;
		for (auto & boneName : mesh->m_boneNames)
		{
			int boneId = m_model.m_avatar->m_boneToIndex[boneName];
			renderer->bones().push_back(m_model.m_bones[boneId]);
		}
	}

	for (auto & mesh : m_model.m_meshes)
	{
		AssetImporter::s_objectInstanceIDToPath[mesh->GetInstanceID()] = path;
		if (mesh->m_skinned)
		{
			mesh->m_bindposes.reserve(mesh->m_boneNames.size());
			//mesh->m_bones.reserve(mesh->m_boneNames.size());
			//mesh->m_bindposes = m_model.m_bindposes;
			for (auto & boneName : mesh->m_boneNames)
			{
				int boneId = m_model.m_avatar->m_boneToIndex[boneName];
				mesh->m_bindposes.push_back(m_model.m_bindposes[boneId]);
				//mesh->m_bones.push_back(m_model.m_bones[boneId]);
			}

			// make sure all the weights sum to 1.
			for (auto & bw : mesh->m_boneWeights)
			{
				if (bw.weight[0] > 0)
				{
					float boneWeightScale = 0.0f;
					for (float w : bw.weight)
					{
						boneWeightScale += w;
					}
					if ( Mathf::Abs(boneWeightScale - 1.0f) > 1E-5f)
					{
						boneWeightScale = 1.0f / boneWeightScale;
						for (float & w : bw.weight)
						{
							w *= boneWeightScale;
						}
					}
				}
			}
		}
	}

	//std::deque<TransformPtr> transforms;
	//transforms.push_back(m_model.m_rootNode->transform());
	//while (!transforms.empty())
	//{
	//	auto current = transforms.front();
	//	transforms.pop_front();
	//	for (auto & child : current->children())
	//	{
	//		transforms.push_back(child.lock());
	//	}
	//	
	//	auto srenderer = current->gameObject()->GetComponent<SkinnedMeshRenderer>();
	//	if (srenderer != nullptr)
	//	{
	//		srenderer.
	//	}
	//}

	//for (int i = 0; i < m_boneCount; ++i)
	//{
	//	Debug::LogWarning("%s", m_model.m_avatar->m_indexToBone[i].c_str());
	//	PrintMatrix(m_bindPoses[i]);
	//}
	
	//m_model.m_rootNode = root;
	if (m_model.m_animationClips.size() > 0)
	{
		auto animation = root->AddComponent<Animation>();
		animation->m_clip = m_model.m_animationClips.front();
	}
	
	m_model.m_modelPrefab->setRootGameObject(root);
	
	if (IsNewlyCreated())
	{
		BuildFileIDToRecycleName();
	}
	
	m_asset->Add(m_model.m_modelPrefab->rootGameObject());
	for (auto & mesh : m_model.m_meshes)
		m_asset->Add(mesh);

	for (auto & clip : m_model.m_animationClips)
		m_asset->Add(clip);
	m_asset->Add(m_model.m_avatar);

	return m_model.m_modelPrefab;
}


int IncreaseMapValue(std::map<int, int> & dict, int key)
{
	auto it = dict.find(key);
	if (it == dict.end())
	{
		dict[key] = 1;
		return 0;
	}
	int ret = it->second;
	it->second++;
	return ret;
}

void FishEditor::FBXImporter::RecursivelyBuildFileIDToRecycleName(FishEngine::TransformPtr const & transform)
{
	auto gameObject = transform->gameObject();
	{
		constexpr int classID = FishEngine::ClassID<GameObject>();
		int id = IncreaseMapValue(m_classIDCount, classID);
		id = classID * 100000 + id*2;
		m_fileIDToRecycleName[id] = gameObject->name();
	}
	{
		constexpr int classID = FishEngine::ClassID<Transform>();
		int id = IncreaseMapValue(m_classIDCount, classID);
		id = classID * 100000 + id*2;
		m_fileIDToRecycleName[id] = gameObject->name();
	}
	
	
	for (auto & component : gameObject->Components())
	{
		int classID = component->ClassID();
		int id = IncreaseMapValue(m_classIDCount, classID);
		id = classID * 100000 + id*2;
		m_fileIDToRecycleName[id] = component->name();
	}
	
	for (auto & child : transform->children())
	{
		RecursivelyBuildFileIDToRecycleName(child);
	}
}


void FishEditor::FBXImporter::BuildFileIDToRecycleName()
{
	RecursivelyBuildFileIDToRecycleName(m_model.m_modelPrefab->rootGameObject()->transform());
}


AnimationClipPtr FishEditor::FBXImporter::ConvertAnimationClip(const FBXAnimationClip& fbxClip)
{
	auto result = std::make_shared<AnimationClip>();
	result->setName(fbxClip.name);
	result->frameRate = fbxClip.sampleRate;
	result->length = fbxClip.end - fbxClip.start;

	auto IsBone = [this](std::string const& name) {
		auto it = m_model.m_avatar->m_boneToIndex.find(name);
		return it != m_model.m_avatar->m_boneToIndex.end();
	};

	for (auto & animation : fbxClip.boneAnimations)
	{
		// TODO: move
		//std::string path = "";
		auto node = animation.node;
		std::string path = node->name();
		node = node->parent();
		while (node != nullptr)
		{
			if (!IsBone(node->name()))
				break;
			path = node->name() + "/" + path;
			node = node->parent();
		}
		if (animation.translation.keyframeCount() > 0)
			result->m_positionCurve.emplace_back(Vector3Curve{ path, animation.translation });
		if (animation.rotation.keyframeCount() > 0)
			result->m_rotationCurves.emplace_back(QuaternionCurve{ path, animation.rotation });
//		if (animation.eulers.keyframeCount() > 0)
//			result->m_eulersCurves.emplace_back(Vector3Curve{ path, animation.eulers});
		if (animation.scale.keyframeCount() > 0)
			result->m_scaleCurves.emplace_back(Vector3Curve{ path, animation.scale });
	}
	return result;
}

void FishEditor::FBXImporter::ImportAnimations(fbxsdk::FbxScene* scene)
{
	FbxNode * root = scene->GetRootNode();
	int numAnimStacks = scene->GetSrcObjectCount<FbxAnimStack>();
	for (int i = 0; i < numAnimStacks; ++i)
	{
		FbxAnimStack* animStack = scene->GetSrcObject<FbxAnimStack>(i);

		m_model.m_clips.emplace_back();
		auto & clip = m_model.m_clips.back();
		clip.name = animStack->GetName();
		FbxTimeSpan timeSpan = animStack->GetLocalTimeSpan();
		clip.start = (float)timeSpan.GetStart().GetSecondDouble();
		clip.end = (float)timeSpan.GetStop().GetSecondDouble();
		clip.sampleRate = (uint32_t)FbxTime::GetFrameRate(scene->GetGlobalSettings().GetTimeMode());

		int layerCount = animStack->GetMemberCount<FbxAnimLayer>();
		if (layerCount == 1)
		{
			FbxAnimLayer* animLayer = animStack->GetMember<FbxAnimLayer>(0);
			ImportAnimations(animLayer, root, clip);
		}
		else
		{
			abort();
		}
	}

	for (auto clip : m_model.m_clips)
	{
		auto animationClip = ConvertAnimationClip(clip);
		m_model.m_animationClips.push_back(animationClip);
		animationClip->m_avatar = m_model.m_avatar;
	}
}


TAnimationCurve<Vector3> reduceKeyframes(TAnimationCurve<Vector3>& curve)
{
	uint32_t keyCount = (uint32_t)curve.keyframeCount();

	std::vector<TKeyframe<Vector3>> newKeyframes;

	bool lastWasEqual = false;
	for (uint32_t i = 0; i < keyCount; i++)
	{
		bool isEqual = true;

		const TKeyframe<Vector3>& curKey = curve.keyframeAt(i);
		if (i > 0)
		{
			TKeyframe<Vector3>& prevKey = newKeyframes.back();

			isEqual = isEqual && (prevKey.value == curKey.value) && (prevKey.outTangent == curKey.inTangent);
		}
		else
			isEqual = false;

		// More than two keys in a row are equal, remove previous key by replacing it with this one
		if (lastWasEqual && isEqual)
		{
			TKeyframe<Vector3>& prevKey = newKeyframes.back();

			// Other properties are guaranteed unchanged
			prevKey.time = curKey.time;
			prevKey.outTangent = curKey.outTangent;

			continue;
		}

		newKeyframes.push_back(curKey);
		lastWasEqual = isEqual;
	}

	return TAnimationCurve<Vector3>(newKeyframes);
}


void SetKeyframeValues(TKeyframe<Vector3>& keyFrame, int idx, float value, float inTangent, float outTangent)
{
	keyFrame.value[idx] = value;
	keyFrame.inTangent[idx] = inTangent;
	keyFrame.outTangent[idx] = outTangent;
}


template<class T, int C>
TAnimationCurve<T> ImportCurve(FbxAnimCurve*(&fbxCurve)[C], float(&defaultValues)[C], float start, float end)
{
	int keyCounts[C];
	for (int i = 0; i < C; i++)
	{
		if (fbxCurve[i] != nullptr)
			keyCounts[i] = fbxCurve[i]->KeyGetCount();
		else
			keyCounts[i] = 0;
	}

	// If curve key-counts don't match, we need to force resampling 
	bool forceResample = false;
	if (!forceResample)
	{
		for (int i = 1; i < C; i++)
		{
			forceResample |= keyCounts[i - 1] != keyCounts[i];
			if (forceResample)
				break;
		}
	}

	// Read keys directly
	if (!forceResample)
	{
		bool foundMismatch = false;
		int keyCount = keyCounts[0];
		std::vector<TKeyframe<T>> keyframes;
		for (int i = 0; i < keyCount; i++)
		{
			FbxTime fbxTime = fbxCurve[0]->KeyGetTime(i);
			float time = (float)fbxTime.GetSecondDouble();

			// Ensure times from other curves match
			for (int j = 1; j < C; j++)
			{
				fbxTime = fbxCurve[j]->KeyGetTime(i);
				float otherTime = (float)fbxTime.GetSecondDouble();

				if (!Mathf::CompareApproximately(time, otherTime))
				{
					foundMismatch = true;
					break;
				}
			}

			if (foundMismatch)
				break;

			if (time < start || time > end)
				continue;

			keyframes.push_back(TKeyframe<T>());
			TKeyframe<T>& keyFrame = keyframes.back();

			keyFrame.time = time;

			for (int j = 0; j < C; j++)
			{
				SetKeyframeValues(keyFrame, j,
					fbxCurve[j]->KeyGetValue(i),
					fbxCurve[j]->KeyGetLeftDerivative(i),
					fbxCurve[j]->KeyGetRightDerivative(i));
			}
		}

		if (!foundMismatch)
			return TAnimationCurve<T>(keyframes);
		else
			forceResample = true;
	}

	//if (!importOptions.animResample && forceResample)
	//	LOGWRN("Animation has different keyframes for different curve components, forcing resampling.");


	// Resample keys
	float curveStart = std::numeric_limits<float>::infinity();
	float curveEnd = -std::numeric_limits<float>::infinity();

	for (uint32_t i = 0; i < C; i++)
	{
		if (fbxCurve[i] == nullptr)
		{
			curveStart = std::min(0.0f, curveStart);
			curveEnd = std::max(0.0f, curveEnd);

			continue;
		}

		int keyCount = keyCounts[i];
		for (int j = 0; j < keyCount; j++)
		{
			FbxTime fbxTime = fbxCurve[i]->KeyGetTime(j);
			float time = (float)fbxTime.GetSecondDouble();

			curveStart = std::min(time, curveStart);
			curveEnd = std::max(time, curveEnd);
		}
	}

	curveStart = Mathf::Clamp(curveStart, start, end);
	curveEnd = Mathf::Clamp(curveEnd, start, end);

	float curveLength = curveEnd - curveStart;
	constexpr float animSampleRate = 1.0f / 30.0f;
	int numSamples = Mathf::CeilToInt(curveLength / animSampleRate);

	// We don't use the exact provided sample rate but instead modify it slightly so it
	// completely covers the curve range including start/end points while maintaining
	// constant time step between keyframes.
	float dt = curveLength / (float)numSamples;

	int lastKeyframe[] = { 0, 0, 0 };
	int lastLeftTangent[] = { 0, 0, 0 };
	int lastRightTangent[] = { 0, 0, 0 };

	std::vector<TKeyframe<T>> keyframes(numSamples);
	for (int i = 0; i < numSamples; i++)
	{
		float sampleTime = std::min(curveStart + i * dt, curveEnd);
		FbxTime fbxSampleTime;
		fbxSampleTime.SetSecondDouble(sampleTime);

		TKeyframe<T>& keyFrame = keyframes[i];
		keyFrame.time = sampleTime;

		for (int j = 0; j < C; j++)
		{
			if (fbxCurve[j] != nullptr)
			{
				SetKeyframeValues(keyFrame, j,
					fbxCurve[j]->Evaluate(fbxSampleTime, &lastKeyframe[j]),
					fbxCurve[j]->EvaluateLeftDerivative(fbxSampleTime, &lastLeftTangent[j]),
					fbxCurve[j]->EvaluateRightDerivative(fbxSampleTime, &lastRightTangent[j]));
			}
			else
			{
				SetKeyframeValues(keyFrame, j, defaultValues[C], 0.0f, 0.0f);
			}
		}
	}

	return TAnimationCurve<T>(keyframes);
}


void FishEditor::FBXImporter::ImportAnimations(fbxsdk::FbxAnimLayer* layer, fbxsdk::FbxNode * node, FBXAnimationClip & clip)
{
	FbxAnimCurve* translation[3];
	translation[0] = node->LclTranslation.GetCurve(layer, FBXSDK_CURVENODE_COMPONENT_X);
	translation[1] = node->LclTranslation.GetCurve(layer, FBXSDK_CURVENODE_COMPONENT_Y);
	translation[2] = node->LclTranslation.GetCurve(layer, FBXSDK_CURVENODE_COMPONENT_Z);
	
	FbxAnimCurve* rotation[3];
	rotation[0] = node->LclRotation.GetCurve(layer, FBXSDK_CURVENODE_COMPONENT_X);
	rotation[1] = node->LclRotation.GetCurve(layer, FBXSDK_CURVENODE_COMPONENT_Y);
	rotation[2] = node->LclRotation.GetCurve(layer, FBXSDK_CURVENODE_COMPONENT_Z);
	
	FbxAnimCurve* scale[3];
	scale[0] = node->LclScaling.GetCurve(layer, FBXSDK_CURVENODE_COMPONENT_X);
	scale[1] = node->LclScaling.GetCurve(layer, FBXSDK_CURVENODE_COMPONENT_Y);
	scale[2] = node->LclScaling.GetCurve(layer, FBXSDK_CURVENODE_COMPONENT_Z);
	
	Vector3 defaultTranslation = FBXToNativeType(node->LclTranslation.Get());
	Vector3 defaultRotation = FBXToNativeType(node->LclRotation.Get());
	Vector3 defaultScale = FBXToNativeType(node->LclScaling.Get());
	
	auto hasCurveValues = [](FbxAnimCurve* curves[3])
	{
		for (int i = 0; i < 3; i++)
		{
			if (curves[i] != nullptr && curves[i]->KeyGetCount() > 0)
				return true;
		}
		
		return false;
	};

	EFbxRotationOrder order;
	node->GetRotationOrder(FbxNode::eDestinationPivot, order);
	RotationOrder rotationOrder = FBXToNativeType(order);
	
	bool hasBoneAnimation = hasCurveValues(translation) || hasCurveValues(rotation) || hasCurveValues(scale);
	
	if (order != eEulerXYZ && hasBoneAnimation)
	{
		LogWarning("here");
	}
	
	if (hasBoneAnimation)
	{
		clip.boneAnimations.push_back(FBXBoneAnimation());
		FBXBoneAnimation& boneAnim = clip.boneAnimations.back();
		boneAnim.node = m_model.m_fbxNodeLookup[node];
		
		if (hasCurveValues(translation))
		{
			float defaultValues[3];
			memcpy(defaultValues, &defaultTranslation, sizeof(defaultValues));
			
			boneAnim.translation = ImportCurve<Vector3, 3>(translation, defaultValues,
														   clip.start, clip.end);
		}
		//else
		//{
		//	std::vector<TKeyframe<Vector3>> keyframes(1);
		//	keyframes[0].value = defaultTranslation;
		//	keyframes[0].inTangent = Vector3::zero;
		//	keyframes[0].outTangent = Vector3::zero;
		//	boneAnim.translation = TAnimationCurve<Vector3>(keyframes);
		//}
		
		if (hasCurveValues(scale))
		{
			float defaultValues[3];
			memcpy(defaultValues, &defaultScale, sizeof(defaultValues));
			
			boneAnim.scale = ImportCurve<Vector3, 3>(scale, defaultValues, clip.start, clip.end);
		}
		//else
		//{
		//	std::vector<TKeyframe<Vector3>> keyframes(1);
		//	keyframes[0].value = defaultScale;
		//	keyframes[0].inTangent = Vector3::zero;
		//	keyframes[0].outTangent = Vector3::zero;
		//	boneAnim.scale = TAnimationCurve<Vector3>(keyframes);
		//}
		
		TAnimationCurve<Vector3> eulerAnimation;
		if (hasCurveValues(rotation))
		{
			float defaultValues[3];
			memcpy(defaultValues, &defaultRotation, sizeof(defaultValues));
			
			eulerAnimation = ImportCurve<Vector3, 3>(rotation, defaultValues, clip.start, clip.end);
		}
		//else
		//{
		//	std::vector<TKeyframe<Vector3>> keyframes(1);
		//	keyframes[0].value = defaultRotation;
		//	keyframes[0].inTangent = Vector3::zero;
		//	keyframes[0].outTangent = Vector3::zero;
		//	eulerAnimation = TAnimationCurve<Vector3>(keyframes);
		//}
		
		//if(importOptions.reduceKeyframes)
		//{
		//	boneAnim.translation = reduceKeyframes(boneAnim.translation);
		//	boneAnim.scale = reduceKeyframes(boneAnim.scale);
		//	eulerAnimation = reduceKeyframes(eulerAnimation);
		//}
		
		boneAnim.translation = AnimationCurveUtility::ScaleCurve(boneAnim.translation, m_fileScale * m_globalScale);
		//boneAnim.eulers = eulerAnimation;
		boneAnim.rotation = AnimationCurveUtility::EulerToQuaternionCurve(eulerAnimation, rotationOrder);
		
	}
	
	int childCount = node->GetChildCount();
	for (int i = 0; i < childCount; i++)
	{
		FbxNode* child = node->GetChild(i);
		ImportAnimations(layer, child, clip);
	}
}
