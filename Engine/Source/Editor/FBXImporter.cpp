#include "FBXImporter.hpp"

#include <fbxsdk.h>
#include <fbxsdk/utils/fbxgeometryconverter.h>

#include <Debug.hpp>
#include <GameObject.hpp>
#include <MeshFilter.hpp>
#include <MeshRenderer.hpp>
#include <SkinnedMeshRenderer.hpp>

#include "AssetDataBase.hpp"
#include "private/RawMesh.hpp"

using namespace FishEngine;


Matrix4x4 FbxAMatrixToMatrix4x4(fbxsdk::FbxAMatrix const & fmatrix)
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
	
//	for ( int lSkinIndex = 0; lSkinIndex < lSkinCount; ++lSkinIndex )
//	{
	FbxSkin * lSkinDeformer = (FbxSkin *)pMesh->GetDeformer(0, FbxDeformer::eSkin);
	FbxSkin::EType lSkinningType = lSkinDeformer->GetSkinningType();

	// cluser == bone
	int lClusterCount = lSkinDeformer->GetClusterCount();
	std::vector<uint32_t> boneIndices(lClusterCount);

	//mesh->m_bindposes.resize(lClusterCount);
	mesh->m_boneNames.resize(lClusterCount);
	
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
			boneId = m_boneCount;
			m_model.m_avatar->m_boneToIndex[boneName] = boneId;
			m_model.m_avatar->m_indexToBone[boneId] = boneName;
			//fbxsdk::FbxAMatrix transformMatrix;
			//lCluster->GetTransformMatrix(transformMatrix);
			fbxsdk::FbxAMatrix bindPoseMatrix;
			lCluster->GetTransformLinkMatrix(bindPoseMatrix);	// this bind pose is in world(global) space
			m_model.m_bindposes.push_back(FbxAMatrixToMatrix4x4(bindPoseMatrix));
			m_boneCount++;
		}
		
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
//	}
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
		UpdateBones(child.lock());
	}
}

