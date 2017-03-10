#include "SceneArchive.hpp"
#include "AssetImporter.hpp"
#include "AssetDataBase.hpp"

using namespace FishEngine;

void FishEditor::SceneOutputArchive::SerializeObject(FishEngine::ObjectPtr const & obj)
{
	if (obj == nullptr)
	{
		(*this) << nullptr;
		return;
	}

	auto instanceID = obj->GetInstanceID();
	auto classID = obj->ClassID();
	
	auto find_result = m_serialized.find(instanceID);
	bool serialized = false;
	int fileID = -1;
	if (find_result != m_serialized.end()) // has already serialized
	{
		fileID = find_result->second;
		serialized = true;
	}
	else
	{
		fileID = m_nextFileID;
		m_totalCount++;
		m_nextFileID = m_totalCount+1;
		//m_serialized[instanceID] = fileID;
	}

	if (m_isInsideDoc)
	{
		BeginFlow();
		BeginMap();

		if (classID == ClassID<Mesh>())
		{
			auto importer = AssetImporter::GetAtPath(AssetDatabase::GetAssetPath(obj));
			(*this) << FishEngine::make_nvp("fileID", importer->m_recycleNameToFileID[obj->name()]);
			(*this) << FishEngine::make_nvp("guid", importer->GetGUID());
		}
		//else if (classID == ClassID<Material>())
		//{
		//	(*this) << FishEngine::make_nvp("fileID", instanceID);
		//}
		//else if (classID == ClassID<Shader>())
		//{
		//	(*this) << FishEngine::make_nvp("fileID", instanceID);
		//}
		else
		{
			(*this) << FishEngine::make_nvp("fileID", fileID);
		}

		EndMap();
	}
	
	if (serialized)
	{
		return;
	}

	bool isComponent = FishEngine::IsComponent(classID);
	bool isGameobject = FishEngine::IsGameObject(classID);
	if (!(isComponent || isGameobject))
	{
		return;
	}

	if (m_isInsideDoc)
	{
		m_objectsToBeSerialized.push_back({ fileID, obj });
	}
	else
	{
		m_serialized[instanceID] = fileID;
		//AssetOutputArchive::SerializeObject(obj);
		BeginDoc(classID, fileID);
		BeginMap();
		m_emitter << obj->ClassName();
		BeginMap();
		obj->Serialize(*this);
		EndMap();
		EndMap();
		EndDoc();
	}
}
