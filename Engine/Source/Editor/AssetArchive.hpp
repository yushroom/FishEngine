#include <Archive.hpp>

#include <yaml-cpp/yaml.h>

namespace FishEditor
{
	class AssetInputArchive : public ::FishEngine::InputArchive
	{
	public:
		AssetInputArchive(std::istream & is) : FishEngine::InputArchive(is) { }

		virtual ~AssetInputArchive() = default;
	};


	class AssetOutputArchive : public ::FishEngine::OutputArchive
	{
	public:
		AssetOutputArchive(std::ostream & os) : OutputArchive(os), m_emitter(os)
		{

		}

		virtual ~AssetOutputArchive() = default;

		virtual void BeginDoc()
		{
			m_emitter << YAML::BeginDoc;
		}

		virtual void EndDoc()
		{
			m_emitter << YAML::EndDoc;
		}

		virtual void BeginClass() override
		{
			BeginMap();
		}

		virtual void EndClass() override
		{
			EndMap();
		}

		void BeginFlow()
		{
			m_emitter << YAML::Flow;
		}

		virtual void BeginSequence(std::size_t sequenceSize) override
		{
			if (sequenceSize <= 0)
				BeginFlow();
			m_emitter << YAML::BeginSeq;
		}
		virtual void EndSequence() override
		{
			m_emitter << YAML::EndSeq;
		}

		virtual void BeginMap() override
		{
			m_emitter << YAML::BeginMap;
		}

		virtual void EndMap() override
		{
			m_emitter << YAML::EndMap;
		}

	protected:
		virtual void Serialize(short t) override { m_emitter << t; }
		virtual void Serialize(unsigned short t) override { m_emitter << t; }
		virtual void Serialize(int t) override { m_emitter << t; }
		virtual void Serialize(unsigned int t) override { m_emitter << t; }
		virtual void Serialize(long t) override { m_emitter << t; }
		virtual void Serialize(unsigned long t) override { m_emitter << t; }
		virtual void Serialize(long long t) override { m_emitter << t; }
		virtual void Serialize(unsigned long long t) override { m_emitter << t; }
		virtual void Serialize(float t) override { m_emitter << t; }
		virtual void Serialize(double t) override { m_emitter << t; }
		virtual void Serialize(bool t) override { m_emitter << t; }
		virtual void Serialize(std::string t) override { m_emitter << t; }
		virtual void Serialize(const char* t) override { m_emitter << t; }

		virtual void Serialize(std::nullptr_t const & t) override
		{
			BeginFlow();
			BeginMap();
			(*this) << FishEngine::make_nvp("fileID", 0);
			EndMap();
		}

		virtual void SerializeObject(FishEngine::ObjectPtr const & obj) override
		{
			if (obj == nullptr)
			{
				(*this) << nullptr;
				return;
			}

			BeginDoc();
			BeginMap();
			m_emitter << obj->ClassName();
			BeginMap();
			obj->Serialize(*this);
			EndMap();
			EndMap();
			EndDoc();
		}

		virtual void SerializeWeakObject(std::weak_ptr<FishEngine::Object> const & obj) override
		{
			auto t = obj.lock();
			this->SerializeObject(t);
		}

		virtual void SerializeNameOfNVP(const char* name) override
		{
			m_emitter << name;
		}

		virtual void MiddleOfNVP() override
		{
		}

		virtual void EndNVP() override
		{
			//m_emitter << ::YAML::EndMap;
		}

	protected:
		YAML::Emitter m_emitter;
	};
}
