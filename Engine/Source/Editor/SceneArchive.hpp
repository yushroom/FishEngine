#pragma once

#include <deque>
#include <set>
#include <vector>

#include "AssetArchive.hpp"

namespace FishEditor
{
	class SceneInputArchive : public AssetInputArchive
	{
	public:
		SceneInputArchive(std::istream & is) 
			: AssetInputArchive(is), m_nodes(YAML::LoadAll(is))
		{
		}

	protected:
		std::vector<YAML::Node> m_nodes;
		//uint32_t m_nodeIndex = 0;
	};

	class SceneOutputArchive : public AssetOutputArchive
	{
	public:
		SceneOutputArchive(std::ostream & os) : AssetOutputArchive(os)
		{
			m_emitter.EmitHeader_FishEngine();
		}

		virtual ~SceneOutputArchive() = default;

		virtual void BeginDoc() override
		{
			// should call m_emitter.EmitBeginDoc_FishEngine directly
			abort();
			//assert(!m_isInsideDoc);
			//m_isInsideDoc = true;
			//m_emitter << YAML::BeginDoc;
		}

		void BeginDoc(int classID, int fileID)
		{
			assert(!m_isInsideDoc);
			m_isInsideDoc = true;
			//m_emitter << YAML::BeginDoc;
			m_emitter.EmitBeginDoc_FishEngine(classID, m_nextFileID);
		}

		virtual void EndDoc() override
		{
			m_isInsideDoc = false;
			//m_emitter << YAML::EndDoc;
			m_emitter.EmitEndDoc_FishEngine();
			if (!m_objectsToBeSerialized.empty())
			{
				auto item = m_objectsToBeSerialized.front();
				m_objectsToBeSerialized.pop_front();
				int fileID = item.first;
				auto obj = item.second;
				int oldFileId = m_nextFileID;
				m_nextFileID = fileID;
				SerializeObject(obj);
				m_nextFileID = oldFileId;
			}
		}

	protected:
		virtual void SerializeObject(FishEngine::ObjectPtr const & obj) override;

		int m_nextFileID = 1;

	private:

		std::set<int> m_serialized;	// instanceID
		std::deque<std::pair<int, std::shared_ptr<FishEngine::Object>>> m_objectsToBeSerialized; // fileID
		bool m_isInsideDoc = false;
	};
}