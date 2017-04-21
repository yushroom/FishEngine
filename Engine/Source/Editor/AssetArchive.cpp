#include "AssetArchive.hpp"
#include <sstream>

#include <Debug.hpp>
#include <Material.hpp>

#include "AssetImporter.hpp"
#include "TextureImporter.hpp"
#include "ModelImporter.hpp"
#include "FBXImporter.hpp"
#include "NativeFormatImporter.hpp"
#include "ShaderImporter.hpp"
#include "DDSImporter.hpp"
#include "AudioImporter.hpp"

using namespace FishEngine;

void PrintNode2(YAML::Node const & node)
{
	if (node.IsScalar())
	{
		LogInfo(node.as<std::string>());
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

		auto material = MakeShared<Material>();
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
			importer = MakeShared<TextureImporter>();
		}
		else if (key == "ModelImporter")
		{
			importer = MakeShared<FBXImporter>();
		}
		else if (key == "NativeFormatImporter")
		{
			importer = MakeShared<NativeFormatImporter>();
		}
		else if (key == "ShaderImporter")
		{
			importer = MakeShared<ShaderImporter>();
		}
		else if (key == "DDSImporter")
		{
			importer = MakeShared<DDSImporter>();
		}
		else if (key == "AudioImporter")
		{
			importer = MakeShared<AudioImporter>();
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