MeshPtr FishEditor::FBXImporter::MeshFromFbxMesh(FbxMesh* fbxMesh)
{
	assert(fbxMesh->IsTriangleMesh());
	fbxMesh->GenerateNormals(false, true, false);
	fbxMesh->GenerateTangentsDataForAllUVSets();

	// http://help.autodesk.com/view/FBX/2017/ENU/?guid=__cpp_ref_class_fbx_mesh_html
	// A control point is an XYZ coordinate, it is synonym of vertex.
	// A polygon vertex is an index to a control point(the same control point can be referenced by multiple polygon vertices).
	// A polygon is a group of polygon vertices.The minimum valid number of polygon vertices to define a polygon is 3.

	int polygonCount = fbxMesh->GetPolygonCount();
	FbxVector4* controlPoints = fbxMesh->GetControlPoints();

	if (fbxMesh->GetElementUVCount() == 0)
	{
		abort();
	}

#if 0
	FbxGeometryElementUV* leUV0 = fbxMesh->GetElementUV(0);
	FbxGeometryElementNormal* leNormal0 = fbxMesh->GetElementNormal(0);
	FbxGeometryElementTangent* leTangent0 = fbxMesh->GetElementTangent(0);

	// https://forums.autodesk.com/t5/fbx-forum/split-uv-per-control-point/td-p/4239606?nobounce


	if (leUV0->GetMappingMode() == FbxGeometryElement::eByControlPoint &&
		leNormal0->GetMappingMode() == FbxGeometryElement::eByControlPoint &&
		leTangent0->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		// do not need a RawMesh, generate a Mesh directly
		
		std::vector<Vector3>	positionBuffer;
		std::vector<Vector3>	normalBuffer;
		std::vector<Vector2>	uvBuffer;
		std::vector<Vector3>	tangentBuffer;
		std::vector<uint32_t>	indexBuffer;
		indexBuffer.reserve(polygonCount * 3);
		
		int vertexCount = fbxMesh->GetControlPointsCount();
		positionBuffer.reserve(vertexCount);
		normalBuffer.reserve(vertexCount);
		uvBuffer.reserve(vertexCount);
		tangentBuffer.reserve(vertexCount);
		for (int controlPointIndex = 0; controlPointIndex < vertexCount; ++controlPointIndex)
		{
			// Position
			auto & p = controlPoints[controlPointIndex];
			positionBuffer.emplace_back(p[0], p[1], p[2]);

			// UV
			auto mode = leUV0->GetReferenceMode();
			if (mode == FbxGeometryElement::eDirect)
			{
				auto uv = leUV0->GetDirectArray().GetAt(controlPointIndex);
				uvBuffer.emplace_back(uv[0], uv[1]);
			}
			else if (mode == FbxGeometryElement::eIndexToDirect)
			{
				int id = leUV0->GetIndexArray().GetAt(controlPointIndex);
				auto uv = leUV0->GetDirectArray().GetAt(id);
				uvBuffer.emplace_back(uv[0], uv[1]);
			}
			else
			{
				abort();
			}

			// Normal
			mode = leNormal0->GetReferenceMode();
			if (mode == FbxGeometryElement::eDirect)
			{
				auto n = leNormal0->GetDirectArray().GetAt(controlPointIndex);
				normalBuffer.emplace_back(n[0], n[1], n[2]);
			}
			else if (mode == FbxGeometryElement::eIndexToDirect)
			{
				int id = leNormal0->GetIndexArray().GetAt(controlPointIndex);
				auto n = leNormal0->GetDirectArray().GetAt(id);
				normalBuffer.emplace_back(n[0], n[1], n[2]);
			}
			else
			{
				abort();
			}

			// Tangent
			mode = leTangent0->GetReferenceMode();
			if (mode == FbxGeometryElement::eDirect)
			{
				auto t = leTangent0->GetDirectArray().GetAt(controlPointIndex);
				tangentBuffer.emplace_back(t[0], t[1], t[2]);
			}
			else if (mode == FbxGeometryElement::eIndexToDirect)
			{
				int id = leTangent0->GetIndexArray().GetAt(controlPointIndex);
				auto t = leTangent0->GetDirectArray().GetAt(id);
				tangentBuffer.emplace_back(t[0], t[1], t[2]);
			}
			else
			{
				abort();
			}

			for (int polygonIndex = 0; polygonIndex < polygonCount; ++polygonIndex)
			{
				indexBuffer.push_back( fbxMesh->GetPolygonVertex(polygonIndex, 0) );
				indexBuffer.push_back( fbxMesh->GetPolygonVertex(polygonIndex, 1) );
				indexBuffer.push_back( fbxMesh->GetPolygonVertex(polygonIndex, 2) );
			}
		}
		
		auto mesh = std::make_shared<Mesh>(std::move(positionBuffer), std::move(normalBuffer), std::move(uvBuffer), std::move(tangentBuffer), std::move(indexBuffer));
		mesh->setName(fbxMesh->GetName());
		GetLinkData(fbxMesh, mesh);
		return mesh;
	}
#endif
	
	// use RawMesh to construct Mesh
	
	RawMesh rawMesh;
	
	int vertexCount = fbxMesh->GetControlPointsCount();
	
	rawMesh.SetFaceCount(polygonCount);
	rawMesh.SetVertexCount(vertexCount);
	
	for (int controlPointIndex = 0; controlPointIndex < vertexCount; ++controlPointIndex)
	{
		// Position
		auto & p = controlPoints[controlPointIndex];
		rawMesh.m_vertexPositions.emplace_back((float)p[0], (float)p[1], (float)p[2]);
	}
	
	for (int polygonIndex = 0; polygonIndex < polygonCount; ++polygonIndex)
	{
		rawMesh.m_wedgeIndices.push_back( fbxMesh->GetPolygonVertex(polygonIndex, 0) );
		rawMesh.m_wedgeIndices.push_back( fbxMesh->GetPolygonVertex(polygonIndex, 1) );
		rawMesh.m_wedgeIndices.push_back( fbxMesh->GetPolygonVertex(polygonIndex, 2) );
	}

	int vertexId = 0;
	for (int i = 0; i < polygonCount; i++)
	{
#if 0
		for (int l = 0; l < fbxMesh->GetElementPolygonGroupCount(); l++)
		{
			FbxGeometryElementPolygonGroup* lePolgrp = fbxMesh->GetElementPolygonGroup(l);
			switch (lePolgrp->GetMappingMode())
			{
			case FbxGeometryElement::eByPolygon:
				if (lePolgrp->GetReferenceMode() == FbxGeometryElement::eIndex)
				{
					//FBXSDK_sprintf(header, 100, "        Assigned to group: ");
					int polyGroupId = lePolgrp->GetIndexArray().GetAt(i);
					//DisplayInt(header, polyGroupId);
					break;
				}
			default:
				// any other mapping modes don't make sense
				//DisplayString("        \"unsupported group assignment\"");
				break;
			}
		}
#endif

		int lPolygonSize = fbxMesh->GetPolygonSize(i);
		//assert(lPolygonSize == 3);

		for (int j = 0; j < lPolygonSize; j++)
		{
			int lControlPointIndex = fbxMesh->GetPolygonVertex(i, j);
			//auto p = controlPoints[lControlPointIndex];
			//rawMesh.VertexPositions.emplace_back(p[0], p[1], p[2]);

			//Display3DVector("            Coordinates: ", lControlPoints[lControlPointIndex]);

#if 0
			for (int l = 0; l < fbxMesh->GetElementVertexColorCount(); l++)
			{
				FbxGeometryElementVertexColor* leVtxc = fbxMesh->GetElementVertexColor(l);
				//FBXSDK_sprintf(header, 100, "            Color vertex: ");

				switch (leVtxc->GetMappingMode())
				{
				default:
					break;
				case FbxGeometryElement::eByControlPoint:
					switch (leVtxc->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
						//DisplayColor(header, leVtxc->GetDirectArray().GetAt(lControlPointIndex));
						break;
					case FbxGeometryElement::eIndexToDirect:
					{
						int id = leVtxc->GetIndexArray().GetAt(lControlPointIndex);
						//DisplayColor(header, leVtxc->GetDirectArray().GetAt(id));
					}
					break;
					default:
						break; // other reference modes not shown here!
					}
					break;

				case FbxGeometryElement::eByPolygonVertex:
				{
					switch (leVtxc->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
						//DisplayColor(header, leVtxc->GetDirectArray().GetAt(vertexId));
						break;
					case FbxGeometryElement::eIndexToDirect:
					{
						int id = leVtxc->GetIndexArray().GetAt(vertexId);
						//DisplayColor(header, leVtxc->GetDirectArray().GetAt(id));
					}
					break;
					default:
						break; // other reference modes not shown here!
					}
				}
				break;

				case FbxGeometryElement::eByPolygon: // doesn't make much sense for UVs
				case FbxGeometryElement::eAllSame:   // doesn't make much sense for UVs
				case FbxGeometryElement::eNone:       // doesn't make much sense for UVs
					break;
				}
			}
#endif
			for (int l = 0; l < fbxMesh->GetElementUVCount(); ++l)
			{
				FbxGeometryElementUV* leUV = fbxMesh->GetElementUV(l);
				//FBXSDK_sprintf(header, 100, "            Texture UV: ");

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
			}

			for (int l = 0; l < fbxMesh->GetElementNormalCount(); ++l)
			{
				FbxGeometryElementNormal* leNormal = fbxMesh->GetElementNormal(l);
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
			}

			for (int l = 0; l < fbxMesh->GetElementTangentCount(); ++l)
			{
				FbxGeometryElementTangent* leTangent = fbxMesh->GetElementTangent(l);
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
			}

#if 0
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
			//rawMesh.WedgeIndices.push_back(vertexId);
			//indexBuffer.push_back(vertexId * 3 + 1);
			//indexBuffer.push_back(vertexId * 3 + 2);
			vertexId++;
		} // for polygonSize
	} // for polygonCount
	
	auto mesh = rawMesh.ToMesh();
	
	GetLinkData(fbxMesh, mesh, rawMesh.m_vertexIndexRemapping);
	
	return mesh;
}


