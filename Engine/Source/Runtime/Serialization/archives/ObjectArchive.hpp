#include <Serialization/archives/YAMLArchive.hpp>
#include <cassert>

namespace FishEngine
{
	class ObjectInputArchive : public InputArchive
	{

	};


	class ObjectOutputArchive : public YAMLOutputArchive
	{
	public:
		ObjectOutputArchive(std::ostream & os) : YAMLOutputArchive(os)
		{

		}

		virtual ~ObjectOutputArchive() = default;

		virtual void BeginDoc() override
		{
			// call BeginDoc(int classID, int fileID) instead
			abort();
		}

		void BeginDoc(int classID, int fileID)
		{
			assert(!m_isInsideDoc);
			m_isInsideDoc = true;
			m_emitter.EmitBeginDoc_FishEngine(classID, fileID);
		}

		virtual void EndDoc() override
		{
			m_isInsideDoc = false;
			m_emitter.EmitEndDoc_FishEngine();
		}

	protected:
		virtual void SerializeObject(FishEngine::ObjectPtr const & object) override;

		int m_nextFileID = 1;
		int m_totalCount = 0;

	private:

		void SerializeObject_impl(FishEngine::ObjectPtr const & obj);

		std::map<int, int> m_serialized;	// instanceID to fileID
		std::deque<std::pair<int, std::shared_ptr<FishEngine::Object>>> m_objectsToBeSerialized; // fileID
		bool m_isInsideDoc = false;
	};
}