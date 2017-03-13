#pragma once
#include <Archive.hpp>
#include <yaml-cpp/yaml.h>
#include <cassert>
#include <stack>

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

		const YAML::Node & CurrentNode()
		{
			if (m_workingNodes.empty())
			{
				m_workingNodes.push(m_nodes[m_nodeIndex]);
			}
			
			if (m_expectedNextType == Type::MapKey)
			{
				m_expectedNextType = Type::MapValue;
				m_workingNodes.pop();	// pop value node
				auto key = m_mapOrSequenceiterator->first;
				m_workingNodes.push(key);
			}
			else if (m_expectedNextType == Type::MapValue)
			{
				m_expectedNextType = Type::MapKey;
				m_workingNodes.pop();	// pop key node
				auto value = m_mapOrSequenceiterator->second;
				++m_mapOrSequenceiterator;
				m_workingNodes.push(value);
			}
			return m_workingNodes.top();
		}

		void ToNextNode()
		{
			m_nodeIndex++;
		}

		template<class T, std::enable_if_t<std::is_base_of<Object, T>::value, int> = 0>
		std::shared_ptr<T> DeserializeObject()
		{
			auto currentNode = CurrentNode();
			assert(CurrentNode().IsMap());
			m_workingNodes.push(currentNode[T::StaticClassName()]);
			auto p = std::make_shared<T>();
			p->Deserialize(*this);
			m_workingNodes.pop();
			return p;
		}
		
		virtual void BeginClass() override
		{
			//m_workingNodes.push()
		}
		
		virtual void EndClass() override
		{
		}
		
		virtual void BeginMap() override
		{
			m_mapOrSequenceiterator = CurrentNode().begin();
			m_expectedNextType = Type::MapKey;
		}
		
		//virtual void GetMapItem() {}
		virtual void EndMap() override
		{
			// the top node of m_workingNodes if map node itself (if this map is empty) or the last MapValue node of this map
			//m_workingNodes.pop();
			m_expectedNextType = Type::None;
		}

	protected:

		virtual void Deserialize(short & t) override { Convert(CurrentNode(), t); }
		virtual void Deserialize(unsigned short & t) override { Convert(CurrentNode(), t); }
		virtual void Deserialize(int & t) override { Convert(CurrentNode(), t); }
		virtual void Deserialize(unsigned int & t) override { Convert(CurrentNode(), t); }
		virtual void Deserialize(long & t) override { Convert(CurrentNode(), t); }
		virtual void Deserialize(unsigned long & t) override { Convert(CurrentNode(), t); }
		virtual void Deserialize(long long & t) override { Convert(CurrentNode(), t); }
		virtual void Deserialize(unsigned long long & t) override { Convert(CurrentNode(), t); }
		virtual void Deserialize(float & t) override { Convert(CurrentNode(), t); }
		virtual void Deserialize(double & t) override { Convert(CurrentNode(), t); }
		virtual void Deserialize(bool & t) override { Convert(CurrentNode(), t); }
		virtual void Deserialize(std::string & t) override { Convert(CurrentNode(), t); }

		virtual void DeserializeObject(FishEngine::ObjectPtr const & obj) override
		{

		}

		virtual void DeserializeWeakObject(std::weak_ptr<FishEngine::Object> const & obj) override
		{

		}


		virtual void EndNVP() override
		{
			m_workingNodes.pop();
		}

		virtual void NameOfNVP(const char* name) override
		{
			auto currentNode = CurrentNode();
			assert(currentNode.IsMap());
			//assert(currentNode.front());
			m_workingNodes.push(currentNode[name]);
		}

		virtual void MiddleOfNVP() override
		{

		}
		
		virtual std::size_t GetSizeTag() override
		{
			auto currentNode = CurrentNode();
			assert(currentNode.IsMap() || currentNode.IsSequence());
			return currentNode.size();
		}

	protected:

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
		
		std::vector<YAML::Node> m_nodes;
		uint32_t m_nodeIndex = 0;
		std::stack<YAML::Node> m_workingNodes;
		
		enum class Type
		{
			None,
			MapKey,
			MapValue,
		};
		
		Type					m_expectedNextType = Type::None;
		YAML::const_iterator	m_mapOrSequenceiterator;
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