void FindAndDisplayTextureInfoByProperty(FbxProperty pProperty, bool& pDisplayHeader, int pMaterialIndex) {

	if (pProperty.IsValid())
	{
		int lTextureCount = pProperty.GetSrcObjectCount<FbxTexture>();

		for (int j = 0; j < lTextureCount; ++j)
		{
			//Here we have to check if it's layeredtextures, or just textures:
			FbxLayeredTexture *lLayeredTexture = pProperty.GetSrcObject<FbxLayeredTexture>(j);
			if (lLayeredTexture)
			{
				Debug::Log("    Layered Texture: %d", j);
				int lNbTextures = lLayeredTexture->GetSrcObjectCount<FbxTexture>();
				for (int k = 0; k < lNbTextures; ++k)
				{
					FbxTexture* lTexture = lLayeredTexture->GetSrcObject<FbxTexture>(k);
					if (lTexture)
					{

						if (pDisplayHeader) {
							//DisplayInt("    Textures connected to Material ", pMaterialIndex);
							pDisplayHeader = false;
						}

						//NOTE the blend mode is ALWAYS on the LayeredTexture and NOT the one on the texture.
						//Why is that?  because one texture can be shared on different layered textures and might
						//have different blend modes.

						FbxLayeredTexture::EBlendMode lBlendMode;
						lLayeredTexture->GetTextureBlendMode(k, lBlendMode);
						//Debug::Log("    Textures for ", pProperty.GetName());
						//Debug::Log("        Texture ", k);
						//DisplayTextureInfo(lTexture, (int)lBlendMode);
					}

				}
			}
			else
			{
				//no layered texture simply get on the property
				FbxTexture* lTexture = pProperty.GetSrcObject<FbxTexture>(j);
				if (lTexture)
				{
					//display connected Material header only at the first time
					if (pDisplayHeader) {
						//DisplayInt("    Textures connected to Material ", pMaterialIndex);
						pDisplayHeader = false;
					}

					//DisplayString("    Textures for ", pProperty.GetName());
					//DisplayInt("        Texture ", j);
					//DisplayTextureInfo(lTexture, -1);
				}
			}
		}
	}//end if pProperty
}


