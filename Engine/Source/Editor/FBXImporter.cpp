#include "FBXImporter.hpp"

#include <fbxsdk.h>
#include <fbxsdk/utils/fbxgeometryconverter.h>

#include <Debug.hpp>
#include <GameObject.hpp>
#include "AssetDataBase.hpp"

#include <MeshFilter.hpp>
#include <MeshRenderer.hpp>

using namespace FishEngine;

/* Tab character ("\t") counter */
int numTabs = 0;

/**
* Print the required number of tabs.
*/
void PrintTabs()
{
	for (int i = 0; i < numTabs; i++)
		printf("\t");
}

/**
* Return a string-based representation based on the attribute type.
*/
FbxString GetAttributeTypeName(FbxNodeAttribute::EType type)
{
	switch (type) {
	case FbxNodeAttribute::eUnknown: return "unidentified";
	case FbxNodeAttribute::eNull: return "null";
	case FbxNodeAttribute::eMarker: return "marker";
	case FbxNodeAttribute::eSkeleton: return "skeleton";
	case FbxNodeAttribute::eMesh: return "mesh";
	case FbxNodeAttribute::eNurbs: return "nurbs";
	case FbxNodeAttribute::ePatch: return "patch";
	case FbxNodeAttribute::eCamera: return "camera";
	case FbxNodeAttribute::eCameraStereo: return "stereo";
	case FbxNodeAttribute::eCameraSwitcher: return "camera switcher";
	case FbxNodeAttribute::eLight: return "light";
	case FbxNodeAttribute::eOpticalReference: return "optical reference";
	case FbxNodeAttribute::eOpticalMarker: return "marker";
	case FbxNodeAttribute::eNurbsCurve: return "nurbs curve";
	case FbxNodeAttribute::eTrimNurbsSurface: return "trim nurbs surface";
	case FbxNodeAttribute::eBoundary: return "boundary";
	case FbxNodeAttribute::eNurbsSurface: return "nurbs surface";
	case FbxNodeAttribute::eShape: return "shape";
	case FbxNodeAttribute::eLODGroup: return "lodgroup";
	case FbxNodeAttribute::eSubDiv: return "subdiv";
	default: return "unknown";
	}
}

/**
* Print an attribute.
*/
void PrintAttribute(FbxNodeAttribute* pAttribute)
{
	if (!pAttribute) return;

	FbxString typeName = GetAttributeTypeName(pAttribute->GetAttributeType());
	FbxString attrName = pAttribute->GetName();
	PrintTabs();
	// Note: to retrieve the character array of a FbxString, use its Buffer() method.
	printf("<attribute type='%s' name='%s'/>\n", typeName.Buffer(), attrName.Buffer());
}

