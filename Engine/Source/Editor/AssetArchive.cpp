#include "AssetArchive.hpp"
#include <sstream>

#include <Debug.hpp>
#include <Material.hpp>

#include "AssetImporter.hpp"
#include "TextureImporter.hpp"
#include "ModelImporter.hpp"
#include "FBXImporter.hpp"
#include "NativeFormatImporter.hpp"

using namespace FishEngine;

void PrintNode2(YAML::Node const & node)
{
	if (node.IsScalar())
	{
		Debug::Log("%s", node.as<std::string>().c_str());
	}
	else if (node.IsSequence())
	{
		for (auto it = node.begin(); it != node.end(); ++it)
		{
			PrintNode2(*it);
		}
	}
	else if (node.IsMap())
	{
		for (auto it = node.begin(); it != node.end(); ++it)
		{
			PrintNode2(it->first);
			PrintNode2(it->second);
		}
	}
}

std::vector<FishEngine::ObjectPtr> FishEditor::AssetInputArchive::LoadAll()
{
	std::vector<FishEngine::ObjectPtr> objects;
	for (auto & node : m_nodes)
	{
		PrintNode2(node);
	}

	return objects;
}


FishEngine::MaterialPtr FishEditor::MaterialArchive::DeserializeMaterial()
{
	if (m_nodes.size() != 1 && !m_nodes.front().IsMap())
	{
		abort();
	}

	auto & node = m_nodes.front();

	for (auto it = node.begin(); it != node.end(); ++it)
	{
		auto key = it->first.as<std::string>();

		if (key != "Material")
		{
			abort();
		}

		auto material = std::make_shared<Material>();
		m_workingNodes.push(it->second);
		material->Deserialize(*this);
		m_workingNodes.pop();
		return material;
	}
	return nullptr;
}


uint32_t FishEditor::MetaInputArchive::timeStamp() const
{
	if (m_nodes.size() != 1 && !m_nodes.front().IsMap())
	{
		abort();
	}

	auto & node = m_nodes.front();
	uint32_t meta_created_time = node["timeCreated"].as<uint32_t>();
	return meta_created_time;
}


FishEditor::AssetImporterPtr FishEditor::MetaInputArchive::DeserializeAssetImporter()
{
	if (m_nodes.size() != 1 && !m_nodes.front().IsMap())
	{
		abort();
	}

	auto & node = m_nodes.front();
	uint32_t meta_created_time = node["timeCreated"].as<uint32_t>();
	GUID guid;
	std::string strGUID = node["guid"].as<std::string>();
	std::istringstream sin(strGUID);
	sin >> guid;

	for (auto it = node.begin(); it != node.end(); ++it)
	{
		auto key = it->first.as<std::string>();
		if (key == "timeCreated" || key == "guid")
		{
			continue;
		}

		AssetImporterPtr importer;
		if (key == "TextureImporter")
		{
			importer = std::make_shared<TextureImporter>();
		}
		else if (key == "ModelImporter")
		{
			importer = std::make_shared<FBXImporter>();
		}
		else if (key == "NativeFormatImporter")
		{
			importer = std::make_shared<NativeFormatImporter>();
		}
		else
		{
			abort();
		}
		m_workingNodes.push(it->second);
		importer->Deserialize(*this);
		m_workingNodes.pop();
		importer->m_guid = guid;
		importer->m_assetTimeStamp = meta_created_time;
		return importer;
	}
	return nullptr;
}
