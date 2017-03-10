#include "SceneArchive.hpp"
#include "AssetImporter.hpp"
#include "AssetDataBase.hpp"

#include <Debug.hpp>

using namespace FishEngine;

void FishEditor::SceneOutputArchive::SerializeObject_impl(FishEngine::ObjectPtr const & obj)
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
		BeginMap(1);	// do not know map size

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
		BeginMap(2);
		m_emitter << obj->ClassName();
		BeginMap(1);	// do not know map size
		obj->Serialize(*this);
		EndMap();
		EndMap();
		EndDoc();
	}
}

void FishEditor::SceneOutputArchive::SerializeObject(FishEngine::ObjectPtr const & object)
{
	SerializeObject_impl(object);
	while (!m_objectsToBeSerialized.empty() && !m_isInsideDoc)
	{
		auto item = m_objectsToBeSerialized.front();
		m_objectsToBeSerialized.pop_front();
		int fileID = item.first;
		auto obj = item.second;
		m_nextFileID = fileID;
		SerializeObject_impl(obj);
	}
	FishEngine::Debug::LogWarning("here");
}

void FishEditor::SceneInputArchive::LoadAll()
{
	std::list<GameObjectPtr> gameObjects;
	//for (auto & node : m_nodes)
	//{
	//	//Debug::Log(node->)
	//}
}