MeshPtr MeshFromFbxMesh(FbxMesh* fbxMesh)
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

	std::vector<float>		positionBuffer;
	std::vector<float>		normalBuffer;
	std::vector<float>		uvBuffer;
	std::vector<float>		tangentBuffer;
	std::vector<uint32_t>	indexBuffer(polygonCount * 3);

	if (fbxMesh->GetElementUVCount() == 0)
	{
		abort();
	}
	
	FbxGeometryElementUV* leUV0 = fbxMesh->GetElementUV(0);
	FbxGeometryElementNormal* leNormal0 = fbxMesh->GetElementNormal(0);
	FbxGeometryElementTangent* leTangent0 = fbxMesh->GetElementTangent(0);

	// https://forums.autodesk.com/t5/fbx-forum/split-uv-per-control-point/td-p/4239606?nobounce

	if (leUV0->GetMappingMode() == FbxGeometryElement::eByControlPoint &&
		leNormal0->GetMappingMode() == FbxGeometryElement::eByControlPoint &&
		leTangent0->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		int vertexCount = fbxMesh->GetControlPointsCount();
		positionBuffer.reserve(vertexCount * 3);
		normalBuffer.reserve(vertexCount * 3);
		uvBuffer.reserve(vertexCount * 2);
		tangentBuffer.reserve(vertexCount * 3);
		for (int controlPointIndex = 0; controlPointIndex < vertexCount; ++controlPointIndex)
		{
			// Position
			auto & p = controlPoints[controlPointIndex];
			positionBuffer.push_back(p[0]);
			positionBuffer.push_back(p[1]);
			positionBuffer.push_back(p[2]);

			// UV
			auto mode = leUV0->GetReferenceMode();
			if (mode == FbxGeometryElement::eDirect)
			{
				auto uv = leUV0->GetDirectArray().GetAt(controlPointIndex);
				uvBuffer.push_back(uv[0]);
				uvBuffer.push_back(uv[1]);
			}
			else if (mode == FbxGeometryElement::eIndexToDirect)
			{
				int id = leUV0->GetIndexArray().GetAt(controlPointIndex);
				auto uv = leUV0->GetDirectArray().GetAt(id);
				uvBuffer.push_back(uv[0]);
				uvBuffer.push_back(uv[1]);
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
				normalBuffer.push_back(n[0]);
				normalBuffer.push_back(n[1]);
				normalBuffer.push_back(n[2]);
			}
			else if (mode == FbxGeometryElement::eIndexToDirect)
			{
				int id = leNormal0->GetIndexArray().GetAt(controlPointIndex);
				auto n = leNormal0->GetDirectArray().GetAt(id);
				normalBuffer.push_back(n[0]);
				normalBuffer.push_back(n[1]);
				normalBuffer.push_back(n[2]);
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
				normalBuffer.push_back(t[0]);
				normalBuffer.push_back(t[1]);
				normalBuffer.push_back(t[2]);
			}
			else if (mode == FbxGeometryElement::eIndexToDirect)
			{
				int id = leTangent0->GetIndexArray().GetAt(controlPointIndex);
				auto t = leTangent0->GetDirectArray().GetAt(id);
				normalBuffer.push_back(t[0]);
				normalBuffer.push_back(t[1]);
				normalBuffer.push_back(t[2]);
			}
			else
			{
				abort();
			}

			for (int polygonIndex = 0; polygonIndex < polygonCount; ++polygonCount)
			{
				indexBuffer.push_back( fbxMesh->GetPolygonVertex(polygonIndex, 0) );
				indexBuffer.push_back( fbxMesh->GetPolygonVertex(polygonIndex, 1) );
				indexBuffer.push_back( fbxMesh->GetPolygonVertex(polygonIndex, 2) );
			}
		}
	}
	else
	{
		int vertexCount = polygonCount * 3;
		positionBuffer.reserve(vertexCount * 3);
		normalBuffer.reserve(vertexCount * 3);
		uvBuffer.reserve(vertexCount * 2);
		tangentBuffer.reserve(vertexCount * 3);

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
				auto p = controlPoints[lControlPointIndex];
				positionBuffer.push_back(p[0]);
				positionBuffer.push_back(p[1]);
				positionBuffer.push_back(p[2]);

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
							uvBuffer.push_back(uv[0]);
							uvBuffer.push_back(uv[1]);
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
							normalBuffer.push_back(n[0]);
							normalBuffer.push_back(n[1]);
							normalBuffer.push_back(n[2]);
						}
						else if (mode == FbxGeometryElement::eIndexToDirect)
						{
							int id = leNormal->GetIndexArray().GetAt(vertexId);
							//Display3DVector(header, leNormal->GetDirectArray().GetAt(id));
							auto n = leNormal->GetDirectArray().GetAt(id);
							normalBuffer.push_back(n[0]);
							normalBuffer.push_back(n[1]);
							normalBuffer.push_back(n[2]);
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
							tangentBuffer.push_back(t[0]);
							tangentBuffer.push_back(t[1]);
							tangentBuffer.push_back(t[2]);
						}
						else if (mode == FbxGeometryElement::eIndexToDirect)
						{
							int id = leTangent->GetIndexArray().GetAt(vertexId);
							//Display3DVector(header, leTangent->GetDirectArray().GetAt(id));
							auto t = leTangent->GetDirectArray().GetAt(id);
							tangentBuffer.push_back(t[0]);
							tangentBuffer.push_back(t[1]);
							tangentBuffer.push_back(t[2]);
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
				indexBuffer.push_back(vertexId * 3);
				indexBuffer.push_back(vertexId * 3 + 1);
				indexBuffer.push_back(vertexId * 3 + 2);
				vertexId++;
			} // for polygonSize
		} // for polygonCount
	}

	auto mesh = std::make_shared<Mesh>(std::move(positionBuffer), std::move(normalBuffer), std::move(uvBuffer), std::move(tangentBuffer), std::move(indexBuffer));
	mesh->setName(fbxMesh->GetName());
	return mesh;
}

