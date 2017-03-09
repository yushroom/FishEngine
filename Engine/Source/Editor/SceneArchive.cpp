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
			(*this) << FishEngine::make_nvp("fileID", m_nextFileID);
		}

		EndMap();
	}

	bool isComponent = FishEngine::IsComponent(classID);
	bool isGameobject = FishEngine::IsGameObject(classID);
	if (!(isComponent || isGameobject))
	{
		return;
	}

	auto find_result = m_serialized.find(instanceID);
	if (find_result != m_serialized.end())
	{
		return;
	}

	if (m_isInsideDoc)
	{
		m_objectsToBeSerialized.push_back({ m_nextFileID, obj });
		m_nextFileID++;
	}
	else
	{
		m_serialized.insert(instanceID);
		//AssetOutputArchive::SerializeObject(obj);
		BeginDoc(classID, m_nextFileID);
		BeginMap();
		//m_emitter << "ClassID" << classID;
		//m_emitter << "fileID" << m_nextFileID;
		m_emitter << obj->ClassName();
		BeginMap();
		obj->Serialize(*this);
		EndMap();
		EndMap();
		EndDoc();
	}
}
