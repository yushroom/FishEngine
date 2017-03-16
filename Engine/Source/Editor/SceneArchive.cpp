#include "SceneArchive.hpp"
#include "AssetImporter.hpp"
#include "AssetDataBase.hpp"
#include <Prefab.hpp>

#include <Debug.hpp>

using namespace FishEngine;

void FishEditor::SceneOutputArchive::SerializeObject_impl(FishEngine::ObjectPtr obj)
{
	if (obj == nullptr)
	{
		(*this) << nullptr;
		return;
	}

	if (obj->prefabInternal() != nullptr && obj->prefabInternal()->parentPrefab() != nullptr)
	{
		obj = obj->prefabInternal();
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
		else if (classID == ClassID<Prefab>())
		{
			auto prefab = As<Prefab>(obj);
			if (prefab->parentPrefab() != nullptr)
			{
				prefab = prefab->parentPrefab();
			}
			auto path = AssetDatabase::GetAssetPath(prefab);
			auto importer = AssetImporter::GetAtPath(path);
			(*this) << FishEngine::make_nvp("fileID", "100100000");
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
	bool isInternalPrefab = false;
	if (isPrefab)
	{
		isInternalPrefab = ! FishEngine::As<FishEngine::Prefab>(obj)->isPrefabParent();
	}
	if (!(isComponent || isGameobject || isInternalPrefab))
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
	//if (object == nullptr)
	//{
	//	SerializeObject_impl(object);
	//}
	//else
	//{
	//	auto prefab = object->prefabInternal();
	//	if (prefab == nullptr)
	//	{
	//		SerializeObject_impl(object);
	//	}
	//	else
	//	{
	//		if (m_serializePrefab)
	//		{
	//			SerializeObject_impl(object);
	//		}
	//		else
	//		{
	//			SerializeObject_impl(prefab);
	//		}
	//	}
	//}
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
}

void PrintNode(YAML::Node const & node)
{
	if (node.IsScalar())
	{
		Debug::Log("%s", node.as<std::string>().c_str());
	}
	else if (node.IsSequence())
	{
		for (auto it = node.begin(); it != node.end(); ++it)
		{
			PrintNode(*it);
		}
	}
	else if (node.IsMap())
	{
		for (auto it = node.begin(); it != node.end(); ++it)
		{
			PrintNode(it->first);
			PrintNode(it->second);
		}
	}
}

void FishEditor::SceneInputArchive::LoadAll()
{
	std::list<GameObjectPtr> gameObjects;
	for (auto & node : m_nodes)
	{
		PrintNode(node);
	}
}

void FishEditor::SingleObjectOutputArchive::SerializeObject(FishEngine::ObjectPtr const & object)
{
	if (!m_isInsideDoc)
	{
		m_isInsideDoc = true;
		auto classID = object->ClassID();
		int fileID = classID * 100000;
		BeginDoc(classID, fileID);
		BeginMap(2);
		m_emitter << object->ClassName();
		BeginMap(1);	// do not know map size
		object->Serialize(*this);
		EndMap();
		EndMap();
		EndDoc();
	}
	else
	{
		if (object == nullptr)
		{
			(*this) << nullptr;
		}
		else
		{
			BeginFlow();
			BeginMap(1);
			(*this) << FishEngine::make_nvp("fileID", 1111);
			EndMap();
		}
	}
}