FishEngine::TexturePtr FishEditor::FBXImporter::GetTextureInfo(fbxsdk::FbxGeometry* pGeometry)
{
	int lMaterialIndex;
	FbxProperty lProperty;
	if (pGeometry->GetNode() == nullptr)
	{
		return nullptr;
	}
	int lNbMat = pGeometry->GetNode()->GetSrcObjectCount<FbxSurfaceMaterial>();
	for (lMaterialIndex = 0; lMaterialIndex < lNbMat; lMaterialIndex++) {
		FbxSurfaceMaterial *lMaterial = pGeometry->GetNode()->GetSrcObject<FbxSurfaceMaterial>(lMaterialIndex);
		bool lDisplayHeader = true;

		//go through all the possible textures
		if (lMaterial) {

			int lTextureIndex;
			FBXSDK_FOR_EACH_TEXTURE(lTextureIndex)
			{
				lProperty = lMaterial->FindProperty(FbxLayerElement::sTextureChannelNames[lTextureIndex]);
				FindAndDisplayTextureInfoByProperty(lProperty, lDisplayHeader, lMaterialIndex);
			}

		}//end if(lMaterial)

	}// end for lMaterialIndex
	return nullptr;
}

/**
* Print a node, its attributes, and all its children recursively.
*/
GameObjectPtr FishEditor::FBXImporter::ParseNodeRecursively(FbxNode* pNode)
{
	const char* nodeName = pNode->GetName();
	auto go = GameObject::Create();
	go->setName(nodeName);
	if (IsNewlyCreated())
	{
		m_fileIDToRecycleName[m_nextNodeFileID] = nodeName;
		m_recycleNameToFileID[nodeName] = m_nextNodeFileID;
		m_nextNodeFileID++;
	}
#if 0
	FbxDouble3 t = pNode->LclTranslation.Get();
	FbxDouble3 r = pNode->LclRotation.Get();
	FbxDouble3 s = pNode->LclScaling.Get();
	go->transform()->setLocalPosition(t[0], t[1], t[2]);
	go->transform()->setLocalEulerAngles(r[0], r[1], r[2]);
	go->transform()->setLocalScale(s[0], s[1], s[2]);
#elif 0
	auto fmat = pNode->EvaluateLocalTransform();
	auto mat = FbxAMatrixToMatrix4x4(fmat);
	go->transform()->setLocalToWorldMatrix(mat);
#else
	FbxDouble3 t = pNode->LclTranslation.Get();
	FbxDouble3 r = pNode->LclRotation.Get();
	FbxDouble3 s = pNode->LclScaling.Get();
	EFbxRotationOrder order1;
	pNode->GetRotationOrder(FbxNode::eDestinationPivot, order1);
	
	EFbxRotationOrder order2;
	pNode->GetRotationOrder(FbxNode::eSourcePivot, order2);
	//pNode->SetRotationOrder(FbxNode::eDestinationPivot, EFbxRotationOrder::eOrderZYX);
	//r[0] = -r[0];
	go->transform()->setLocalPosition(t[0], t[1], t[2]);
	//go->transform()->setLocalEulerAngles(r[0], r[1], r[2]);
	go->transform()->setLocalScale(s[0], s[1], s[2]);
	
	auto AngleX = [](float x) {
		return Quaternion::AngleAxis(x, Vector3(1, 0, 0));
	};
	auto AngleY = [](float y) {
		return Quaternion::AngleAxis(y, Vector3(0, 1, 0));
	};
	auto AngleZ = [](float z) {
		return Quaternion::AngleAxis(z, Vector3(0, 0, 1));
	};
	
	if (order1 != EFbxRotationOrder::eOrderXYZ)
	{
		abort();
	}
	
	if (order2 == EFbxRotationOrder::eOrderZXY)
	{
		Quaternion q = AngleY(r[1]) * AngleX(r[0]) * AngleZ(r[2]);
		go->transform()->setLocalRotation(q);
	}
	else if (order2 == EFbxRotationOrder::eOrderXYZ)
	{
		Quaternion q = AngleZ(r[2]) * AngleY(r[1]) * AngleX(r[0]);
		go->transform()->setLocalRotation(q);
	}
	else
	{
		abort();
	}
#endif


	FishEditor::AssetDatabase::s_allAssetObjects.insert(go);

	auto nodeAttribute = pNode->GetNodeAttribute();
	if (nodeAttribute != nullptr)
	{
		auto type = nodeAttribute->GetAttributeType();
		if (type == FbxNodeAttribute::eMesh)
		{
			FbxMesh* lMesh = (FbxMesh*)nodeAttribute;
			auto mesh = MeshFromFbxMesh(lMesh);
			m_model.m_meshes.push_back(mesh);
			if (mesh->name().empty())
			{
				mesh->setName(nodeName);
			}
			
			if (IsNewlyCreated())
			{
				m_recycleNameToFileID[mesh->name()] = m_nextMeshFileID;
				m_fileIDToRecycleName[m_nextMeshFileID] = mesh->name();
				m_nextMeshFileID++;
			}
			
			if (mesh->m_skinned)
			{
				auto srenderer = go->AddComponent<SkinnedMeshRenderer>();
				srenderer->SetMaterial(Material::defaultMaterial());
				srenderer->setSharedMesh(mesh);
				srenderer->setAvatar(m_model.m_avatar);
				srenderer->setRootBone(m_model.m_rootNode->transform());
			}
			else
			{
				go->AddComponent<MeshFilter>()->SetMesh(mesh);
				go->AddComponent<MeshRenderer>()->SetMaterial(Material::defaultMaterial());
			}

			auto texture = GetTextureInfo(lMesh);
		}
//		else if (type == FbxNodeAttribute::eSkeleton)
//		{
//			FbxSkeleton* lSkeleton = (FbxSkeleton*)nodeAttribute;
//			std::string boneName = nodeName;
//			m_model.m_avatar->m_boneToIndex[boneName] = m_boneCount;
//			m_model.m_avatar->m_indexToBone[m_boneCount] = boneName;
//			m_boneCount++;
//		}
	}


	// Recursively print the children.
	for (int j = 0; j < pNode->GetChildCount(); j++)
	{
		auto child = ParseNodeRecursively(pNode->GetChild(j));
		child->transform()->SetParent(go->transform(), false);
	}

	return go;
}