/**
* Print a node, its attributes, and all its children recursively.
*/
GameObjectPtr PrintNode(FbxNode* pNode)
{
	PrintTabs();
	const char* nodeName = pNode->GetName();
	FbxDouble3 t = pNode->LclTranslation.Get();
	FbxDouble3 r = pNode->LclRotation.Get();
	FbxDouble3 s = pNode->LclScaling.Get();

	auto go = GameObject::Create();
	go->setName(nodeName);
	go->transform()->setLocalPosition(t[0], t[1], t[2]);
	go->transform()->setLocalEulerAngles(r[0], r[1], r[2]);
	go->transform()->setLocalScale(s[0], s[1], s[2]);
	FishEditor::AssetDatabase::s_allAssetObjects.insert(go);

	// Print the contents of the node.
	printf("<node name='%s' translation='(%f, %f, %f)' rotation='(%f, %f, %f)' scaling='(%f, %f, %f)'>\n",
		nodeName,
		t[0], t[1], t[2],
		r[0], r[1], r[2],
		s[0], s[1], s[2]
	);
	numTabs++;

	auto nodeAttribute = pNode->GetNodeAttribute();
	if (nodeAttribute != nullptr)
	{
		if (nodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh)
		{
			FbxMesh* lMesh = (FbxMesh*)nodeAttribute;
			auto mesh = MeshFromFbxMesh(lMesh);
			if (mesh->name().empty())
				mesh->setName(nodeName);
			go->AddComponent<MeshFilter>()->SetMesh(mesh);
			go->AddComponent<MeshRenderer>();
		}
	}

	// Print the node's attributes.
	//for (int i = 0; i < pNode->GetNodeAttributeCount(); i++)
	//{
	//	PrintAttribute(pNode->GetNodeAttributeByIndex(i));
	//	if (pNode->GetNodeAttributeByIndex(i)->GetAttributeType() == FbxNodeAttribute::eMesh)
	//	{
	//		go->AddComponent<MeshFilter>();
	//		go->AddComponent<MeshRenderer>();
	//	}
	//}

	// Recursively print the children.
	for (int j = 0; j < pNode->GetChildCount(); j++)
	{
		auto child = PrintNode(pNode->GetChild(j));
		go->transform()->SetParent(child->transform(), false);
	}

	numTabs--;
	PrintTabs();
	printf("</node>\n");
	return go;
}


GameObjectPtr FishEditor::FBXImporter::Load(boost::filesystem::path const & path)
{
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


	// Print the nodes of the scene and their attributes recursively.
	// Note that we are not printing the root node because it should
	// not contain any attributes.
	FbxNode* lRootNode = lScene->GetRootNode();
	auto root = FishEngine::GameObject::Create();
	root->setName(name);
	AssetDatabase::s_allAssetObjects.insert(root);
	if (lRootNode)
	{
		for (int i = 0; i < lRootNode->GetChildCount(); i++)
		{
			auto child = PrintNode(lRootNode->GetChild(i));
			child->transform()->SetParent(root->transform(), false);
		}
	}
	// Destroy the SDK manager and all the other objects it was handling.
	lSdkManager->Destroy();

	return root;
}
