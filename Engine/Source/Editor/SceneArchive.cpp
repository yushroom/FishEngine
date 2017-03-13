#include "SceneArchive.hpp"
#include "AssetImporter.hpp"
#include "AssetDataBase.hpp"
#include <Prefab.hpp>

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
	bool isPrefab = obj->ClassID() == FishEngine::ClassID<FishEngine::Prefab>();
	if (!(isComponent || isGameobject || isPrefab))
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
	if (object == nullptr)
	{
		SerializeObject_impl(object);
	}
	else
	{
		auto prefab = object->prefabInternal();
		if (prefab == nullptr)
		{
			SerializeObject_impl(object);
		}
		else
		{
			if (m_serializePrefab)
			{
				SerializeObject_impl(object);
			}
			else
			{
				SerializeObject_impl(prefab);
			}
		}
	}
	while (!m_objectsToBeSerialized.empty() && !m_isInsideDoc)
	{
		auto item = m_objectsToBeSerialized.front();
		m_objectsToBeSerialized.pop_front();
		int fileID = item.first;
		auto obj = item.second;
		m_nextFileID = fileID;
		SerializeObject_impl(obj);
	}
}

void FishEditor::SceneInputArchive::LoadAll()
{
	std::list<GameObjectPtr> gameObjects;
	//for (auto & node : m_nodes)
	//{
	//	//Debug::Log(node->)
	//}
}
