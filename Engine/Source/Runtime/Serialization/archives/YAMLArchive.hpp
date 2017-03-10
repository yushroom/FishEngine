#pragma once
#include <Archive.hpp>
#include <yaml-cpp/yaml.h>
#include <cassert>

namespace FishEngine
{
	class YAMLInputArchive : public InputArchive
	{
	public:
		YAMLInputArchive(std::istream & is)
			: InputArchive(is), m_nodes(YAML::LoadAll(is))
		{

		}

		virtual ~YAMLInputArchive() = default;

		YAML::Node CurrentNode()
		{
			if (m_node == nullptr)
				return m_nodes[m_nodeIndex];
			return *m_node;
		}

		void ToNextNode()
		{
			m_nodeIndex++;
		}

		template<class T, std::enable_if_t<std::is_base_of<Object, T>::value, int> = 0>
		std::shared_ptr<T> DeserializeObject()
		{
			assert(CurrentNode().IsMap());
			auto node = CurrentNode()[T::StaticClassName()];
			m_node = &node;
			auto p = std::make_shared<T>();
			(*this) >> *p;
			m_node = nullptr;
			return p;
		}

	protected:

		virtual void Deserialize(short & t) { Convert(CurrentNode(), t); }
		virtual void Deserialize(unsigned short & t) { Convert(CurrentNode(), t); }
		virtual void Deserialize(int & t) { Convert(CurrentNode(), t); }
		virtual void Deserialize(unsigned int & t) { Convert(CurrentNode(), t); }
		virtual void Deserialize(long & t) { Convert(CurrentNode(), t); }
		virtual void Deserialize(unsigned long & t) { Convert(CurrentNode(), t); }
		virtual void Deserialize(long long & t) { Convert(CurrentNode(), t); }
		virtual void Deserialize(unsigned long long & t) { Convert(CurrentNode(), t); }
		virtual void Deserialize(float & t) { Convert(CurrentNode(), t); }
		virtual void Deserialize(double & t) { Convert(CurrentNode(), t); }
		virtual void Deserialize(bool & t) { Convert(CurrentNode(), t); }
		virtual void Deserialize(std::string & t) { Convert(CurrentNode(), t); }

		virtual void DeserializeObject(FishEngine::ObjectPtr const & obj) override
		{

		}

		virtual void DeserializeWeakObject(std::weak_ptr<FishEngine::Object> const & obj) override
		{

		}


		virtual void EndNVP() override
		{

		}

		virtual void NameOfNVP(const char* name) override
		{
			assert(CurrentNode().IsMap());
			m_node = &CurrentNode()[name];
		}

		virtual void MiddleOfNVP() override
		{

		}

	private:
		std::vector<YAML::Node> m_nodes;
		uint32_t m_nodeIndex = 0;
		YAML::Node * m_node = nullptr;
		 

		static void Convert(YAML::Node const & node, std::string & t)
		{
			t = node.as<std::string>();
			//std::cout << "as string: " << t << std::endl;
		}

		static void Convert(YAML::Node const & node, boost::uuids::uuid & t)
		{
			assert(node.IsMap());
			//t = boost::lexical_cast<boost::uuids::uuid>(node["fileID"].as<std::string>());
			std::istringstream sin(node["fileID"].as<std::string>());
			sin >> t;
		}


		template<class T, std::enable_if_t<std::is_arithmetic<T>::value, int> = 0>
		static void Convert(YAML::Node const & node, T & t)
		{
			t = node.as<T>();
		}

		template<class T, std::enable_if_t<std::is_enum<T>::value, int> = 0>
		static void Convert(YAML::Node const & node, T & t)
		{
			//std::cout << node.Scalar() << std::endl;
			t = static_cast<T>(node.as<std::underlying_type_t<T>>());
		}
	};


	class YAMLOutputArchive : public OutputArchive
	{
	public:
		YAMLOutputArchive(std::ostream & os) : OutputArchive(os), m_emitter(os)
		{

		}

		virtual ~YAMLOutputArchive() = default;

		virtual void BeginDoc()
		{
			m_emitter << YAML::BeginDoc;
		}

		virtual void EndDoc()
		{
			//m_emitter << YAML::EndDoc;
			m_emitter.EmitEndDoc_FishEngine();
		}

		virtual void BeginClass() override
		{
			BeginMap(1);
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

		virtual void BeginMap(std::size_t mapSize) override
		{ 
			if (mapSize == 0)
				BeginFlow();
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
			BeginMap(1);
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
			BeginMap(1);	// do not known map size
			m_emitter << obj->ClassName();
			BeginMap(1);	// do not known map size
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