void PrintMatrix(Matrix4x4 const & m)
{
	for (int i = 0; i < 4; ++i)
	{
		Debug::Log("%lf %lf %lf %lf", m.m[i][0], m.m[i][1], m.m[i][2], m.m[i][3]);
	}
}


GameObjectPtr FishEditor::FBXImporter::Load(boost::filesystem::path const & path)
{
	if (m_model.m_avatar == nullptr)
		m_model.m_avatar = std::make_shared<Avatar>();
	
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
		FishEngine::Debug::LogError("Call to FbxImporter::Initialize() failed.");
		FishEngine::Debug::LogError("Error returned: %s", lImporter->GetStatus().GetErrorString());
		abort();
	}

	auto name = path.stem().string();

	// Create a new scene so that it can be populated by the imported file.
	FbxScene * lScene = FbxScene::Create(lSdkManager, name.c_str());

	// Import the contents of the file into the scene.
	lImporter->Import(lScene);

	// The file is imported, so get rid of the importer.
	lImporter->Destroy();

	FbxGeometryConverter converter(lSdkManager);
	converter.Triangulate(lScene, true);
	
	FbxAxisSystem::MayaYUp.ConvertScene(lScene);
	//FbxAxisSystem::DirectX.ConvertScene(lScene);


	// Print the nodes of the scene and their attributes recursively.
	// Note that we are not printing the root node because it should
	// not contain any attributes.
	FbxNode* lRootNode = lScene->GetRootNode();
	auto & root = m_model.m_rootNode;
	root = FishEngine::GameObject::Create();
	root->setName(name);
	AssetDatabase::s_allAssetObjects.insert(root);
	if (lRootNode)
	{
		for (int i = 0; i < lRootNode->GetChildCount(); i++)
		{
			auto child = ParseNodeRecursively(lRootNode->GetChild(i));
			child->transform()->SetParent(root->transform(), false);
		}
	}
	// Destroy the SDK manager and all the other objects it was handling.
	lSdkManager->Destroy();

	m_model.m_bones.resize(m_boneCount);
	UpdateBones(root->transform());

	for (auto & mesh : m_model.m_meshes)
	{
		AssetImporter::s_objectInstanceIDToPath[mesh->GetInstanceID()] = path;
		if (mesh->m_skinned)
		{
			mesh->m_bindposes.reserve(mesh->m_boneNames.size());
			mesh->m_bones.reserve(mesh->m_boneNames.size());
			//mesh->m_bindposes = m_model.m_bindposes;
			for (auto & boneName : mesh->m_boneNames)
			{
				int boneId = m_model.m_avatar->m_boneToIndex[boneName];
				mesh->m_bindposes.push_back(m_model.m_bindposes[boneId]);
				mesh->m_bones.push_back(m_model.m_bones[boneId]);
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
	return root;
}
