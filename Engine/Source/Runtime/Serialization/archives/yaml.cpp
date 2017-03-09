#include "yaml.hpp"
#include "Serialization.hpp"
//#include <fstream>
//#include "AssetDataBase.hpp"

void FishEngine::YAMLOutputArchive::SerializeObject(std::shared_ptr<Object> const & obj)
{
	if (obj == nullptr)
	{
		(*this) << nullptr;
		return;
	}

	//std::cout << "SerializeObject: " << obj->ClassName() << std::endl;

	auto instanceID = obj->GetInstanceID();
	auto find_result = m_serialized.find(instanceID);
	
	if (m_isInsideDoc)
	{
		SetManipulator(YAML::Flow);
		SetManipulator(YAML::BeginMap);
		(*this) << make_nvp("fileID", instanceID);
		if (obj->ClassID() == ClassID<Mesh>())
		{
			//auto instanceID = obj->GetInstanceID();
			//AssetDatabase::GetAssetPath(instanceID);
			(*this) << "GUID" << "xxxxxxxx";
		}
		SetManipulator(YAML::EndMap);
	}
	
	if (find_result != m_serialized.end() && find_result->second)
	{
		return;
	}
	
	if (m_isInsideDoc)
	{
		m_objectsToBeSerialized.push_back(obj);
	}
	else
	{
		m_serialized[instanceID] = true;
		SetManipulator(YAML::BeginDoc);
		//m_emitter << " !u! " << guid;
		//m_emitter << YAML::LocalTag("u", boost::uuids::to_string(guid));
		//assert(m_emitter.good());
		SetManipulator(YAML::BeginMap);
		m_emitter << "fileID" << obj->GetInstanceID();
		m_emitter << obj->ClassName();
		//this->operator<<(*obj);
		//DynamicSerializeObject(*this, obj);
		SetManipulator(YAML::BeginMap);
		obj->Serialize(*this);
		SetManipulator(YAML::EndMap);
		//(*this) << obj;
		SetManipulator(YAML::EndMap);
		SetManipulator(YAML::EndDoc);
	}
}
