#pragma once

#include <deque>
#include <set>
#include <vector>
#include <cassert>

#include <Serialization/archives/YAMLArchive.hpp>

namespace FishEditor
{
	class SceneInputArchive : public FishEngine::YAMLInputArchive
	{
	public:
		SceneInputArchive(std::istream & is) 
			: FishEngine::YAMLInputArchive(is)
		{
		}

		void LoadAll();

	protected:
	};

	class SceneOutputArchive : public FishEngine::YAMLOutputArchive
	{
	public:
		SceneOutputArchive(std::ostream & os) : FishEngine::YAMLOutputArchive(os)
		{
			m_emitter.EmitHeader_FishEngine();
		}

		virtual ~SceneOutputArchive() = default;

		virtual void BeginDoc() override
		{
			// call BeginDoc(int classID, int fileID) instead
			abort();
		}

		void BeginDoc(int classID, int fileID)
		{
			assert(!m_isInsideDoc);
			m_isInsideDoc = true;
			//m_emitter << YAML::BeginDoc;
			m_emitter.EmitBeginDoc_FishEngine(classID, fileID);
		}

		virtual void EndDoc() override
		{
			m_isInsideDoc = false;
			//m_emitter << YAML::EndDoc;
			m_emitter.EmitEndDoc_FishEngine();
		}
		
		void setSerializePrefab(bool value)
		{
			m_serializePrefab = value;
		}

	protected:
		virtual void SerializeObject(FishEngine::ObjectPtr const & object) override;

		int m_nextFileID = 1;
		int m_totalCount = 0;

	private:

		void SerializeObject_impl(FishEngine::ObjectPtr obj);

		std::map<int, int> m_serialized;	// instanceID to fileID
		std::deque<std::pair<int, std::shared_ptr<FishEngine::Object>>> m_objectsToBeSerialized; // fileID
		bool m_isInsideDoc = false;
		
		// TODO: delete it
		bool m_serializePrefab = false;
	};
}
